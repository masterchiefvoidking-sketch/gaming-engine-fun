add_library(eve_ai STATIC
    src/wardrobe.cpp
    src/relationship.cpp
    src/schedule.cpp
    src/social_ai.cpp
    src/romance_hooks.cpp
    src/relationship_stages.cpp
    src/character_sim_state.cpp
    src/boundaries.cpp
    src/mood_system.cpp
    src/dialogue_engine.cpp
    src/memory_system.cpp
    src/room_behavior.cpp
    src/outfit_reactions.cpp
    src/romance_events.cpp
    src/adaptive_schedule.cpp
    src/scene_interruption.cpp
    src/relationship_simulation.cpp
)

add_library(eve::ai ALIAS eve_ai)

target_include_directories(eve_ai
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(eve_ai
    PUBLIC
        eve::core
        eve::ecs
        eve::world
        nlohmann_json::nlohmann_json
)

target_compile_features(eve_ai PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_ai)
eve_set_compile_definitions(eve_ai)
