#include <eve/audit/memory_review.hpp>

#include <eve/core/filesystem/filesystem.hpp>

#include <filesystem>

namespace eve::audit {
namespace {

u64 estimate_json_bytes(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        return 0;
    }
    if (std::filesystem::is_directory(path)) {
        u64 total = 0;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
            if (entry.is_regular_file()) {
                total += entry.file_size();
            }
        }
        return total;
    }
    return std::filesystem::file_size(path);
}

MemoryCategoryReport category(std::string name, u64 bytes, std::string notes) {
    return {std::move(name), bytes, std::move(notes)};
}

} // namespace

std::vector<MemoryCategoryReport> MemoryReviewer::audit(std::string_view data_root,
                                                        std::string_view game_root) const {
    const std::string data(data_root);
    const std::string game(game_root);
    std::vector<MemoryCategoryReport> reports;

    reports.push_back(category("Texture memory (metadata)",
                               estimate_json_bytes(data + "/anime/materials.json"),
                               "JSON material definitions; GPU textures not yet loaded"));
    reports.push_back(category("Mesh memory (metadata)",
                               estimate_json_bytes(game + "/Assets/Characters"),
                               "Character mesh references in JSON"));
    reports.push_back(category("Animation memory",
                               estimate_json_bytes(game + "/Assets/Animations"),
                               "Animation clip definitions"));
    reports.push_back(category("Audio memory", 0, "No audio middleware integrated yet"));
    reports.push_back(category("Shader cache",
                               estimate_json_bytes(game + "/Build/windows_development/cooked/shader_variants.json"),
                               "Cooked shader variant manifest"));
    reports.push_back(category("Asset cache",
                               estimate_json_bytes(game + "/Build/windows_development/cooked"),
                               "Cooked runtime package"));
    reports.push_back(category("Save data",
                               estimate_json_bytes(data + "/saves"),
                               "Unified save JSON files"));
    reports.push_back(
        category("Runtime allocations", 0,
                 "Use PerformanceProfiler and allocator hooks for runtime tracking"));

    return reports;
}

} // namespace eve::audit
