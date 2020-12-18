include(FetchContent)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/QHexView)
include_directories(${PROJECT_SOURCE_DIR}/core)

FetchContent_Declare(
    QHexView
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/QHexView
    GIT_REPOSITORY https://github.com/Dax89/QHexView.git
    GIT_TAG        master
)

FetchContent_GetProperties(QHexView)
FetchContent_MakeAvailable(QHexView)

# For legacy QHexView
#add_library(QHexView
#    ${PROJECT_SOURCE_DIR}/QHexView/qhexedit.cpp
#    ${PROJECT_SOURCE_DIR}/QHexView/qhexedit.h
#    ${PROJECT_SOURCE_DIR}/QHexView/qhexeditcomments.cpp
#    ${PROJECT_SOURCE_DIR}/QHexView/qhexeditcomments.h
#    ${PROJECT_SOURCE_DIR}/QHexView/qhexeditdata.cpp
#    ${PROJECT_SOURCE_DIR}/QHexView/qhexeditdata.h
#    ${PROJECT_SOURCE_DIR}/QHexView/qhexeditdatadevice.cpp
#    ${PROJECT_SOURCE_DIR}/QHexView/qhexeditdatadevice.h
#    ${PROJECT_SOURCE_DIR}/QHexView/qhexeditdatareader.cpp
#    ${PROJECT_SOURCE_DIR}/QHexView/qhexeditdatareader.h
#    ${PROJECT_SOURCE_DIR}/QHexView/qhexeditdatawriter.cpp
#    ${PROJECT_SOURCE_DIR}/QHexView/qhexeditdatawriter.h
#    ${PROJECT_SOURCE_DIR}/QHexView/qhexedithighlighter.cpp
#    ${PROJECT_SOURCE_DIR}/QHexView/qhexedithighlighter.h
#    ${PROJECT_SOURCE_DIR}/QHexView/qhexeditprivate.cpp
#    ${PROJECT_SOURCE_DIR}/QHexView/qhexeditprivate.h
#    ${PROJECT_SOURCE_DIR}/QHexView/sparserangemap.cpp
#    ${PROJECT_SOURCE_DIR}/QHexView/sparserangemap.h
#)

set(SOURCES
  main.cpp
  mainwindow.cpp
  mainwindow.hpp
  searchdialog.cpp
  searchdialog.hpp
  searchthread.cpp
  searchthread.hpp
  QDiskDevice.cpp
  QDiskDevice.hpp
  QFileDiskStream.cpp
  QFileDiskStream.hpp
  QDiskBuffer.cpp
  QDiskBuffer.hpp
)

add_executable(hex-viewer ${SOURCES})

target_link_libraries(hex-viewer
  core
  qhexview-lib
  Qt5::Widgets
  Qt5::Core
  Qt5::Gui
)

set_target_properties(hex-viewer PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin/Debug"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/Release"
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/bin/RelWithDebInfo"
  RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_SOURCE_DIR}/bin/MinSizeRel"
)