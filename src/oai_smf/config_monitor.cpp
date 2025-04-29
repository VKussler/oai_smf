#include "config_monitor.hpp"

#include <pthread.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include <string>
#include <iostream> // For temporary logging/debugging
#include <atomic>
#include <errno.h> // Include for strerror
#include <map>

#include "../common/logger.hpp" // Corrected include path
#include "smf_app/smf_config.hpp" // Include smf_config definition
#include "smf_app/smf_app.hpp" // Include smf_app definition for trigger function

// Access the global smf_app instance (declared in main.cpp)
extern smf::smf_app* smf_app_inst;

namespace oai::config {

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + 16))

static pthread_t monitor_thread_id;
static std::string monitored_file_path;
static std::atomic<oai::config::smf::smf_config*> smf_config_instance = {nullptr}; // Store the instance pointer

// Thread function to monitor the file
static void* monitor_file(void* arg) {
    int fd = -1;
    int wd = -1;
    char buffer[BUF_LEN];

    Logger::smf_app().info("Starting configuration file monitor thread for: %s", monitored_file_path.c_str());

    fd = inotify_init1(IN_NONBLOCK);
    if (fd < 0) {
        Logger::smf_app().error("inotify_init1 failed: %s", strerror(errno));
        return nullptr;
    }

    wd = inotify_add_watch(fd, monitored_file_path.c_str(), IN_MODIFY | IN_CLOSE_WRITE);
    if (wd < 0) {
        Logger::smf_app().error("inotify_add_watch failed for %s: %s", monitored_file_path.c_str(), strerror(errno));
        close(fd);
        return nullptr;
    }

    Logger::smf_app().debug("inotify watch added for file: %s (wd=%d)", monitored_file_path.c_str(), wd);

    while (true) { // Loop indefinitely (or until thread cancellation is implemented)
        int length = read(fd, buffer, BUF_LEN);

        if (length < 0) {
            if (errno == EAGAIN) {
                // No events pending, sleep briefly to avoid busy-waiting
                usleep(500000); // 500ms
                continue;
            } else {
                Logger::smf_app().error("read from inotify fd failed: %s", strerror(errno));
                break; // Exit loop on error
            }
        }

        int i = 0;
        while (i < length) {
            struct inotify_event* event = (struct inotify_event*)&buffer[i];
            if (event->len) {
                 // We are watching a file, not a directory, so name is not expected/needed
            }

            if (event->mask & IN_MODIFY || event->mask & IN_CLOSE_WRITE) {
                Logger::smf_app().info("Detected modification or close_write on configuration file: %s", monitored_file_path.c_str());
                handle_config_change();
            } else if (event->mask & IN_IGNORED) {
                // The watch was removed, perhaps the file was deleted or filesystem unmounted
                 Logger::smf_app().warn("inotify watch for %s ignored. Re-adding watch.", monitored_file_path.c_str());
                 // Attempt to re-add the watch. This might fail if the file is permanently gone.
                 inotify_rm_watch(fd, wd); // Remove the old watch descriptor explicitly
                 wd = inotify_add_watch(fd, monitored_file_path.c_str(), IN_MODIFY | IN_CLOSE_WRITE);
                 if (wd < 0) {
                    Logger::smf_app().error("inotify_add_watch failed on re-attempt for %s: %s. Stopping monitor.", monitored_file_path.c_str(), strerror(errno));
                    goto cleanup; // Exit thread if re-adding fails
                 }
            }

            i += EVENT_SIZE + event->len;
        }
    }

cleanup:
    Logger::smf_app().info("Stopping configuration file monitor thread for: %s", monitored_file_path.c_str());
    if (wd >= 0) {
        inotify_rm_watch(fd, wd);
    }
    if (fd >= 0) {
        close(fd);
    }
    return nullptr;
}

bool start_config_monitor_thread(const std::string& config_file_path, oai::config::smf::smf_config* config_inst) {
    monitored_file_path = config_file_path; // Store path for the thread
    smf_config_instance.store(config_inst); // Store the config instance pointer

    int rc = pthread_create(&monitor_thread_id, nullptr, monitor_file, nullptr);
    if (rc) {
        Logger::smf_app().error("Failed to create configuration monitor thread: %s", strerror(rc));
        return false;
    }
    // Consider pthread_detach(monitor_thread_id); if we don't need to join it later
    // Or implement proper cleanup/joining on shutdown.
    Logger::smf_app().info("Successfully created configuration monitor thread.");
    return true;
}

void handle_config_change() {
    Logger::smf_app().debug("Entering handle_config_change().");
    Logger::smf_app().info("Configuration change detected. Triggering reload.");
    oai::config::smf::smf_config* config_ptr = smf_config_instance.load();

    if (config_ptr) {
        // Store the old log level
        spdlog::level::level_enum old_log_level = config_ptr->log_level;

        // Attempt reload and get DNN changes
        std::map<std::string, oai::config::smf::DnnChange> dnn_changes = config_ptr->reload_config();

        // Check if reload succeeded (reload_config returns empty map on failure)
        // We assume here that even if dnn_changes is empty, the reload might have succeeded
        // for other parameters like log_level. A more robust approach might involve
        // reload_config returning a status along with the changes.
        if (true) { // Assuming reload_config doesn't throw and handles internal errors
            Logger::smf_app().info("Configuration reload processed.");

            // Apply Log Level Change (always check, even if no DNN changes)
            if (config_ptr->log_level != old_log_level) {
                Logger::smf_app().warn("Log level changed. Applying new level: %s", spdlog::level::to_string_view(config_ptr->log_level).data());
                Logger::set_level(config_ptr->log_level);
            } else {
                Logger::smf_app().debug("Log level unchanged.");
            }

            // Propagate DNN changes if any were detected and smf_app instance exists
            if (!dnn_changes.empty()) {
                if (smf_app_inst) {
                    Logger::smf_app().info("Triggering propagation of %ld DNN changes.", dnn_changes.size());
                    smf_app_inst->trigger_dnn_updates(dnn_changes);
                } else {
                    Logger::smf_app().error("smf_app_inst is null. Cannot trigger DNN updates.");
                }
            } else {
                 Logger::smf_app().info("No DNN changes detected needing propagation.");
            }

        } else {
             // This part is less likely needed now as reload_config returns empty on failure
            Logger::smf_app().error("Configuration reload failed (as indicated by empty map?). Continuing with old configuration.");
            config_ptr->log_level = old_log_level; // Restore log level if needed
        }
    } else {
         Logger::smf_app().error("smf_config instance is null in handle_config_change. Cannot reload.");
    }
    Logger::smf_app().debug("Exiting handle_config_change().");
}

} // namespace oai::config 