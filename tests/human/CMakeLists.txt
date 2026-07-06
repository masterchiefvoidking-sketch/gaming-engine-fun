eve_add_test(eve_human_tests
    test_character_creator.cpp
    test_character_interaction.cpp
    test_character_framework.cpp
    test_body_wardrobe.cpp
    test_wardrobe_system.cpp
    test_anime_expression.cpp
    test_anime_character_style.cpp
    test_anime_visual_pipeline.cpp
)

target_link_libraries(eve_human_tests PRIVATE eve::human)
target_compile_definitions(eve_human_tests PRIVATE EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data")
