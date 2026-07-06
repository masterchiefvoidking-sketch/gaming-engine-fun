eve_add_test(eve_gameplay_tests test_apartment_life_game.cpp)

target_link_libraries(eve_gameplay_tests PRIVATE eve::gameplay eve::apartment_demo)
target_compile_definitions(eve_gameplay_tests PRIVATE
    EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data"
    EVE_GAME_DIR="${CMAKE_SOURCE_DIR}/Game"
)
