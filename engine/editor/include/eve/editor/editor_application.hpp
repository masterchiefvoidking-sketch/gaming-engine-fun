#pragma once

#include <eve/ecs/world.hpp>
#include <eve/render/vulkan_context.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>

namespace eve::editor {

struct EditorConfig {
    std::string title = "EVE Editor";
    int width = 1600;
    int height = 900;
    bool enable_validation = false;
};

class EditorApplication {
public:
    EditorApplication();
    ~EditorApplication();

    bool initialize(const EditorConfig& config = {});
    void run();
    void shutdown();

    [[nodiscard]] bool is_running() const { return running_; }
    [[nodiscard]] GLFWwindow* window() const { return window_; }
    [[nodiscard]] render::VulkanContext& renderer() { return renderer_; }

private:
    void poll_events();
    void begin_frame();
    void end_frame();
    void draw_docking_shell();

    bool running_ = false;
    GLFWwindow* window_ = nullptr;
    render::VulkanContext renderer_;
    ecs::ComponentRegistry component_registry_;
    ecs::World world_;
    double last_frame_time_ = 0.0;
};

} // namespace eve::editor
