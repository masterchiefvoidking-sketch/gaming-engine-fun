add_library(eve_platform STATIC
    src/engine_mission.cpp
    src/unified_save.cpp
    src/web_publisher.cpp
    src/mobile_input.cpp
    src/backup_manager.cpp
    src/project_templates.cpp
    src/local_asset_library.cpp
    src/game_session.cpp
)

add_library(eve::platform ALIAS eve_platform)

target_include_directories(eve_platform
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(eve_platform
    PUBLIC
        eve::core
        eve::ai
        eve::content
        nlohmann_json::nlohmann_json
)

target_compile_features(eve_platform PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_platform)
eve_set_compile_definitions(eve_platform)
