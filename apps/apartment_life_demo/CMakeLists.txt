add_library(eve_apartment_demo_lib STATIC src/apartment_life_demo_app.cpp)
add_library(eve::apartment_demo ALIAS eve_apartment_demo_lib)

target_include_directories(eve_apartment_demo_lib PUBLIC include)
target_link_libraries(eve_apartment_demo_lib PUBLIC eve::gameplay)
target_compile_features(eve_apartment_demo_lib PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_apartment_demo_lib)

add_executable(eve-apartment-life-demo main.cpp)
target_link_libraries(eve-apartment-life-demo PRIVATE eve::apartment_demo)
target_compile_definitions(eve-apartment-life-demo PRIVATE
    EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data"
    EVE_GAME_DIR="${CMAKE_SOURCE_DIR}/Game"
)
set_target_properties(eve-apartment-life-demo PROPERTIES OUTPUT_NAME "eve-apartment-life-demo")
