cmake_minimum_required(VERSION 3.24)

add_library(eve_audit STATIC
    src/module_health.cpp
    src/performance_review.cpp
    src/memory_review.cpp
    src/test_coverage.cpp
    src/dependency_review.cpp
    src/api_review.cpp
    src/architecture_freeze.cpp
)

add_library(eve::audit ALIAS eve_audit)

target_include_directories(eve_audit
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(eve_audit
    PUBLIC
        eve::core
        eve::content
        eve::human
        eve::platform
        eve::build
        eve::ai
        eve::gameplay
        nlohmann_json::nlohmann_json
)

target_compile_features(eve_audit PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_audit)
eve_set_compile_definitions(eve_audit)
