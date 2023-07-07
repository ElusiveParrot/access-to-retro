cmake_minimum_required(VERSION 3.15)
project(access-to-retro-frontend VERSION 1.0 DESCRIPTION "Access to Retro Frontend Application")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Qt's autos (generating C++ files from UI files)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Compiler flags
if (MSVC)
    add_compile_options("/W4" "/WX")
else ()
    add_compile_options("-Wall" "-Wextra" "-Werror" "-Wundef" "-Wshadow" "-Wpointer-arith"
            "-Wcast-align" "-Wstrict-prototypes" "-Wwrite-strings" "-Wcast-qual" "-Wswitch-default"
            "-Wconversion" "-Wunreachable-code")
endif ()

# Recursively search for source files
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true
        *.hpp *.cpp *.ui *.h
        )

# Build documentation
option(BUILD_DOC "Build documentation" ON)
find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(access-to-retro-frontend-docs ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            VERBATIM)

    message("Documentation generation target added")
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

# Make all Qt libraries required
find_package(
        Qt6 COMPONENTS
        Core Gui Widgets OpenGLWidgets
        REQUIRED
)

# Find SDL2 and make it required
find_package(SDL2 REQUIRED)

# '#include' paths
include_directories(src)
include_directories(../developer-library/include)

# Include SDL2 directories
include_directories(${SDL2_INCLUDE_DIRS})

# Add Qt's resource file
qt6_add_resources(RESOURCES resources/resources.qrc)

# Deployment
if (APPLE)
    # Set .app file icon
    set(MACOSX_BUNDLE_ICON_FILE appicon_macos.icns)
    set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/resources/appicon_macos.icns")
    set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    # Create executable as .app file
    add_executable(access-to-retro-frontend MACOSX_BUNDLE ${SOURCES} ${RESOURCES} ${app_icon_macos})
elseif (WIN32)
    # Compile windows resources
    set(WIN_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources/windows_app.rc)
    enable_language("RC")

    # Create executable as .exe file
    add_executable(access-to-retro-frontend ${SOURCES} ${WIN_RESOURCES} ${RESOURCES})
else ()
    # TODO: Linux icon support
    add_executable(access-to-retro-frontend ${SOURCES} ${RESOURCES})
endif ()

# Change executable name to something more user friendly, no need to add 'frontend' as it's the only user facing product
set_target_properties(access-to-retro-frontend PROPERTIES OUTPUT_NAME "AccessToRetro")

# Link with Qt
target_link_libraries(access-to-retro-frontend
        Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGLWidgets
        )

# Link with SDL2
target_link_libraries(access-to-retro-frontend ${SDL2_LIBRARIES})
