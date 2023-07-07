cmake_minimum_required(VERSION 3.15)
project(access-to-retro-dev LANGUAGES C VERSION 1.0 DESCRIPTION "Access to Retro Developer Library")

set(CMAKE_C_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# '#include' paths
include_directories(src)
include_directories(include)

# Include SDL2 directories
include_directories(${SDL2_INCLUDE_DIRS})

# Build documentation
option(BUILD_DOC "Build documentation" ON)
find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(access-to-retro-dev-docs ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            VERBATIM)

    message("Documentation generation target added")
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

# Get source files
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true
        src/*.c
        )

# Find SDL2 and make it required
find_package(SDL2 REQUIRED)

# Compiler Flags
if (MSVC)
    add_compile_options("/W4" "/WX")
else ()
    add_compile_options("-Wall" "-Wextra" "-Werror" "-Wundef" "-Wshadow" "-Wpointer-arith"
            "-Wcast-align" "-Wstrict-prototypes" "-Wwrite-strings" "-Wcast-qual" "-Wswitch-default"
            "-Wconversion" "-Wunreachable-code")
endif ()

# Compile as static library
add_library(access-to-retro-dev STATIC ${SOURCES})

# Link with SDL2
target_link_libraries(access-to-retro-dev ${SDL2_LIBRARIES})

# Set output file path
set_target_properties(access-to-retro-dev
        PROPERTIES
        OUTPUT_NAME "access-to-retro"
        )

# Installation target
include(GNUInstallDirs)
install(TARGETS access-to-retro-dev
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Uncomment below to enable Unit Testing
# enable_testing()
# add_subdirectory(tests)
