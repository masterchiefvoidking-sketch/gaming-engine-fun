add_library(eve_human INTERFACE)
add_library(eve::human ALIAS eve_human)

target_include_directories(eve_human INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(eve_human INTERFACE eve::core eve::render)
