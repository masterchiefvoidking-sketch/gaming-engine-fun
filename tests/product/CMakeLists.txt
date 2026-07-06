eve_add_test(eve_product_tests test_personal_product_cut.cpp test_content_pack01.cpp)

target_link_libraries(eve_product_tests PRIVATE eve::product eve::personal_product_cut eve::editor)
target_compile_definitions(eve_product_tests PRIVATE
    EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data"
    EVE_GAME_DIR="${CMAKE_SOURCE_DIR}/Game"
    EVE_RELEASES_DIR="${CMAKE_SOURCE_DIR}/Releases"
    EVE_REPO_DIR="${CMAKE_SOURCE_DIR}"
)
