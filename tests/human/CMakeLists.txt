eve_add_test(eve_human_tests
    test_character_creator.cpp
    test_character_interaction.cpp
    test_character_framework.cpp
)

target_link_libraries(eve_human_tests PRIVATE eve::human)
