add_executable(eve_editor main.cpp)

target_link_libraries(eve_editor PRIVATE eve::editor)

set_target_properties(eve_editor PROPERTIES
    OUTPUT_NAME "eve-editor"
)
