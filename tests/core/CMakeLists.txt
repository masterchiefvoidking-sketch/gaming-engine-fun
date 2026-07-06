eve_add_test(eve_core_tests
    test_memory.cpp
    test_logging.cpp
    test_math.cpp
    test_reflection.cpp
    test_serialization.cpp
    test_filesystem.cpp
    test_threading.cpp
)

target_link_libraries(eve_core_tests PRIVATE eve::core)
