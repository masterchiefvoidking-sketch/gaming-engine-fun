add_library(eve_product STATIC
    src/bug_triage.cpp
    src/content_completion.cpp
    src/project_archive_enhanced.cpp
    src/release_checklist.cpp
    src/release_bundle.cpp
    src/save_stress_tester.cpp
    src/personal_product_cut.cpp
)

add_library(eve::product ALIAS eve_product)

target_include_directories(eve_product
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(eve_product
    PUBLIC
        eve::core
        eve::build
        eve::platform
        eve::content
        eve::gameplay
        eve::polish
        eve::editor
        nlohmann_json::nlohmann_json
)

target_compile_features(eve_product PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_product)
eve_set_compile_definitions(eve_product)
