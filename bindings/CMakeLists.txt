add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/pybind11 build)
pybind11_add_module(disklib main.cpp)
target_link_libraries(disklib PRIVATE core)
include_directories(${PROJECT_SOURCE_DIR}/core)

set_target_properties(disklib PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin/Debug"
  LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/Release"
  LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/bin/RelWithDebInfo"
  LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_SOURCE_DIR}/bin/MinSizeRel"
)
