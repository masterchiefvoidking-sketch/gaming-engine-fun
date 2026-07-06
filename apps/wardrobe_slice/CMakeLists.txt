add_library(eve_wardrobe_slice_lib STATIC src/wardrobe_vertical_slice.cpp)
add_library(eve::wardrobe_slice ALIAS eve_wardrobe_slice_lib)

target_include_directories(eve_wardrobe_slice_lib PUBLIC include)
target_link_libraries(eve_wardrobe_slice_lib PUBLIC eve::human eve::ai eve::animation eve::render)
target_compile_features(eve_wardrobe_slice_lib PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_wardrobe_slice_lib)

add_executable(eve_wardrobe_slice main.cpp)
target_link_libraries(eve_wardrobe_slice PRIVATE eve::wardrobe_slice)
set_target_properties(eve_wardrobe_slice PROPERTIES OUTPUT_NAME "eve-wardrobe-slice")
