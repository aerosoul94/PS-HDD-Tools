cmake_minimum_required(VERSION 3.1)
project(PSHDDTools)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(core)
add_subdirectory(decrypt-tool)
add_subdirectory(hex-viewer)
add_subdirectory(mount-tool)
add_subdirectory(file-explorer)
add_subdirectory(bindings)
