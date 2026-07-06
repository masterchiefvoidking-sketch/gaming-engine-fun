add_library(eve_physics INTERFACE)
add_library(eve::physics ALIAS eve_physics)

target_include_directories(eve_physics INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(eve_physics INTERFACE eve::core)
