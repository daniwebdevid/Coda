#ifndef CORE_UTILS_H
#define CORE_UTILS_H

/**
 * @brief Reads the entire content of a file into a dynamically allocated string.
 * @param path The path to the file.
 * @return A null-terminated string containing the file's content, or NULL on failure.
 * The caller is responsible for freeing the returned string.
 */
char *read_file_to_string(const char *path);

#endif // CORE_UTILS_H