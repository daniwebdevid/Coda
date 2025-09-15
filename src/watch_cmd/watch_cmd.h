#ifndef WATCH_CMD_H
#define WATCH_CMD_H

/**
 * @brief Starts monitoring the project's source directory for changes and rebuilds automatically.
 * @param config_path The path to the coda.json file.
 * @return 0 on success, 1 on failure.
 */
int watch_project(const char *config_path);

#endif // WATCH_CMD_H