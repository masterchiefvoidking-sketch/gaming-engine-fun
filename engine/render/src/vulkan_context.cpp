#include <eve/core/logging/logger.hpp>
#include <eve/render/vulkan_context.hpp>
#include <set>
#include <stdexcept>

namespace eve::render {

namespace {

VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data) {
    (void)type;
    (void)user_data;
    if (severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        EVE_LOG(Warn, "Vulkan", callback_data->pMessage);
    } else {
        EVE_LOG(Debug, "Vulkan", callback_data->pMessage);
    }
    return VK_FALSE;
}

VulkanQueueFamilies find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface) {
    VulkanQueueFamilies families{};
    u32 count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
    std::vector<VkQueueFamilyProperties> props(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, props.data());

    for (u32 i = 0; i < count; ++i) {
        if (props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            families.graphics_family = i;
            families.graphics_supported = true;
        }
        if (surface != VK_NULL_HANDLE) {
            VkBool32 present_support = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);
            if (present_support) {
                families.present_family = i;
                families.present_supported = true;
            }
        }
    }
    return families;
}

} // namespace

VulkanContext::~VulkanContext() {
    shutdown();
}

bool VulkanContext::initialize(const char* application_name, bool enable_validation) {
    if (initialized_) {
        return true;
    }

    if (!create_instance(application_name, enable_validation)) {
        return false;
    }
    if (!pick_physical_device()) {
        shutdown();
        return false;
    }
    if (!create_logical_device()) {
        shutdown();
        return false;
    }

    initialized_ = true;
    EVE_LOG(Info, "Render", "Vulkan initialized on device: ", capabilities_.device_name);
    return true;
}

bool VulkanContext::attach_window(GLFWwindow* window) {
    if (!initialized_ || window == nullptr) {
        return false;
    }
    if (!create_surface(window)) {
        return false;
    }
    capabilities_.swapchain_supported = queue_families_.present_supported;
    return create_swapchain_stub();
}

void VulkanContext::shutdown() {
    if (device_ != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(device_);
        vkDestroyDevice(device_, nullptr);
        device_ = VK_NULL_HANDLE;
    }
    if (surface_ != VK_NULL_HANDLE && instance_ != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(instance_, surface_, nullptr);
        surface_ = VK_NULL_HANDLE;
    }
    if (debug_messenger_ != VK_NULL_HANDLE) {
        auto destroy_fn = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(instance_, "vkDestroyDebugUtilsMessengerEXT"));
        if (destroy_fn != nullptr) {
            destroy_fn(instance_, debug_messenger_, nullptr);
        }
        debug_messenger_ = VK_NULL_HANDLE;
    }
    if (instance_ != VK_NULL_HANDLE) {
        vkDestroyInstance(instance_, nullptr);
        instance_ = VK_NULL_HANDLE;
    }
    initialized_ = false;
}

bool VulkanContext::create_instance(const char* application_name, bool enable_validation) {
    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = application_name;
    app_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    app_info.pEngineName = kEngineName;
    app_info.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    app_info.apiVersion = VK_API_VERSION_1_3;

    std::vector<const char*> extensions = {VK_KHR_SURFACE_EXTENSION_NAME};
#if defined(_WIN32)
    extensions.push_back("VK_KHR_win32_surface");
#elif defined(__linux__)
    extensions.push_back("VK_KHR_xcb_surface");
#elif defined(__APPLE__)
    extensions.push_back("VK_EXT_metal_surface");
#endif

    std::vector<const char*> layers;
    if (enable_validation) {
        layers.push_back("VK_LAYER_KHRONOS_validation");
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    create_info.enabledExtensionCount = static_cast<u32>(extensions.size());
    create_info.ppEnabledExtensionNames = extensions.data();
    create_info.enabledLayerCount = static_cast<u32>(layers.size());
    create_info.ppEnabledLayerNames = layers.data();

    if (vkCreateInstance(&create_info, nullptr, &instance_) != VK_SUCCESS) {
        EVE_LOG(Error, "Render", "Failed to create Vulkan instance");
        return false;
    }

    enabled_extensions_ = extensions;

    if (enable_validation) {
        VkDebugUtilsMessengerCreateInfoEXT debug_info{};
        debug_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debug_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debug_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
        debug_info.pfnUserCallback = debug_callback;

        auto create_fn = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(instance_, "vkCreateDebugUtilsMessengerEXT"));
        if (create_fn != nullptr) {
            create_fn(instance_, &debug_info, nullptr, &debug_messenger_);
        }
    }

    return true;
}

