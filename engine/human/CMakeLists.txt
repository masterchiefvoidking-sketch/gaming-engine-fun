add_library(eve_human STATIC
    src/character_creator.cpp
    src/character_interaction.cpp
    src/character_interaction_system.cpp
    src/character_framework.cpp
    src/photo_mode_bridge.cpp
    src/body_regions.cpp
    src/interaction_rules.cpp
    src/body_interaction_system.cpp
    src/wardrobe_system.cpp
    src/dressing_room.cpp
)

add_library(eve::human ALIAS eve_human)

target_include_directories(eve_human
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(eve_human
    PUBLIC
        eve::core
        eve::ecs
        eve::render
        eve::animation
        nlohmann_json::nlohmann_json
)

target_compile_features(eve_human PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_human)
eve_set_compile_definitions(eve_human)
