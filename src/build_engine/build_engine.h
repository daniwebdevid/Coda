#ifndef BUILD_ENGINE_H
#define BUILD_ENGINE_H

/**
 * @brief Builds the project based on the configuration file.
 * @param config_path The path to the coda.json file.
 * @return 0 on success, 1 on failure.
 */
int build_project(const char *config_path);

#endif // BUILD_ENGINE_H