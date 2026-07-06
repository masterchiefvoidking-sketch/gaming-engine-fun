eve_add_test(eve_editor_tests test_editor_shell.cpp)
target_link_libraries(eve_editor_tests PRIVATE eve::editor)

eve_add_test(eve_editor_apartment_tests test_apartment_builder.cpp)
target_link_libraries(eve_editor_apartment_tests PRIVATE eve::editor)
target_compile_definitions(eve_editor_apartment_tests PRIVATE EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data")
