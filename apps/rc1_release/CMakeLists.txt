add_library(eve_rc1_release_lib STATIC src/rc1_release_app.cpp)
add_library(eve::rc1_release ALIAS eve_rc1_release_lib)

target_include_directories(eve_rc1_release_lib PUBLIC include)
target_link_libraries(eve_rc1_release_lib PUBLIC eve::polish eve::gameplay)
target_compile_features(eve_rc1_release_lib PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_rc1_release_lib)

add_executable(eve-rc1-release main.cpp)
target_link_libraries(eve-rc1-release PRIVATE eve::rc1_release)
target_compile_definitions(eve-rc1-release PRIVATE
    EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data"
    EVE_GAME_DIR="${CMAKE_SOURCE_DIR}/Game"
)
set_target_properties(eve-rc1-release PROPERTIES OUTPUT_NAME "eve-rc1-release")
