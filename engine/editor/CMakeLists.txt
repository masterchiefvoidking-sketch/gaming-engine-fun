add_library(eve_editor_lib STATIC
    src/editor_application.cpp
    src/apartment_builder.cpp
    src/relationship_inspector.cpp
    src/schedule_editor.cpp
    src/interaction_editor.cpp
)

add_library(eve::editor ALIAS eve_editor_lib)

target_include_directories(eve_editor_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(eve_editor_lib
    PUBLIC
        eve::core
        eve::ecs
        eve::render
        eve::world
        eve::ai
        eve::scripting
        glfw
)

target_compile_features(eve_editor_lib PUBLIC cxx_std_20)

eve_set_compile_warnings(eve_editor_lib)
eve_set_compile_definitions(eve_editor_lib)
