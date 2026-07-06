add_library(eve_example_lib STATIC src/apartment_character_scene.cpp)
add_library(eve::example ALIAS eve_example_lib)

target_include_directories(eve_example_lib PUBLIC include)
target_link_libraries(eve_example_lib PUBLIC eve::editor eve::human eve::world)
target_compile_features(eve_example_lib PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_example_lib)

add_executable(eve_example_scene main.cpp)
target_link_libraries(eve_example_scene PRIVATE eve::example)
set_target_properties(eve_example_scene PROPERTIES OUTPUT_NAME "eve-example-scene")
