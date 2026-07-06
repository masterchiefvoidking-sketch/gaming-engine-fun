eve_add_test(eve_content_tests test_content_database.cpp)
target_link_libraries(eve_content_tests PRIVATE eve::content)
target_compile_definitions(eve_content_tests PRIVATE
    EVE_GAME_DIR="${CMAKE_SOURCE_DIR}/Game"
)
