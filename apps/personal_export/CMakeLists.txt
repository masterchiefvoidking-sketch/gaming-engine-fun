add_library(eve_personal_export_lib STATIC src/personal_export_demo.cpp)
add_library(eve::personal_export ALIAS eve_personal_export_lib)

target_include_directories(eve_personal_export_lib PUBLIC include)
target_link_libraries(eve_personal_export_lib PUBLIC eve::build eve::editor)
target_compile_features(eve_personal_export_lib PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_personal_export_lib)

add_executable(eve-personal-export main.cpp)
target_link_libraries(eve-personal-export PRIVATE eve::personal_export)
target_compile_definitions(eve-personal-export PRIVATE
    EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data"
    EVE_GAME_DIR="${CMAKE_SOURCE_DIR}/Game"
)
set_target_properties(eve-personal-export PROPERTIES OUTPUT_NAME "eve-personal-export")
