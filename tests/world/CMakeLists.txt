eve_add_test(eve_world_tests
    test_interactions.cpp
    test_apartment.cpp
    test_simulation.cpp
    test_persistence.cpp
    test_anime_room_lighting.cpp
)

target_link_libraries(eve_world_tests PRIVATE eve::world)
target_compile_definitions(eve_world_tests PRIVATE EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data")
