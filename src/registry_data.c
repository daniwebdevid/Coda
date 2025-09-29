#include "registry_data.h"
#include <stddef.h> // For NULL

/**
 * @brief This array holds the complete registry of dependencies 
 * that can be installed using the 'coda install' command.
 * All package names in the registry are standardized to lowercase 
 * for better user experience (UX), similar to npm/pip conventions.
 * * Note: The repository URL must point to the Git repository path.
 */
const DependencyRegistryItem dependency_registry[] = {
    // -----------------------------------------------------------------------
    // 1. JSON & YAML PARSING (Now all lowercase)
    // -----------------------------------------------------------------------
    {"jansson", "https://github.com/akheron/jansson.git"},
    {"cjson", "https://github.com/DaveGamble/cJSON.git"},
    {"rapidjson", "https://github.com/Tencent/rapidjson.git"},
    {"json-c", "https://github.com/json-c/json-c.git"},
    {"libyaml", "https://github.com/yaml/libyaml.git"},

    // -----------------------------------------------------------------------
    // 2. DATA STRUCTURES & CONTAINERS
    // -----------------------------------------------------------------------
    {"uthash", "https://github.com/troydhanson/uthash.git"},
    {"sglib", "https://github.com/clibs/sglib.git"},
    {"p99", "https://github.com/P99-project/p99.git"},
    {"klib", "https://github.com/attractivechaos/klib.git"},
    {"vector.h", "https://github.com/eteran/c-vector.git"},
    
    // -----------------------------------------------------------------------
    // 3. TESTING FRAMEWORKS
    // -----------------------------------------------------------------------
    {"unity", "https://github.com/ThrowTheSwitch/Unity.git"},
    {"cgreen", "https://github.com/cgreen-dev/cgreen.git"},
    {"check", "https://github.com/libcheck/check.git"},
    {"catch2", "https://github.com/catchorg/Catch2.git"}, 
    {"doctest", "https://github.com/doctest/doctest.git"}, 
    
    // -----------------------------------------------------------------------
    // 4. NETWORKING & WEB
    // -----------------------------------------------------------------------
    {"libcurl", "https://github.com/curl/curl.git"},
    {"mongoose", "https://github.com/cesanta/mongoose.git"},
    {"libwebsockets", "https://github.com/warmcat/libwebsockets.git"},
    {"h2o", "https://github.com/h2o/h2o.git"},
    {"lwip", "https://github.com/lwip/lwip.git"},

    // -----------------------------------------------------------------------
    // 5. DATABASE & STORAGE
    // -----------------------------------------------------------------------
    {"sqlite", "https://github.com/sqlite/sqlite.git"},
    {"hiredis", "https://github.com/redis/hiredis.git"},
    {"mysql-connector-c", "https://github.com/mysql/mysql-connector-c.git"},
    {"postgresql-libpq", "https://github.com/postgres/postgres.git"},

    // -----------------------------------------------------------------------
    // 6. GRAPHICS & GUI (Core Libraries)
    // -----------------------------------------------------------------------
    {"glfw", "https://github.com/glfw/glfw.git"},
    {"sdl", "https://github.com/libsdl-org/SDL.git"},
    {"glew", "https://github.com/nigels-com/glew.git"},
    {"sokol", "https://github.com/floooh/sokol.git"},
    {"raylib", "https://github.com/raysan5/raylib.git"},
    {"nuklear", "https://github.com/Immediate-Mode-UI/Nuklear.git"},

    // -----------------------------------------------------------------------
    // 7. COMPRESSION & CRYPTO
    // -----------------------------------------------------------------------
    {"zlib", "https://github.com/madler/zlib.git"},
    {"libzip", "https://github.com/nih-at/libzip.git"},
    {"openssl", "https://github.com/openssl/openssl.git"},
    {"libsodium", "https://github.com/jedisct1/libsodium.git"},
    {"mbedtls", "https://github.com/Mbed-TLS/mbedtls.git"},

    // -----------------------------------------------------------------------
    // 8. LOGGING & UTILITIES
    // -----------------------------------------------------------------------
    {"loguru", "https://github.com/emilk/loguru.git"}, 
    {"spdlog", "https://github.com/gabime/spdlog.git"}, 
    {"argparse", "https://github.com/kazan-s/argparse.git"},
    {"docopt", "https://github.com/docopt/docopt.c.git"},
    {"inih", "https://github.com/benhoyt/inih.git"},
    
    // -----------------------------------------------------------------------
    // 9. EVENT DRIVEN & CONCURRENCY
    // -----------------------------------------------------------------------
    {"libevent", "https://github.com/libevent/libevent.git"},
    {"libuv", "https://github.com/libuv/libuv.git"},
    {"protobuf", "https://github.com/protocolbuffers/protobuf.git"},
    {"grpc", "https://github.com/grpc/grpc.git"},

    // -----------------------------------------------------------------------
    // 10. MATH & ALGEBRA
    // -----------------------------------------------------------------------
    {"blas", "https://github.com/Reference-LAPACK/blas.git"}, 
    {"cglm", "https://github.com/recp/cglm.git"}, 
    {"libomp", "https://github.com/llvm/llvm-project.git"}, 
    {"gsl", "https://github.com/ampl/gsl.git"}, 
    
    // -----------------------------------------------------------------------
    // 11. AUDIO & MULTIMEDIA
    // -----------------------------------------------------------------------
    {"portaudio", "https://github.com/PortAudio/portaudio.git"},
    {"stb", "https://github.com/nothings/stb.git"}, 

    // -----------------------------------------------------------------------
    // END OF LIST
    // -----------------------------------------------------------------------
    {NULL, NULL} // Mandatory terminator
};
