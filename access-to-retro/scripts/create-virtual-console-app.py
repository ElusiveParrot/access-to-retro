"""
This script can be used to quickly create a project of a virtual console for Access to Retro Frontend

Equivalent to 'create-react-app' for React projects.
"""
import os.path
import platform


def get_os_string():
    return platform.system().lower()


# --------------------------------- C/C++ ---------------------------------
def generate_cmake_lists_content(project_name, library_file_path, library_include_path, is_cpp):
    return f"""
# Autogenerated by create-virtual-console-app
cmake_minimum_required(VERSION 3.10)
project({project_name} {"" if is_cpp else "C"})

# Set C standard
{"set(CMAKE_CXX_STANDARD 17)" if is_cpp else "set(CMAKE_C_STANDARD 11)"}

# Find SDL2 and make it required
find_package(SDL2 REQUIRED)

# '#include' paths
include_directories(src)
include_directories({library_include_path})


# Get source files, remember to remove unwanted ones
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true
{"src/*.cpp" if is_cpp else "src/*.c"}
        )

# Compile as shared (dynamic) library
add_library({project_name} SHARED ${{SOURCES}})

# Rename output library to match frontend format
set_target_properties({project_name}
        PROPERTIES
        PREFIX ""
        OUTPUT_NAME "${{PROJECT_NAME}}"
        )

# Deployment
if (APPLE)
    set(OS_SUFFIX ".darwin.vc")
elseif (WIN32)
    set(OS_SUFFIX ".windows.vc")
else ()
    set(OS_SUFFIX ".linux.vc")
endif ()

set_target_properties({project_name} PROPERTIES SUFFIX ${{OS_SUFFIX}})

# Link with Access to Retro developer library
target_link_libraries({project_name} {library_file_path})

# Link with SDL2
target_link_libraries({project_name} ${{SDL2_LIBRARIES}})
"""


def create_c_cpp_cmake_files(project_name, project_dir, library_path, library_include_path, is_cpp):
    cmake_file_content = generate_cmake_lists_content(project_name, library_path, library_include_path, is_cpp)

    cmake_lists = open(f"{project_dir}/{project_name}/CMakeLists.txt", "w")
    cmake_lists.write(cmake_file_content)
    cmake_lists.close()


def lay_out_c_cpp_arguments(arguments):
    arg_str = ""
    for argument in arguments:
        arg_str += (argument + ", ")

    # Args not empty so remove trailing ', '
    if len(arguments) != 0:
        arg_str = arg_str[:-2]

    return arg_str


def generate_c_cpp_function(annotation, return_type, name, arguments, content):
    return f"""
{annotation + " " if annotation != "" else ""}{return_type} {name}({lay_out_c_cpp_arguments(arguments)})
{{
    {content}
}}
"""


def generate_c_cpp_threads_file_content():
    return f"""
#include <access-to-retro-dev/access-to-retro-dev.h>

AR_DEFINE_REQUIRED_FN(AR_THREAD_MAIN_FN)
{{
    // Note that code below will run in an infinite loop, sleep function recommended, please see the guide for more information
}}

AR_DEFINE_REQUIRED_FN(AR_THREAD_RENDER_FN)
{{
    // Note that code below will run in an infinite loop, sleep function recommended, please see the guide for more information
}}

AR_DEFINE_REQUIRED_FN(AR_THREAD_INPUT_FN)
{{
    // Note that code below will run in an infinite loop, sleep function recommended, please see the guide for more information
}}
"""


def generate_c_cpp_config_file_content(project_name, system_name, author_name, file_extension):
    return f"""
#include <access-to-retro-dev/access-to-retro-dev.h>

// Define basic information about the emulator for the Access to Retro library (recommended to add 'constexpr' for modern C++)
const char* NAME    = "{project_name}";
const char* SYSTEM  = "{system_name}"; // Please use standard name if possible, check the guide for more information
const char* AUTHOR  = "{author_name}";
const char* ROM_EXT = "{file_extension}";

unsigned FRAME_RATE = 60;

// Recomended to be internal system resolution multiplied by an integer
unsigned DEFAULT_WINDOW_WIDTH  = 640;
unsigned DEFAULT_WINDOW_HEIGHT = 320;

// Your virtual console version
struct ar_version VERSION = {{ .major = 1, .minor = 0, .patch = 0 }};

AR_DEFINE_REQUIRED_FN(AR_DEFINE_FN)
{{
    ar_define(NAME, SYSTEM, AUTHOR, ROM_EXT, VERSION, FRAME_RATE, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    // Set your desired graphics method here
    ar_graphics_set_method(ar_graphics_method_sdl);

    return 0;
}}

AR_DEFINE_REQUIRED_FN(AR_STARTUP_FN)
{{
    ar_init();

    // Executable file to set in your emulator
    struct ar_executable* executable = ar_get_executable();

    return 0;
}}

AR_DEFINE_REQUIRED_FN(AR_QUIT_FN)
{{
    // You can add stuff here but DO NOT remove ar_quit()
    ar_quit();
}}

"""


def create_c_cpp_project_files(project_name, project_dir, is_cpp, system_name, author_name, file_extension):
    # threads.cpp
    threads_file_name = "threads.cpp" if is_cpp else "threads.c"
    threads = open(f"{project_dir}/{project_name}/src/{threads_file_name}", "w")
    threads.write(generate_c_cpp_threads_file_content())
    threads.close()

    # config.cpp
    config_file_name = "config.cpp" if is_cpp else "config.c"
    config = open(f"{project_dir}/{project_name}/src/{config_file_name}", "w")
    config.write(generate_c_cpp_config_file_content(project_name, system_name, author_name, file_extension))
    config.close()
    return


def generate_c_cpp_project(project_name, project_dir, system_name, author_name, file_extension, is_cpp):
    # Replace spaces with '-' and remove special characters
    cmake_project_name = "".join(c for c in project_name.replace(" ", "-") if c.isalnum() or c == '-')
    library_path = input("Access to Retro library path (library file): ")
    library_include_path = input("Access to Retro library include path (include folder): ")

    try:
        os.makedirs(f"{project_dir}/{cmake_project_name}")
        os.makedirs(f"{project_dir}/{cmake_project_name}/src")
    except FileExistsError:
        print("Project's directory already exists, aborting..")
        return

    create_c_cpp_cmake_files(project_name, project_dir, library_path, library_include_path, is_cpp)
    create_c_cpp_project_files(project_name, project_dir, is_cpp, system_name, author_name, file_extension)


# --------------------------------- Main ---------------------------------
def main():
    directory = input("Directory: ")
    if not os.path.exists(directory):
        print(f"Directory: '{directory}' does not exist, it will now be created.")
        os.makedirs(directory)

    project_name = input("Project name: ")

    # TODO: Validate
    system_name = input("Emulated system (follow names specified in guide): ")
    author_name = input("Author name: ")
    file_extension = input("ROM's extension (ex: '*.gb'): ")

    while True:
        programming_language = input("Programming Language (Supported: C, C++): ")
        if programming_language == "C" or "C++":
            is_cpp = programming_language == "C++"
            generate_c_cpp_project(project_name, directory, system_name, author_name, file_extension, is_cpp)
            break
        else:
            print("Invalid/unsupported programming language (Supported: C, C++).")

    print(f"Project {project_name} created.")


if __name__ == '__main__':
    main()
