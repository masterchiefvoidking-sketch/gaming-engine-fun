add_library(eve_anime_demo_lib STATIC src/cozy_wardrobe_demo.cpp)
add_library(eve::anime_demo ALIAS eve_anime_demo_lib)

target_include_directories(eve_anime_demo_lib PUBLIC include)
target_link_libraries(eve_anime_demo_lib PUBLIC eve::human eve::world eve::animation eve::render)
target_compile_features(eve_anime_demo_lib PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_anime_demo_lib)

add_executable(eve_anime_wardrobe_demo main.cpp)
target_link_libraries(eve_anime_wardrobe_demo PRIVATE eve::anime_demo)
set_target_properties(eve_anime_wardrobe_demo PROPERTIES OUTPUT_NAME "eve-anime-wardrobe-demo")
