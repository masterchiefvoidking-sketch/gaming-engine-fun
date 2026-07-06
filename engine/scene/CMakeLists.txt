add_library(eve_scene STATIC
    src/scene_types.cpp
    src/scene_state_machine.cpp
    src/scene_conditions.cpp
    src/emotional_outcomes.cpp
    src/camera_shot_sequencer.cpp
    src/animation_cue_system.cpp
    src/lighting_cue_system.cpp
    src/music_cue_system.cpp
    src/dialogue_scene_sequencer.cpp
    src/room_event_triggers.cpp
    src/event_scheduler.cpp
    src/scene_director.cpp
)

add_library(eve::scene ALIAS eve_scene)

target_include_directories(eve_scene
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(eve_scene
    PUBLIC
        eve::core
        eve::ai
        eve::render
        eve::animation
        eve::human
        eve::world
        nlohmann_json::nlohmann_json
)

target_compile_features(eve_scene PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_scene)
eve_set_compile_definitions(eve_scene)
