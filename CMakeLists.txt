cmake_minimum_required(VERSION 3.0)
project(mann VERSION 1.1.2 LANGUAGES CXX)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

include_directories(include)
add_subdirectory(lib)
#add_subdirectory(examples)
add_subdirectory(test)
