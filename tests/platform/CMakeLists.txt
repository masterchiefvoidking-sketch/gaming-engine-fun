eve_add_test(eve_platform_tests test_engine_direction.cpp)

target_link_libraries(eve_platform_tests PRIVATE eve::platform)
target_compile_definitions(eve_platform_tests PRIVATE
    EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data"
    EVE_GAME_DIR="${CMAKE_SOURCE_DIR}/Game"
)
