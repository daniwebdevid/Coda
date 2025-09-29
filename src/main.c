#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Include headers for all command functionalities
// Note: We are using init_project_config() instead of init_project() 
// due to the function naming in the implementation file.
#include "init_cmd.h" 
#include "build_engine.h"
#include "install_cmd.h"
#include "watch_cmd.h"

/**
 * @brief Prints the tool's usage instructions to stderr.
 */
static void print_usage() {
    fprintf(stderr, "Usage: coda <command> [arguments]\n");
    fprintf(stderr, "Commands:\n");
    fprintf(stderr, "  init             Initializes a new Coda project.\n");
    fprintf(stderr, "  build            Reads the project config and compiles.\n");
    fprintf(stderr, "  install <package_name> Downloads a dependency from the package registry.\n");
    fprintf(stderr, "  watch            Monitors source files and rebuilds automatically.\n");
}

int main(int argc, char *argv[]) {
    // Ensure at least one command-line argument is provided.
    if (argc < 2) {
        print_usage();
        return 1;
    }

    const char *command = argv[1];

    // The main command router
    if (strcmp(command, "init") == 0) {
        if (argc != 2) {
            fprintf(stderr, "Error: 'init' command takes no arguments.\n");
            print_usage();
            return 1;
        }
        // FIX: Changed init_project() to the correct name, init_project_config()
        return init_project_config();
    } else if (strcmp(command, "build") == 0) {
        if (argc != 2) {
            fprintf(stderr, "Error: 'build' command takes no arguments.\n");
            print_usage();
            return 1;
        }
        return build_project("coda.json");
    } else if (strcmp(command, "install") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Error: 'install' command requires one argument: <package_name>.\n");
            print_usage();
            return 1;
        }
        return install_dependency(argv[2]);
    } else if (strcmp(command, "watch") == 0) {
        if (argc != 2) {
            fprintf(stderr, "Error: 'watch' command takes no arguments.\n");
            print_usage();
            return 1;
        }
        return watch_project("coda.json");
    } else {
        fprintf(stderr, "Error: Invalid command '%s'.\n", command);
        print_usage();
        return 1;
    }

    return 0;
}
