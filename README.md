# Coda Build Tool

Coda is a simple build tool designed for modular C projects. It uses a "Unity Build" approach to simplify the compilation process and dependency management, eliminating the need for complex headers or Makefiles.

## Key Features

* **Simplified Modularity**: It combines multiple `.c` files into a single file for compilation, removing the need for separate `.h` files.
* **Automatic Dependency Management**: Use `coda install` to download dependencies from Git repositories and automatically update the `coda.json` file.
* **Easy Build Process**: Simply run `coda build` to compile the entire project.
* **Real-time Change Detection (Experimental)**: It monitors source files to detect changes and rebuilds the project automatically.

## System Requirements

To build and use Coda, you need:

* **Linux**: This project is designed specifically for a Linux environment.
* **Git**: Required for managing dependencies.
* **C Compiler**: `clang` or `gcc`.
* **Jansson Library**: Needed to parse JSON files.

To install dependencies on Debian/Ubuntu-based systems, use the following command:

```bash
sudo apt-get update
sudo apt-get install build-essential clang git libjansson-dev

```

## How to Build Coda

1.  **Clone the Repository**:
    
    Bash
    
    ```
    git clone [https://github.com/nama-anda/coda.git](https://github.com/nama-anda/coda.git)
    cd coda
    
    ```
    
2.  Compile from Source:
    
    Use the clang command to compile your tool:
    
    Bash
    
    ```
    clang src/main.c \
          src/registry_data.c \
          src/build_engine/build_engine.c \
          src/core_utils/core_utils.c \
          src/init_cmd/init_cmd.c \
          src/install_cmd/install_cmd.c \
          src/project_mgr/project_mgr.c \
          src/watch_cmd/watch_cmd.c \
          src/fs_monitor/fs_monitor.c \
          -o coda \
          -I./includes/ \
          -I./src/build_engine/ \
          -I./src/core_utils/ \
          -I./src/init_cmd/ \
          -I./src/install_cmd/ \
          -I./src/project_mgr/ \
          -I./src/watch_cmd/ \
          -I./src/fs_monitor/ \
          -ljansson \
          -Wall -Wextra
    
    ```
    
3.  (Optional) Move to Your PATH:
    
    To run coda from any directory, move the executable file to a directory in your PATH.
    
    Bash
    
    ```
    sudo mv coda /usr/local/bin/
    
    ```
    

## How to Use Coda

After building Coda, you can use it for your new C projects.

1.  **Initialize a New Project**:
    
    Bash
    
    ```
    coda init
    
    ```
    
    This command creates a `coda.json` file in your project directory.
    
2.  **Install Dependencies**:
    
    Bash
    
    ```
    coda install <package_name>
    
    ```
    
    Example: `coda install rapidjson`
    
3.  **Build Your Project**:
    
    Bash
    
    ```
    coda build
    
    ```
    
    This command reads `coda.json`, compiles all source files, and generates an executable in `dist/`.
    

## Contributing

This project is open-source. Contributions in the form of bug reports, pull requests, or new ideas are highly appreciated.

## License

This project is licensed under GPL - see the LICENSE file for details.
