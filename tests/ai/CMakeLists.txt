eve_add_test(eve_ai_tests
    test_relationship.cpp
    test_schedule.cpp
    test_social_ai.cpp
    test_wardrobe.cpp
)

target_link_libraries(eve_ai_tests PRIVATE eve::ai)
