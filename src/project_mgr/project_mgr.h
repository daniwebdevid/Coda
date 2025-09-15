#ifndef PROJECT_MGR_H
#define PROJECT_MGR_H

#include <jansson.h>

/**
 * @struct ProjectConfig
 * @brief A structure to hold parsed project configuration data from coda.json.
 */
typedef struct {
    char *project_name;
    const char **source_files;
    const char **dependencies; // This will hold the key-value pairs
    char *compiler;
    char *output_path;
} ProjectConfig;

/**
 * @brief Parses a coda.json file and populates the ProjectConfig struct.
 * @param path The path to the coda.json file.
 * @param config A pointer to the struct to be populated.
 * @return 0 on success, 1 on failure.
 */
int parse_config_from_file(const char *path, ProjectConfig *config);

/**
 * @brief Adds a new dependency entry to the project's coda.json file.
 * @param package_name The name of the package to add.
 * @param repo_url The Git repository URL of the package.
 * @return 0 on success, 1 on failure.
 */
int add_dependency_to_project_config(const char *package_name, const char *repo_url);

/**
 * @brief Frees all dynamically allocated memory within the ProjectConfig struct.
 * @param config The struct to be freed.
 */
void free_config(ProjectConfig *config);

#endif // PROJECT_MGR_H