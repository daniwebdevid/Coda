#ifndef REGISTRY_DATA_H
#define REGISTRY_DATA_H

/**
 * @brief Structure to define a single entry in the Coda dependency registry.
 * * The 'name' is the package name used in 'coda install <name>'.
 * The 'url' is the git repository URL used for cloning.
 */
typedef struct {
    const char *name;
    const char *url;
} DependencyRegistryItem;

/**
 * @brief External declaration of the constant dependency registry array.
 * * This array is defined and populated in src/registry_data.c.
 */
extern const DependencyRegistryItem dependency_registry[];

#endif // REGISTRY_DATA_H
