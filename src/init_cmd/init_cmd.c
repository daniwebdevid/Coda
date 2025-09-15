#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "init_cmd.h"

// List of directories to be created for a new project.
static const char *project_dirs[] = {
    "src", "dist", "build", "lib", "modules", NULL
};

/**
 * @brief Creates a single directory with a default permission.
 */
static int create_directory(const char *path) {
    if (mkdir(path, 0755) != 0) {
        if (errno == EEXIST) {
            fprintf(stdout, "Directory '%s' already exists, skipping.\n", path);
            return 0;
        }
        perror("Failed to create directory");
        return 1;
    }
    fprintf(stdout, "Directory '%s' created.\n", path);
    return 0;
}

/**
 * @brief Creates a boilerplate coda.json file.
 */
static int create_coda_config() {
    FILE *fp = fopen("coda.json", "w");
    if (!fp) {
        perror("Failed to create coda.json file");
        return 1;
    }
    fprintf(fp, "{\n");
    fprintf(fp, "  \"projectName\": \"MyProject\",\n");
    fprintf(fp, "  \"sourceFiles\": [\"src/main.c\"],\n");
    fprintf(fp, "  \"dependencies\": {}\n");
    fprintf(fp, "}\n");
    fclose(fp);
    fprintf(stdout, "File 'coda.json' created.\n");
    return 0;
}

/**
 * @brief Creates a basic main.c file for the user to get started.
 */
static int create_main_c_file() {
    FILE *fp = fopen("src/main.c", "w");
    if (!fp) {
        perror("Failed to create src/main.c file");
        return 1;
    }
    fprintf(fp, "#include <stdio.h>\n\n");
    fprintf(fp, "int main() {\n");
    fprintf(fp, "    printf(\"Hello, Coda!\\n\");\n");
    fprintf(fp, "    return 0;\n");
    fprintf(fp, "}\n");
    fclose(fp);
    fprintf(stdout, "File 'src/main.c' created.\n");
    return 0;
}

int init_project() {
    for (int i = 0; project_dirs[i] != NULL; ++i) {
        if (create_directory(project_dirs[i]) != 0) {
            return 1;
        }
    }
    if (create_coda_config() != 0) {
        return 1;
    }
    if (create_main_c_file() != 0) {
        return 1;
    }
    fprintf(stdout, "Coda project successfully initialized!\n");
    return 0;
}