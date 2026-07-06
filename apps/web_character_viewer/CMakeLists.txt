add_library(eve_web_viewer_lib STATIC src/web_character_viewer_app.cpp)
add_library(eve::web_viewer ALIAS eve_web_viewer_lib)

target_include_directories(eve_web_viewer_lib PUBLIC include)
target_link_libraries(eve_web_viewer_lib PUBLIC eve::interaction)
target_compile_features(eve_web_viewer_lib PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_web_viewer_lib)

add_executable(eve-web-character-viewer main.cpp)
target_link_libraries(eve-web-character-viewer PRIVATE eve::web_viewer)
target_compile_definitions(eve-web-character-viewer PRIVATE
    EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data"
    EVE_GAME_DIR="${CMAKE_SOURCE_DIR}/Game"
)
set_target_properties(eve-web-character-viewer PROPERTIES OUTPUT_NAME "eve-web-character-viewer")
