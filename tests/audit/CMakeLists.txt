eve_add_test(eve_audit_tests test_architecture_freeze.cpp)

target_link_libraries(eve_audit_tests PRIVATE eve::audit eve::architecture_freeze)
target_compile_definitions(eve_audit_tests PRIVATE
    EVE_DATA_DIR="${CMAKE_SOURCE_DIR}/data"
    EVE_GAME_DIR="${CMAKE_SOURCE_DIR}/Game"
    EVE_REPO_DIR="${CMAKE_SOURCE_DIR}"
)
