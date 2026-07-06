eve_add_test(eve_polish_tests test_polish_performance.cpp)

target_link_libraries(eve_polish_tests PRIVATE eve::polish eve::gameplay eve::rc1_release)
target_compile_definitions(eve_polish_tests PRIVATE
    EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data"
    EVE_GAME_DIR="${CMAKE_SOURCE_DIR}/Game"
)
