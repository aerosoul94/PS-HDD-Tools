cmake_minimum_required(VERSION 3.1)
project(PSHDDTools)

# Include third party libraries
include(third_party/third_party.cmake)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
# Export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Change thsi to turn debug mode on/off
# for the whole project
set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_C_FLAGS "-g -Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3 -flto")

set(CMAKE_CXX_FLAGS "-g -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto")

# Include all the executables/libraries
add_subdirectory(core)
add_subdirectory(decrypt-tool)
add_subdirectory(hex-viewer)
add_subdirectory(mount-tool)
add_subdirectory(file-explorer)
add_subdirectory(bindings)
