add_library(eve_render STATIC
    src/vulkan_context.cpp
    src/inspection_camera.cpp
    src/human_shader_pipeline.cpp
    src/hair_renderer.cpp
    src/photo_mode.cpp
)

add_library(eve::render ALIAS eve_render)

target_include_directories(eve_render
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(eve_render
    PUBLIC
        eve::core
        Vulkan::Vulkan
        glfw
)

target_compile_features(eve_render PUBLIC cxx_std_20)

eve_set_compile_warnings(eve_render)
eve_set_compile_definitions(eve_render)
