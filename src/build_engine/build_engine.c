#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "build_engine.h"
#include "project_mgr.h"
#include "core_utils.h" // Assuming core_utils.h has read_file_to_string

#define TEMP_FILE_PATH "build/temp_coda.c"

// Helper function to count elements in a NULL-terminated array
static int count_array_elements(const char **arr) {
    if (!arr) return 0;
    int count = 0;
    while (arr[count] != NULL) {
        count++;
    }
    return count;
}

// Helper function to copy elements from a source array to a target array
static char **copy_array_elements(char **target, const char **source, int *current_index, const char *prefix) {
    if (!source) return target;
    for (int i = 0; source[i] != NULL; ++i) {
        // If prefix is provided (like '-I' for include paths)
        if (prefix) {
            size_t len = strlen(prefix) + strlen(source[i]) + 1;
            char *prefixed_arg = (char *)malloc(len);
            if (!prefixed_arg) {
                perror("malloc");
                return NULL;
            }
            snprintf(prefixed_arg, len, "%s%s", prefix, source[i]);
            target[*current_index] = prefixed_arg;
        } else {
            // Just copy the string (already const char* which execvp expects as char*)
            target[*current_index] = (char *)source[i];
        }
        (*current_index)++;
    }
    return target;
}


static int perform_unity_build(const char **src_files) {
    printf("[LOG] Starting Unity Build process...\n");
    FILE *temp_file = fopen(TEMP_FILE_PATH, "w");
    if (!temp_file) {
        perror("[ERROR] Failed to create temporary file");
        return 1;
    }

    // Write all source files into the temporary file
    for (int i = 0; src_files[i] != NULL; ++i) {
        printf("[LOG] Reading file: %s\n", src_files[i]);
        char *file_content = read_file_to_string(src_files[i]);
        if (!file_content) {
            fprintf(stderr, "[ERROR] Failed to read file: %s\n", src_files[i]);
            fclose(temp_file);
            return 1;
        }

        fprintf(temp_file, "// File: %s\n", src_files[i]);
        fprintf(temp_file, "%s\n\n", file_content);
        free(file_content);
        printf("[LOG] Successfully appended %s to temp file.\n", src_files[i]);
    }
    
    fclose(temp_file);
    printf("[LOG] Unity Build process completed. All source files are in %s.\n", TEMP_FILE_PATH);
    return 0;
}

static int run_compiler(const ProjectConfig *config) {
    printf("[LOG] Starting compilation...\n");
    
    // 1. Calculate the total number of arguments needed
    // 5 mandatory args: compiler, -o, output_path, temp_file_path, NULL terminator
    // 2 args for mandatory flags: -Wall, -Wextra
    int total_args = 5 + 2; 

    // Add optional arrays
    total_args += count_array_elements(config->compiler_flags);
    total_args += count_array_elements(config->linker_flags);
    total_args += count_array_elements(config->include_paths); // These will be prefixed with -I

    // 2. Allocate memory for the argument vector (argv)
    char **argv = (char **)malloc(sizeof(char *) * total_args);
    if (!argv) {
        perror("[ERROR] Failed to allocate memory for compiler arguments");
        return 1;
    }

    int current_index = 0;

    // 3. Populate argv with mandatory arguments
    argv[current_index++] = (char *)config->compiler; // argv[0] is the compiler name
    argv[current_index++] = "-o";
    argv[current_index++] = (char *)config->output_path;
    argv[current_index++] = TEMP_FILE_PATH;
    
    // Add mandatory flags
    argv[current_index++] = "-Wall";
    argv[current_index++] = "-Wextra";


    // 4. Populate argv with custom flags from config (Compiler Flags & Linker Flags)
    // Linker flags must come last, but since we are using unity build, it doesn't matter much.
    // However, it's good practice to append all needed flags before the final NULL.

    // Copy compiler_flags
    if (!copy_array_elements(argv, config->compiler_flags, &current_index, NULL)) return 1;

    // Copy linker_flags
    if (!copy_array_elements(argv, config->linker_flags, &current_index, NULL)) return 1;

    // Copy include_paths (must be prefixed with -I)
    if (!copy_array_elements(argv, config->include_paths, &current_index, "-I")) return 1;

    // 5. Add NULL terminator at the very end
    argv[current_index] = NULL;
    
    printf("[LOG] Compiler arguments prepared. Executing: %s ...\n", config->compiler);

    // 6. Execute the compiler (fork/execvp)
    pid_t pid = fork();
    if (pid == -1) {
        perror("[ERROR] Failed to fork process");
        free(argv); // Clean up the argv array
        return 1;
    }
    if (pid == 0) {
        // Child process: execute the compiler
        execvp(config->compiler, argv);
        perror("[ERROR] Failed to run compiler");
        _exit(1);
    } else {
        // Parent process: wait for the compiler to finish
        int status;
        waitpid(pid, &status, 0);
        
        // Clean up memory allocated for the arguments
        // IMPORTANT: We only free strings that were allocated inside copy_array_elements (those with prefixes, i.e., include_paths)
        for (int i = 0; config->include_paths && config->include_paths[i] != NULL; i++) {
            // Include paths are the only prefixed ones that need freeing here
            if (i + 4 + 2 + count_array_elements(config->compiler_flags) + count_array_elements(config->linker_flags) < current_index) {
                free(argv[i + 4 + 2 + count_array_elements(config->compiler_flags) + count_array_elements(config->linker_flags)]);
            }
        }
        free(argv); 

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("[LOG] Compiler finished with status code 0. Compilation succeeded.\n");
            return 0;
        }
    }
    printf("[ERROR] Compiler failed. Check the errors above.\n");
    return 1;
}

int build_project(const char *config_path) {
    printf("[LOG] Starting build_project function...\n");
    ProjectConfig config;
    if (parse_config_from_file(config_path, &config) != 0) {
        fprintf(stderr, "[ERROR] Failed to parse configuration from %s.\n", config_path);
        return 1;
    }
    printf("[LOG] Configuration parsed successfully.\n");
    
    if (perform_unity_build(config.source_files) != 0) {
        fprintf(stderr, "[ERROR] Unity build failed.\n");
        free_config(&config);
        return 1;
    }
    
    // Pass the entire config structure to the compiler runner
    if (run_compiler(&config) != 0) { 
        fprintf(stderr, "[ERROR] Compilation failed.\n");
        free_config(&config);
        return 1;
    }

    printf("[LOG] Build process completed successfully.\n");
    printf("Build succeeded! Executable: %s\n", config.output_path);
    free_config(&config);
    return 0;
}