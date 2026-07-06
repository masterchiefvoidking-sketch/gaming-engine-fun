#pragma once

#include <eve/core/types.hpp>
#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

namespace eve::render {

struct VulkanQueueFamilies {
    u32 graphics_family = 0;
    u32 present_family = 0;
    bool graphics_supported = false;
    bool present_supported = false;
};

struct VulkanCapabilities {
    std::string device_name;
    u32 api_version = 0;
    bool ray_tracing_ready = false;
    bool swapchain_supported = false;
};

class VulkanContext {
public:
    VulkanContext() = default;
    ~VulkanContext();

    VulkanContext(const VulkanContext&) = delete;
    VulkanContext& operator=(const VulkanContext&) = delete;

    bool initialize(const char* application_name, bool enable_validation = false);
    bool attach_window(GLFWwindow* window);
    void shutdown();

    [[nodiscard]] bool is_initialized() const { return initialized_; }
    [[nodiscard]] VkInstance instance() const { return instance_; }
    [[nodiscard]] VkPhysicalDevice physical_device() const { return physical_device_; }
    [[nodiscard]] VkDevice device() const { return device_; }
    [[nodiscard]] VkSurfaceKHR surface() const { return surface_; }
    [[nodiscard]] VkQueue graphics_queue() const { return graphics_queue_; }
    [[nodiscard]] VkQueue present_queue() const { return present_queue_; }
    [[nodiscard]] const VulkanCapabilities& capabilities() const { return capabilities_; }

private:
    bool create_instance(const char* application_name, bool enable_validation);
    bool pick_physical_device();
    bool create_logical_device();
    bool create_surface(GLFWwindow* window);
    bool create_swapchain_stub();

    bool initialized_ = false;
    VkInstance instance_ = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT debug_messenger_ = VK_NULL_HANDLE;
    VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
    VkDevice device_ = VK_NULL_HANDLE;
    VkSurfaceKHR surface_ = VK_NULL_HANDLE;
    VkQueue graphics_queue_ = VK_NULL_HANDLE;
    VkQueue present_queue_ = VK_NULL_HANDLE;
    VulkanQueueFamilies queue_families_{};
    VulkanCapabilities capabilities_{};
    std::vector<const char*> enabled_extensions_;
};

} // namespace eve::render
