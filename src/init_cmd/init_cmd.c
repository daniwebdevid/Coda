#include "init_cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

/**
 * @brief Creates the necessary directories (dist/, build/, src/, modules/) for the project.
 * @return 0 on success, 1 on failure.
 */
static int create_project_directories() {
    // Create 'dist' directory for the final executable
    if (mkdir("dist", 0755) == -1 && errno != EEXIST) {
        perror("Error creating dist directory");
        return 1;
    }
    // Create 'build' directory for temporary files
    if (mkdir("build", 0755) == -1 && errno != EEXIST) {
        perror("Error creating build directory");
        return 1;
    }
    // Create 'src' directory for user source files
    if (mkdir("src", 0755) == -1 && errno != EEXIST) {
        perror("Error creating src directory");
        return 1;
    }
    // Create 'modules' directory for external dependencies
    if (mkdir("modules", 0755) == -1 && errno != EEXIST) {
        perror("Error creating modules directory");
        return 1;
    }
    return 0;
}

/**
 * @brief Creates a minimal initial source file (main.c).
 * @return 0 on success, 1 on failure.
 */
static int create_initial_source_file() {
    FILE *fp = fopen("src/main.c", "w");
    if (!fp) {
        perror("Error creating src/main.c");
        return 1;
    }
    
    // Write minimal C code with guidance
    fprintf(fp, "// Main entry point of your Coda project.\n");
    fprintf(fp, "// Add your modules to the 'source_files' array in coda.json and use 'extern' to call functions.\n\n");
    fprintf(fp, "#include <stdio.h>\n\n");
    fprintf(fp, "int main() {\n");
    fprintf(fp, "    printf(\"Project initialized successfully by Coda.\\n\");\n");
    fprintf(fp, "    return 0;\n");
    fprintf(fp, "}\n");
    
    fclose(fp);
    return 0;
}


/**
 * @brief Creates the initial coda.json configuration file using snake_case keys.
 * @return 0 on success, 1 on failure.
 */
static int create_initial_config_file() {
    // 1. Create the root JSON object
    json_t *root = json_object();

    // 2. Set core project keys (using snake_case: project_name, output_path, etc.)
    json_object_set_new(root, "project_name", json_string("NewCodaProject"));
    json_object_set_new(root, "compiler", json_string("clang"));
    json_object_set_new(root, "output_path", json_string("dist/app_name"));

    // 3. Create and set 'source_files' array (snake_case)
    json_t *source_files_array = json_array();
    json_array_append_new(source_files_array, json_string("src/main.c"));
    json_object_set_new(root, "source_files", source_files_array); 

    // 4. Create and set empty optional configuration arrays/objects
    json_object_set_new(root, "dependencies", json_object());
    json_object_set_new(root, "compiler_flags", json_array());
    json_object_set_new(root, "linker_flags", json_array());
    json_object_set_new(root, "include_paths", json_array());


    // 5. Write the JSON structure to the file
    if (json_dump_file(root, "coda.json", JSON_INDENT(2)) != 0) {
        fprintf(stderr, "Error writing initial coda.json file.\n");
        json_decref(root);
        return 1;
    }

    // 6. Clean up
    json_decref(root);
    return 0;
}


/**
 * @brief Initializes a new Coda project in the current directory.
 * @return 0 on success, 1 on failure.
 */
int init_project_config() {
    printf("[INIT] Starting project initialization...\n");

    if (create_project_directories() != 0) {
        fprintf(stderr, "[ERROR] Directory creation failed.\n");
        return 1;
    }
    printf("[INIT] Project structure (src/, dist/, build/, modules/) created.\n");

    if (create_initial_source_file() != 0) {
        fprintf(stderr, "[ERROR] Failed to create initial source file (src/main.c).\n");
        return 1;
    }
    printf("[INIT] Initial source file (src/main.c) created.\n");

    if (create_initial_config_file() != 0) {
        fprintf(stderr, "[ERROR] Failed to create configuration file (coda.json).\n");
        return 1;
    }
    printf("[INIT] Configuration file (coda.json) created successfully.\n");
    printf("\n[SUCCESS] Coda project is ready. Run 'coda build' to compile.\n");

    return 0;
}
