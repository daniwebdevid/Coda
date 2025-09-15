#include "core_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_file_to_string(const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("Failed to open file");
        return NULL;
    }

    // Determine file size
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Allocate buffer for content + null terminator
    char *buffer = malloc(file_size + 1);
    if (!buffer) {
        perror("Failed to allocate memory for file content");
        fclose(fp);
        return NULL;
    }

    // Read file into buffer and null-terminate
    size_t read_size = fread(buffer, 1, file_size, fp);
    buffer[read_size] = '\0';

    fclose(fp);
    return buffer;
}