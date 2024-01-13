cmake_minimum_required(VERSION 3.14)

add_executable(decrypt-tool main.cpp)

target_link_libraries(decrypt-tool core)

set_target_properties(decrypt-tool PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin/Debug"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/Release"
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/bin/RelWithDebInfo"
  RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_SOURCE_DIR}/bin/MinSizeRel"
)