#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "build_engine.h"
#include "project_mgr.h"
#include "core_utils.h"

#define TEMP_FILE_PATH "build/temp_coda.c"

static int perform_unity_build(const char **src_files) {
    printf("[LOG] Starting Unity Build process...\n");
    FILE *temp_file = fopen(TEMP_FILE_PATH, "w");
    if (!temp_file) {
        perror("[ERROR] Failed to create temporary file");
        return 1;
    }

    // Tulis semua file sumber ke dalam satu file sementara
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

static int run_compiler(const char *compiler, const char *output_path) {
    printf("[LOG] Starting compilation...\n");
    pid_t pid = fork();
    if (pid == -1) {
        perror("[ERROR] Failed to fork process");
        return 1;
    }
    if (pid == 0) {
        char *args[] = {
            (char *)compiler,
            "-o", (char *)output_path,
            (char *)TEMP_FILE_PATH,
            "-Wall",
            "-Wextra",
            NULL
        };
        execvp(compiler, args);
        perror("[ERROR] Failed to run compiler");
        _exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("[LOG] Compiler finished with status code 0. Compilation succeeded.\n");
            return 0;
        }
    }
    printf("[ERROR] Compiler failed with status code != 0. Compilation failed.\n");
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
    
    if (run_compiler(config.compiler, config.output_path) != 0) {
        fprintf(stderr, "[ERROR] Compilation failed.\n");
        free_config(&config);
        return 1;
    }

    printf("[LOG] Build process completed successfully.\n");
    printf("Build succeeded! Executable: %s\n", config.output_path);
    free_config(&config);
    return 0;
}