#pragma once

#include <string>

namespace oai::config::smf {
class smf_config; // Forward declaration
} // namespace oai::config::smf

namespace oai::config {

/**
 * @brief Starts a dedicated thread to monitor the specified configuration file for modifications.
 *
 * @param config_file_path The absolute or relative path to the configuration file.
 * @param config_inst Pointer to the smf_config instance to be reloaded.
 * @return true if the monitor thread was started successfully, false otherwise.
 */
bool start_config_monitor_thread(const std::string& config_file_path, oai::config::smf::smf_config* config_inst);

/**
 * @brief Handler function called when a configuration file modification is detected.
 *        This function will eventually trigger the configuration reload process.
 */
void handle_config_change();

} // namespace oai::config 