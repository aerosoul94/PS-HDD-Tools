cmake_minimum_required(VERSION 3.14)

file(GLOB SRC_FILES 
  */*.cpp
  */*/*.cpp
  */*/*.c
)

add_library(core ${SRC_FILES})
target_include_directories(core PUBLIC .)
target_link_libraries(core PUBLIC ${CMAKE_DL_LIBS} m)

set_property(TARGET core PROPERTY POSITION_INDEPENDENT_CODE ON)
