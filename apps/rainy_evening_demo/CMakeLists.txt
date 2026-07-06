add_library(eve_rainy_demo_lib STATIC src/rainy_evening_demo.cpp)
add_library(eve::rainy_demo ALIAS eve_rainy_demo_lib)

target_include_directories(eve_rainy_demo_lib PUBLIC include)
target_link_libraries(eve_rainy_demo_lib PUBLIC eve::ai)
target_compile_features(eve_rainy_demo_lib PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_rainy_demo_lib)

add_executable(eve_rainy_evening_demo main.cpp)
target_link_libraries(eve_rainy_evening_demo PRIVATE eve::rainy_demo)
set_target_properties(eve_rainy_evening_demo PROPERTIES OUTPUT_NAME "eve-rainy-evening-demo")
