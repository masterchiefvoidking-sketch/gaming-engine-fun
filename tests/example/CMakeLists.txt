eve_add_test(eve_example_scene_tests
    test_apartment_character_scene.cpp
    test_wardrobe_vertical_slice.cpp
    test_cozy_wardrobe_demo.cpp
)
target_link_libraries(eve_example_scene_tests PRIVATE eve::example eve::wardrobe_slice eve::anime_demo)
target_compile_definitions(eve_example_scene_tests PRIVATE EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data")
