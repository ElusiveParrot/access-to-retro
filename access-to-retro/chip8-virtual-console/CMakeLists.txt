cmake_minimum_required(VERSION 3.15)
project(access-to-retro-chip8 VERSION 1.0 DESCRIPTION "CHIP8 Emulator Virtual Console")

set(CMAKE_CXX_STANDARD 20)

# '#include' paths
include_directories(src)
include_directories(../developer-library/include)

# Get source files
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true
        *.hpp *.cpp *.h
        )

# Build documentation
option(BUILD_DOC "Build documentation" ON)
find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(access-to-retro-chip8-docs ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            VERBATIM)

    message("Documentation generation target added")
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

# Compiler Flags
if (MSVC)
    add_compile_options("/W4" "/WX")
else ()
    add_compile_options("-Wall" "-Wextra" "-Werror" "-Wundef" "-Wshadow" "-Wpointer-arith"
            "-Wcast-align" "-Wstrict-prototypes" "-Wwrite-strings" "-Wcast-qual" "-Wswitch-default"
            "-Wconversion" "-Wunreachable-code")
endif ()

# Compile as shared (dynamic) library
add_library(access-to-retro-chip8 SHARED ${SOURCES})

# Rename output library to match frontend format
set_target_properties(access-to-retro-chip8
        PROPERTIES
        PREFIX ""
        OUTPUT_NAME "${PROJECT_NAME}"
        )

# Deployment
if (APPLE)
    set(OS_SUFFIX ".darwin.vc")
elseif (WIN32)
    set(OS_SUFFIX ".windows.vc")
else ()
    set(OS_SUFFIX ".linux.vc")
endif ()

set_target_properties(access-to-retro-chip8 PROPERTIES SUFFIX ${OS_SUFFIX})

# Link with Access to Retro developer library
target_link_libraries(access-to-retro-chip8 access-to-retro-dev)
