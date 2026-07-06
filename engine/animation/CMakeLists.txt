# Animation module — milestone 2
# Motion matching, state machines, blend trees, facial animation

add_library(eve_animation INTERFACE)
add_library(eve::animation ALIAS eve_animation)

target_include_directories(eve_animation INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(eve_animation INTERFACE eve::core)
