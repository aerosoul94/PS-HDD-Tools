#add_definitions("-Wall" "-g")

file(GLOB SRC_FILES 
  ${PROJECT_SOURCE_DIR}/core/disk/*.cpp
  ${PROJECT_SOURCE_DIR}/core/crypto/*.cpp
  ${PROJECT_SOURCE_DIR}/core/crypto/aes/*.c
  ${PROJECT_SOURCE_DIR}/core/formats/*.cpp
  ${PROJECT_SOURCE_DIR}/core/io/data/*.cpp
  ${PROJECT_SOURCE_DIR}/core/io/stream/*.cpp)

include_directories(${PROJECT_SOURCE_DIR}/core)
add_library(core ${SRC_FILES})