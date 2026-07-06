add_library(eve_world INTERFACE)
add_library(eve::world ALIAS eve_world)

target_include_directories(eve_world INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(eve_world INTERFACE eve::core eve::ecs eve::render)
