add_library(eve_ai INTERFACE)
add_library(eve::ai ALIAS eve_ai)

target_include_directories(eve_ai INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(eve_ai INTERFACE eve::core eve::ecs)
