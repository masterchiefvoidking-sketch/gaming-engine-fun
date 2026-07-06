add_library(eve_movie_demo_lib STATIC src/rainy_movie_night_demo.cpp)
add_library(eve::movie_demo ALIAS eve_movie_demo_lib)

target_include_directories(eve_movie_demo_lib PUBLIC include)
target_link_libraries(eve_movie_demo_lib PUBLIC eve::scene eve::ai eve::render eve::animation eve::human eve::world)
target_compile_features(eve_movie_demo_lib PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_movie_demo_lib)

add_executable(eve_rainy_movie_night_demo main.cpp)
target_link_libraries(eve_rainy_movie_night_demo PRIVATE eve::movie_demo)
set_target_properties(eve_rainy_movie_night_demo PROPERTIES OUTPUT_NAME "eve-rainy-movie-night-demo")
