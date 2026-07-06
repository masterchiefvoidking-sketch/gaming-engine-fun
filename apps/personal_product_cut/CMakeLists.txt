add_library(eve_personal_product_cut_lib STATIC src/personal_product_cut_app.cpp)
add_library(eve::personal_product_cut ALIAS eve_personal_product_cut_lib)

target_include_directories(eve_personal_product_cut_lib PUBLIC include)
target_link_libraries(eve_personal_product_cut_lib PUBLIC eve::product eve::editor eve::gameplay)
target_compile_features(eve_personal_product_cut_lib PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_personal_product_cut_lib)

add_executable(eve-personal-product-cut main.cpp)
target_link_libraries(eve-personal-product-cut PRIVATE eve::personal_product_cut)
target_compile_definitions(eve-personal-product-cut PRIVATE
    EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data"
    EVE_GAME_DIR="${CMAKE_SOURCE_DIR}/Game"
    EVE_RELEASES_DIR="${CMAKE_SOURCE_DIR}/Releases"
)
set_target_properties(eve-personal-product-cut PROPERTIES OUTPUT_NAME "eve-personal-product-cut")
