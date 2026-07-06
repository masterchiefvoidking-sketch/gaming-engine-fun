add_library(eve_interaction STATIC
    src/input_controller.cpp
    src/camera_input_bridge.cpp
    src/character_inspection_mode.cpp
    src/interaction_ring.cpp
    src/quality_settings.cpp
    src/web_performance_profile.cpp
    src/responsive_ui_layout.cpp
    src/browser_storage.cpp
    src/player_ui_state.cpp
    src/photo_mode_controller.cpp
    src/web_character_viewer.cpp
)

add_library(eve::interaction ALIAS eve_interaction)

target_include_directories(eve_interaction
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(eve_interaction
    PUBLIC
        eve::core
        eve::render
        eve::human
        eve::world
        eve::platform
        eve::ai
        nlohmann_json::nlohmann_json
)

target_compile_features(eve_interaction PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_interaction)
eve_set_compile_definitions(eve_interaction)
