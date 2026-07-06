eve_add_test(eve_build_tests test_personal_build.cpp)

target_link_libraries(eve_build_tests PRIVATE eve::build eve::editor eve::personal_export)
target_compile_definitions(eve_build_tests PRIVATE
    EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data"
    EVE_GAME_DIR="${CMAKE_SOURCE_DIR}/Game"
)
