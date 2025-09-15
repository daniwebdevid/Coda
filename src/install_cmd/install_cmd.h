#ifndef INSTALL_CMD_H
#define INSTALL_CMD_H

/**
 * @brief Installs a package by its name, looking up the URL in an internal registry.
 * @param package_name The name of the package to install.
 * @return 0 on success, 1 on failure.
 */
int install_dependency(const char *package_name);

#endif // INSTALL_CMD_H