add_library(eve_architecture_freeze_lib STATIC src/architecture_freeze_app.cpp)
add_library(eve::architecture_freeze ALIAS eve_architecture_freeze_lib)

target_include_directories(eve_architecture_freeze_lib PUBLIC include)
target_link_libraries(eve_architecture_freeze_lib PUBLIC eve::audit)
target_compile_features(eve_architecture_freeze_lib PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_architecture_freeze_lib)

add_executable(eve-architecture-freeze main.cpp)
target_link_libraries(eve-architecture-freeze PRIVATE eve::architecture_freeze)
target_compile_definitions(eve-architecture-freeze PRIVATE
    EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data"
    EVE_GAME_DIR="${CMAKE_SOURCE_DIR}/Game"
    EVE_REPO_DIR="${CMAKE_SOURCE_DIR}"
)
set_target_properties(eve-architecture-freeze PROPERTIES OUTPUT_NAME "eve-architecture-freeze")
