add_library(eve_animation STATIC
    src/state_machine.cpp
    src/blend_tree.cpp
    src/ik_solver.cpp
    src/facial_animation.cpp
    src/layer_stack.cpp
)

add_library(eve::animation ALIAS eve_animation)

target_include_directories(eve_animation
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(eve_animation PUBLIC eve::core nlohmann_json::nlohmann_json)
target_compile_features(eve_animation PUBLIC cxx_std_20)
eve_set_compile_warnings(eve_animation)
eve_set_compile_definitions(eve_animation)
