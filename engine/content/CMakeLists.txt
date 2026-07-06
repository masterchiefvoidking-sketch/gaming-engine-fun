add_library(eve_content STATIC
    src/content_types.cpp
    src/content_database.cpp
    src/project_layout.cpp
    src/mod_system.cpp
    src/asset_import.cpp
    src/character_content.cpp
    src/wardrobe_database.cpp
    src/apartment_content.cpp
    src/dialogue_content.cpp
    src/undo_stack.cpp
    src/localization.cpp
    src/packaging.cpp
    src/hot_reload.cpp
    src/game_content_project.cpp
)

add_library(eve::content ALIAS eve_content)

target_include_directories(eve_content
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(eve_content
    PUBLIC
        eve::core
        eve::ai
        nlohmann_json::nlohmann_json
)

target_compile_features(eve_content PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_content)
eve_set_compile_definitions(eve_content)
