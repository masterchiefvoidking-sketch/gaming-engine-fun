add_library(eve_build STATIC
    src/export_profiles.cpp
    src/asset_cooker.cpp
    src/content_validator.cpp
    src/dependency_manifest.cpp
    src/project_archive.cpp
    src/personal_content_pack.cpp
    src/personal_backup_tool.cpp
    src/build_pipeline.cpp
)

add_library(eve::build ALIAS eve_build)

target_include_directories(eve_build
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(eve_build
    PUBLIC
        eve::core
        eve::content
        eve::platform
        nlohmann_json::nlohmann_json
)

target_compile_features(eve_build PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_build)
eve_set_compile_definitions(eve_build)
