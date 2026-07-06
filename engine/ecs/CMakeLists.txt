add_library(eve_ecs STATIC
    src/world.cpp
)

add_library(eve::ecs ALIAS eve_ecs)

target_include_directories(eve_ecs
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(eve_ecs PUBLIC eve::core)
target_compile_features(eve_ecs PUBLIC cxx_std_20)

eve_set_compile_warnings(eve_ecs)
eve_set_compile_definitions(eve_ecs)
