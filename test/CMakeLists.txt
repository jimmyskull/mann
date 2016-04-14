
find_package(Catch)
include_directories(${Catch_INCLUDE_DIRS})

add_executable(mann_lib_test_suite
  test_suite.cpp
  point.cpp
  kdtree.cpp
  box.cpp)

#target_link_libraries(mann_lib_test_suite)
set_target_properties(mann_lib_test_suite PROPERTIES OUTPUT_NAME test)
