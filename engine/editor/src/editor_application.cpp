#include <eve/core/logging/logger.hpp>
#include <eve/core/math/math.hpp>
#include <eve/editor/editor_application.hpp>

namespace eve::editor {

struct TransformComponent {
    math::Vec3 position{};
    math::Quat rotation = math::Quat::identity();
    math::Vec3 scale{1.0f, 1.0f, 1.0f};
};

struct NameComponent {
    std::string name = "Entity";
};

EditorApplication::EditorApplication()
    : world_(component_registry_) {
    component_registry_.register_component<TransformComponent>();
    component_registry_.register_component<NameComponent>();
}

EditorApplication::~EditorApplication() {
    shutdown();
}

bool EditorApplication::initialize(const EditorConfig& config) {
    if (!glfwInit()) {
        EVE_LOG(Error, "Editor", "Failed to initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window_ = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);
    if (window_ == nullptr) {
        EVE_LOG(Error, "Editor", "Failed to create editor window");
        glfwTerminate();
        return false;
    }

    if (!renderer_.initialize(config.title.c_str(), config.enable_validation)) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
        glfwTerminate();
        return false;
    }

    if (!renderer_.attach_window(window_)) {
        renderer_.shutdown();
        glfwDestroyWindow(window_);
        window_ = nullptr;
        glfwTerminate();
        return false;
    }

    const EntityId scene_root = world_.create_entity();
    world_.add_component<NameComponent>(scene_root).name = "Scene Root";
    world_.add_component<TransformComponent>(scene_root);

    running_ = true;
    last_frame_time_ = glfwGetTime();
    EVE_LOG(Info, "Editor", "Editor shell initialized (", config.width, "x", config.height, ")");
    return true;
}

void EditorApplication::run() {
    while (running_ && !glfwWindowShouldClose(window_)) {
        poll_events();
        begin_frame();
        draw_docking_shell();
        end_frame();
    }
}

void EditorApplication::shutdown() {
    if (window_ != nullptr) {
        renderer_.shutdown();
        glfwDestroyWindow(window_);
        window_ = nullptr;
        glfwTerminate();
    }
    running_ = false;
}

void EditorApplication::poll_events() {
    glfwPollEvents();
}

void EditorApplication::begin_frame() {
    const double now = glfwGetTime();
    const double delta = now - last_frame_time_;
    last_frame_time_ = now;
    (void)delta;
}

void EditorApplication::end_frame() {
    // Rendering presentation will be implemented in the render milestone.
}

void EditorApplication::draw_docking_shell() {
    // Docking UI panels (Character Creator, Timeline, Asset Browser) attach here.
}

} // namespace eve::editor
