#include "project_mgr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

int parse_config_from_file(const char *path, ProjectConfig *config) {
    json_t *root;
    json_error_t error;

    // Load the JSON configuration file
    root = json_load_file(path, 0, &error);
    if (!root) {
        fprintf(stderr, "Error parsing JSON file '%s': on line %d: %s\n", path, error.line, error.text);
        return 1;
    }

    // Get project name
    json_t *project_name_json = json_object_get(root, "project_name");
    if (json_is_string(project_name_json)) {
        config->project_name = strdup(json_string_value(project_name_json));
    } else {
        config->project_name = strdup("Coda Project");
    }

    // Get compiler
    json_t *compiler_json = json_object_get(root, "compiler");
    if (json_is_string(compiler_json)) {
        config->compiler = strdup(json_string_value(compiler_json));
    } else {
        config->compiler = strdup("clang");
    }

    // Get output path
    json_t *output_path_json = json_object_get(root, "output_path");
    if (json_is_string(output_path_json)) {
        config->output_path = strdup(json_string_value(output_path_json));
    } else {
        config->output_path = strdup("dist/coda");
    }

    // Get source files
    json_t *src_files_json = json_object_get(root, "source_files");
    if (json_is_array(src_files_json)) {
        size_t count = json_array_size(src_files_json);
        config->source_files = (const char **)malloc(sizeof(char *) * (count + 1));
        for (size_t i = 0; i < count; i++) {
            json_t *file_json = json_array_get(src_files_json, i);
            if (json_is_string(file_json)) {
                config->source_files[i] = strdup(json_string_value(file_json));
            }
        }
        config->source_files[count] = NULL;
    } else {
        fprintf(stderr, "Error: 'source_files' must be a JSON array.\n");
        json_decref(root);
        return 1;
    }

    // Get dependencies (to be used later)
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
        config->dependencies = NULL;
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
    if (config->project_name) free(config->project_name);
    if (config->compiler) free(config->compiler);
    if (config->output_path) free(config->output_path);

    if (config->source_files) {
        for (int i = 0; config->source_files[i] != NULL; i++) {
            free((void*)config->source_files[i]);
        }
        free(config->source_files);
    }
    if (config->dependencies) {
        for (int i = 0; config->dependencies[i] != NULL; i++) {
            free((void*)config->dependencies[i]);
        }
        free(config->dependencies);
    }
}