
add_executable(examples_ann_sample ann_sample.cpp)
target_link_libraries(examples_ann_sample mann_lib)
set_target_properties(examples_ann_sample PROPERTIES OUTPUT_NAME sample)
