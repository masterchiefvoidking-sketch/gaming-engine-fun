eve_add_test(eve_render_tests
    test_vulkan_context.cpp
    test_inspection_camera.cpp
    test_human_shader_pipeline.cpp
    test_photo_mode.cpp
    test_camera_transitions.cpp
)

target_link_libraries(eve_render_tests PRIVATE eve::render eve::human)
