eve_add_test(eve_interaction_tests test_web_mobile_interaction.cpp)

target_link_libraries(eve_interaction_tests PRIVATE eve::interaction eve::web_viewer)
target_compile_definitions(eve_interaction_tests PRIVATE
    EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data"
    EVE_GAME_DIR="${CMAKE_SOURCE_DIR}/Game"
)
