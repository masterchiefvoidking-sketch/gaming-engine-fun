eve_add_test(eve_animation_tests test_animation.cpp test_layer_stack.cpp)
target_link_libraries(eve_animation_tests PRIVATE eve::animation)
target_compile_definitions(eve_animation_tests PRIVATE EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data")
