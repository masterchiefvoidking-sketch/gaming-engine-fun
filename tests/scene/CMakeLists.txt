eve_add_test(eve_scene_tests test_scene_director.cpp)
target_link_libraries(eve_scene_tests PRIVATE eve::scene eve::movie_demo eve::ai)
target_compile_definitions(eve_scene_tests PRIVATE EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data")
