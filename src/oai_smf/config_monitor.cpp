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
#include <libgen.h> // For dirname
#include <vector>   // For storing directory path string

#include "../common/logger.hpp" // Corrected include path
#include "smf_config.hpp" // Corrected include path - removed smf_app/
#include "smf_app.hpp" // Corrected include path - removed smf_app/

// Access the global smf_app instance (declared in main.cpp)
extern smf::smf_app* smf_app_inst;

namespace oai::config {

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + 16))

static pthread_t monitor_thread_id;
static std::string monitored_file_path;
static std::string monitored_dir_path;
static std::string monitored_filename;
static std::atomic<oai::config::smf::smf_config*> smf_config_instance = {nullptr}; // Store the instance pointer

// Thread function to monitor the file
static void* monitor_file(void* arg) {
    Logger::smf_app().info("monitor_file thread started.");
    int fd = -1;
    int wd = -1;
    char buffer[BUF_LEN];

    // Extract directory path and filename
    std::vector<char> file_path_vec(monitored_file_path.begin(), monitored_file_path.end());
    file_path_vec.push_back('\0'); // Null-terminate for dirname/basename
    char* dir_path_cstr = dirname(file_path_vec.data());
    monitored_dir_path = std::string(dir_path_cstr);
    // Need to copy again for basename as dirname might modify the input string
    std::vector<char> file_path_vec_copy(monitored_file_path.begin(), monitored_file_path.end());
    file_path_vec_copy.push_back('\0');
    char* filename_cstr = basename(file_path_vec_copy.data());
    monitored_filename = std::string(filename_cstr);

    Logger::smf_app().info("Starting configuration directory monitor thread for directory: %s, watching for file: %s", 
                         monitored_dir_path.c_str(), monitored_filename.c_str());

    fd = inotify_init1(IN_NONBLOCK);
    if (fd < 0) {
        Logger::smf_app().error("inotify_init1 failed: %s", strerror(errno));
        return nullptr;
    }

    // Watch the DIRECTORY for relevant events
    // IN_CREATE: Catches creation of ..data temp dir/file
    // IN_MOVED_TO: Catches the atomic symlink update
    // IN_CLOSE_WRITE: Might catch direct writes if symlink isn't used
    Logger::smf_app().debug("Attempting to add inotify watch for directory: %s", monitored_dir_path.c_str());
    wd = inotify_add_watch(fd, monitored_dir_path.c_str(), 
                           IN_CREATE | IN_MOVED_TO | IN_CLOSE_WRITE);
    if (wd < 0) {
        Logger::smf_app().error("inotify_add_watch failed for directory %s: %s", monitored_dir_path.c_str(), strerror(errno));
        close(fd);
        return nullptr;
    }

    Logger::smf_app().debug("inotify watch added successfully for directory: %s (wd=%d)", monitored_dir_path.c_str(), wd);

    while (true) { // Loop indefinitely (or until thread cancellation is implemented)
        Logger::smf_app().trace("monitor_file loop: Waiting for inotify events...");
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

            // We are watching a directory now
            // Only trigger reload if the event pertains to our specific config file
            // or the special ..data directory Kubernetes uses for atomic updates.
            std::string event_filename = (event->len > 0) ? event->name : "";
            bool trigger_reload = false;

            if (event->mask & (IN_CREATE | IN_MOVED_TO | IN_CLOSE_WRITE)) {
                if (event_filename == monitored_filename || 
                    event_filename == "..data" ||          // K8s temp data dir
                    event_filename == "..config.yaml") { // K8s uses this sometimes for the link
                    
                    Logger::smf_app().info(
                        "Detected relevant event (mask=0x%x, name='%s') in config directory: %s", 
                        event->mask, event_filename.c_str(), monitored_dir_path.c_str());
                    trigger_reload = true;
                } else if (event_filename.empty() && (event->mask & IN_CLOSE_WRITE)) {
                    // CLOSE_WRITE on the directory itself (less common, but possible?)
                    // Or maybe a direct write to the file without a filename in event?
                    // Let's cautiously trigger a reload, but log it clearly.
                     Logger::smf_app().warn(
                        "Detected CLOSE_WRITE event with no filename in config directory: %s. Triggering reload.", 
                        monitored_dir_path.c_str());
                    trigger_reload = true;
                }
            }

            if (trigger_reload) {
                // Add a small delay to allow filesystem operations to settle, 
                // especially after atomic symlink swaps.
                usleep(100000); // 100ms delay
                handle_config_change();
            } else if (event->mask & IN_IGNORED) {
                // The watch was removed, perhaps the file was deleted or filesystem unmounted
                 Logger::smf_app().warn("inotify watch for %s ignored. Re-adding watch.", monitored_file_path.c_str());
                 // Attempt to re-add the watch. This might fail if the directory is gone.
                 inotify_rm_watch(fd, wd); // Remove the old watch descriptor explicitly
                 // Re-add the directory watch
                 wd = inotify_add_watch(fd, monitored_dir_path.c_str(), 
                                        IN_CREATE | IN_MOVED_TO | IN_CLOSE_WRITE);
                 if (wd < 0) {
                    Logger::smf_app().error("inotify_add_watch failed on re-attempt for directory %s: %s. Stopping monitor.", 
                                           monitored_dir_path.c_str(), strerror(errno));
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
        Logger::smf_app().info("Reload attempt finished. Detected %ld DNN changes.", dnn_changes.size());

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
                    Logger::smf_app().info("DNN change propagation triggered.");

                    // Regenerate profile and update NRF registration
                    Logger::smf_app().info("Regenerating SMF profile after DNN changes.");
                    smf_app_inst->generate_smf_profile();
                    smf_app_inst->trigger_nrf_profile_update();

                } else {
                    Logger::smf_app().error("smf_app_inst is null. Cannot trigger DNN updates or NRF profile update.");
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