bool VulkanContext::pick_physical_device() {
    u32 count = 0;
    vkEnumeratePhysicalDevices(instance_, &count, nullptr);
    if (count == 0) {
        EVE_LOG(Error, "Render", "No Vulkan physical devices found");
        return false;
    }

    std::vector<VkPhysicalDevice> devices(count);
    vkEnumeratePhysicalDevices(instance_, &count, devices.data());

    physical_device_ = devices.front();
    for (VkPhysicalDevice device : devices) {
        VkPhysicalDeviceProperties props{};
        vkGetPhysicalDeviceProperties(device, &props);
        if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            physical_device_ = device;
            break;
        }
    }

    VkPhysicalDeviceProperties props{};
    vkGetPhysicalDeviceProperties(physical_device_, &props);
    capabilities_.device_name = props.deviceName;
    capabilities_.api_version = props.apiVersion;

    VkPhysicalDeviceRayTracingPipelinePropertiesKHR rt_props{};
    rt_props.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;
    VkPhysicalDeviceProperties2 props2{};
    props2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    props2.pNext = &rt_props;
    vkGetPhysicalDeviceProperties2(physical_device_, &props2);
    capabilities_.ray_tracing_ready = rt_props.maxRayRecursionDepth > 0;

    queue_families_ = find_queue_families(physical_device_, surface_);
    return queue_families_.graphics_supported;
}

bool VulkanContext::create_logical_device() {
    std::set<u32> unique_families;
    if (queue_families_.graphics_supported) {
        unique_families.insert(queue_families_.graphics_family);
    }
    if (queue_families_.present_supported) {
        unique_families.insert(queue_families_.present_family);
    }

    std::vector<VkDeviceQueueCreateInfo> queue_infos;
    const float priority = 1.0f;
    for (u32 family : unique_families) {
        VkDeviceQueueCreateInfo queue_info{};
        queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_info.queueFamilyIndex = family;
        queue_info.queueCount = 1;
        queue_info.pQueuePriorities = &priority;
        queue_infos.push_back(queue_info);
    }

    VkPhysicalDeviceFeatures features{};
    features.samplerAnisotropy = VK_TRUE;

    const char* device_extensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkDeviceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount = static_cast<u32>(queue_infos.size());
    create_info.pQueueCreateInfos = queue_infos.data();
    create_info.pEnabledFeatures = &features;
    create_info.enabledExtensionCount = 1;
    create_info.ppEnabledExtensionNames = device_extensions;

    if (vkCreateDevice(physical_device_, &create_info, nullptr, &device_) != VK_SUCCESS) {
        EVE_LOG(Error, "Render", "Failed to create Vulkan logical device");
        return false;
    }

    vkGetDeviceQueue(device_, queue_families_.graphics_family, 0, &graphics_queue_);
    if (queue_families_.present_supported) {
        vkGetDeviceQueue(device_, queue_families_.present_family, 0, &present_queue_);
    }
    return true;
}

bool VulkanContext::create_surface(GLFWwindow* window) {
    if (glfwCreateWindowSurface(instance_, window, nullptr, &surface_) != VK_SUCCESS) {
        EVE_LOG(Error, "Render", "Failed to create window surface");
        return false;
    }
    queue_families_ = find_queue_families(physical_device_, surface_);
    return true;
}

bool VulkanContext::create_swapchain_stub() {
    EVE_LOG(Info, "Render", "Swapchain pipeline slot reserved (implementation milestone 2)");
    return true;
}

} // namespace eve::render
