#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <jansson.h>
#include <limits.h>

#include "install_cmd.h"
#include "project_mgr.h"

// Mendeklarasikan variabel eksternal yang berisi data registri
extern const char *coda_registry_data;

/**
 * @brief Checks if a tool (like git) is available in the system's PATH.
 * @param tool_name The name of the tool to check.
 * @return 1 if the tool is available, 0 otherwise.
 */
static int is_tool_available(const char *tool_name) {
    char *path = getenv("PATH");
    if (path == NULL) {
        return 0;
    }
    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");
    while (dir != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, tool_name);
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return 1;
        }
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return 0;
}

/**
 * @brief Installs a package by its name, looking up the URL in the embedded registry.
 * @param package_name The name of the package to install.
 * @return 0 on success, 1 on failure.
 */
int install_dependency(const char *package_name) {
    // 1. Verifikasi ketersediaan git
    if (!is_tool_available("git")) {
        fprintf(stderr, "Error: 'git' tool not found in system PATH. Please install it.\n");
        return 1;
    }

    // 2. Muat database dari string literal
    json_t *root;
    json_error_t error;
    root = json_loads(coda_registry_data, 0, &error);

    if (!root) {
        fprintf(stderr, "Error: Internal package registry is corrupted: %s\n", error.text);
        return 1;
    }

    // 3. Cari URL paket
    json_t *packages_obj = json_object_get(root, "packages");
    json_t *package_info = json_object_get(packages_obj, package_name);
    json_t *url_json = json_object_get(package_info, "url");

    if (!json_is_string(url_json)) {
        fprintf(stderr, "Error: Package '%s' not found in the Coda registry.\n", package_name);
        json_decref(root);
        return 1;
    }

    const char *repo_url = json_string_value(url_json);
    char install_path[1024];
    snprintf(install_path, sizeof(install_path), "modules/%s", package_name);

    // 4. Tambahkan konfigurasi git untuk menangani repositori besar
    //    Ini akan membuat unduhan lebih stabil dan mencegah error timeout
    system("git config http.postBuffer 524288000");

    // 5. Jalankan git clone sebagai proses anak (child process)
    pid_t pid = fork();
    if (pid == -1) {
        perror("Failed to fork process");
        json_decref(root);
        return 1;
    }
    if (pid == 0) {
        char *args[] = { "git", "clone", (char *)repo_url, install_path, NULL };
        execvp("git", args);
        perror("Failed to run 'git clone'");
        _exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            fprintf(stdout, "Repository '%s' successfully downloaded.\n", package_name);
            
            // 6. Perbarui file coda.json proyek
            if (add_dependency_to_project_config(package_name, repo_url) != 0) {
                fprintf(stderr, "Warning: Failed to update 'coda.json' with new dependency.\n");
            }
            json_decref(root);
            return 0;
        }
    }
    json_decref(root);
    fprintf(stderr, "Error: Installation failed.\n");
    return 1;
}