eve_add_test(eve_ai_tests
    test_relationship.cpp
    test_schedule.cpp
    test_social_ai.cpp
    test_wardrobe.cpp
    test_romance_hooks.cpp
)

target_link_libraries(eve_ai_tests PRIVATE eve::ai)
target_compile_definitions(eve_ai_tests PRIVATE EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data")
