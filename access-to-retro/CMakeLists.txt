cmake_minimum_required(VERSION 3.15)
project(access-to-retro VERSION 1.0 DESCRIPTION "Access to Retro Project")

# Compile Developer Library first
add_subdirectory(developer-library)

# Then virtual console as it depends on the library
add_subdirectory(chip8-virtual-console)

# And lastly the frontend
add_subdirectory(frontend)
