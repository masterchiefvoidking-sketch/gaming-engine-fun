#include <eve/audit/dependency_review.hpp>

namespace eve::audit {

std::vector<DependencyEntry> DependencyReviewer::audit() const {
    return {
        {"Google Test", "v1.15.2", "Automated test framework", true},
        {"GLFW", "3.4", "Windowing and input for editor/render", true},
        {"nlohmann/json", "v3.11.3", "JSON serialization across all data modules", true},
        {"Vulkan SDK", "system", "Graphics API for render module", true},
    };
}

} // namespace eve::audit
