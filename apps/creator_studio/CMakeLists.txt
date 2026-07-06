add_executable(eve_creator_studio main.cpp)
target_link_libraries(eve_creator_studio PRIVATE eve::editor)
set_target_properties(eve_creator_studio PROPERTIES OUTPUT_NAME "eve-creator-studio")
