include(FetchContent)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/core)

set(SOURCES
  main.cpp
  mainwindow.cpp
  mainwindow.hpp
  #QDiskDevice.cpp
  #QDiskDevice.hpp
  QFileDiskStream.cpp
  QFileDiskStream.hpp
  #QDiskBuffer.cpp
  #QDiskBuffer.hpp
)

add_executable(file-explorer ${SOURCES})

target_link_libraries(file-explorer
  core
  Qt5::Widgets
  Qt5::Core
  Qt5::Gui
)

set_target_properties(file-explorer PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin/Debug"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/Release"
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/bin/RelWithDebInfo"
  RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_SOURCE_DIR}/bin/MinSizeRel"
)