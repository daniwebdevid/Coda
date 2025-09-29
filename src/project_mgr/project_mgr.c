#include "project_mgr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

/**
 * @brief Parses a JSON array of strings and stores them into a char** pointer.
 * @param parent_json The root JSON object.
 * @param key The key to look up (e.g., "compiler_flags").
 * @param target_ptr Pointer to the char** field in ProjectConfig.
 * @return 0 on success, 1 on failure.
 */
static int parse_string_array(json_t *parent_json, const char *key, const char ***target_ptr) {
    json_t *json_array = json_object_get(parent_json, key);
    
    // Check if the key exists and is an array. If it doesn't exist, we treat it as an empty array (success).
    if (!json_array) {
        *target_ptr = (const char **)calloc(1, sizeof(char *)); // Allocate space for a single NULL terminator
        return 0; 
    }

    if (!json_is_array(json_array)) {
        fprintf(stderr, "Error: '%s' must be a JSON array.\n", key);
        return 1;
    }

    size_t count = json_array_size(json_array);
    // Allocate space for all strings + 1 for the NULL terminator
    *target_ptr = (const char **)malloc(sizeof(char *) * (count + 1));
    if (*target_ptr == NULL) {
        perror("Failed to allocate memory for config array");
        return 1;
    }

    for (size_t i = 0; i < count; i++) {
        json_t *item_json = json_array_get(json_array, i);
        if (json_is_string(item_json)) {
            (*target_ptr)[i] = strdup(json_string_value(item_json));
        } else {
            fprintf(stderr, "Error: All items in '%s' must be strings.\n", key);
            // Clean up already allocated strings before returning error
            for (size_t j = 0; j < i; j++) free((void*)(*target_ptr)[j]);
            free((void*)*target_ptr);
            *target_ptr = NULL;
            return 1;
        }
    }
    // Add the mandatory NULL terminator
    (*target_ptr)[count] = NULL;
    return 0;
}

/**
 * @brief Helper to free a NULL-terminated array of strings.
 * @param arr The array to free.
 */
static void free_string_array(const char **arr) {
    if (!arr) return;
    for (int i = 0; arr[i] != NULL; i++) {
        free((void*)arr[i]);
    }
    free(arr);
}


int parse_config_from_file(const char *path, ProjectConfig *config) {
    json_t *root;
    json_error_t error;

    // Initialize all array pointers to NULL for safety during error cleanup
    config->source_files = NULL;
    config->dependencies = NULL;
    config->compiler_flags = NULL;
    config->linker_flags = NULL;
    config->include_paths = NULL;


    // 1. Load the JSON configuration file
    root = json_load_file(path, 0, &error);
    if (!root) {
        fprintf(stderr, "Error parsing JSON file '%s': on line %d: %s\n", path, error.line, error.text);
        return 1;
    }

    // 2. Get basic strings
    json_t *project_name_json = json_object_get(root, "project_name");
    config->project_name = json_is_string(project_name_json) ? strdup(json_string_value(project_name_json)) : strdup("Coda Project");
    
    json_t *compiler_json = json_object_get(root, "compiler");
    config->compiler = json_is_string(compiler_json) ? strdup(json_string_value(compiler_json)) : strdup("clang");
    
    json_t *output_path_json = json_object_get(root, "output_path");
    config->output_path = json_is_string(output_path_json) ? strdup(json_string_value(output_path_json)) : strdup("dist/coda");
    
    
    // 3. Parse Source Files (MANDATORY array)
    if (parse_string_array(root, "source_files", &config->source_files) != 0) {
        json_decref(root);
        return 1;
    }

    // 4. Parse Dependencies (Special logic for key-value pairs)
    json_t *deps_obj = json_object_get(root, "dependencies");
    if (json_is_object(deps_obj)) {
        size_t count = json_object_size(deps_obj);
        config->dependencies = (const char **)malloc(sizeof(char *) * (count + 1));
        size_t i = 0;
        const char *key;
        json_t *value;
        json_object_foreach(deps_obj, key, value) {
            config->dependencies[i++] = strdup(key);
        }
        config->dependencies[count] = NULL;
    } else {
        config->dependencies = (const char **)calloc(1, sizeof(char *));
    }

    // 5. Parse NEW Optional Arrays using the helper function
    if (parse_string_array(root, "compiler_flags", &config->compiler_flags) != 0 ||
        parse_string_array(root, "linker_flags", &config->linker_flags) != 0 ||
        parse_string_array(root, "include_paths", &config->include_paths) != 0) 
    {
        json_decref(root);
        return 1;
    }

    json_decref(root);
    return 0;
}

int add_dependency_to_project_config(const char *package_name, const char *repo_url) {
    json_t *root;
    json_error_t error;

    root = json_load_file("coda.json", 0, &error);
    if (!root) {
        if (error.text[0] == '\0' || strcmp(error.text, "unspecified error") == 0) {
            root = json_object();
            json_object_set_new(root, "dependencies", json_object());
        } else {
            fprintf(stderr, "Error parsing 'coda.json': %s\n", error.text);
            return 1;
        }
    }

    json_t *deps_obj = json_object_get(root, "dependencies");
    if (!json_is_object(deps_obj)) {
        deps_obj = json_object();
        json_object_set_new(root, "dependencies", deps_obj);
    }
    
    json_object_set_new(deps_obj, package_name, json_string(repo_url));

    if (json_dump_file(root, "coda.json", JSON_INDENT(2)) != 0) {
        fprintf(stderr, "Error writing to 'coda.json'\n");
        json_decref(root);
        return 1;
    }

    json_decref(root);
    return 0;
}

void free_config(ProjectConfig *config) {
    if (!config) return;
    
    // Free string fields
    if (config->project_name) free((void*)config->project_name);
    if (config->compiler) free((void*)config->compiler);
    if (config->output_path) free((void*)config->output_path);

    // Free array fields using the helper function
    free_string_array(config->source_files);
    free_string_array(config->dependencies);
    free_string_array(config->compiler_flags);
    free_string_array(config->linker_flags);
    free_string_array(config->include_paths);
}
