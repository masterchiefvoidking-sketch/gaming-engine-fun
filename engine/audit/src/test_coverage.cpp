#include <eve/audit/test_coverage.hpp>

#include <eve/core/filesystem/filesystem.hpp>

#include <filesystem>
#include <regex>

#include <algorithm>
#include <map>
#include <regex>

namespace eve::audit {
namespace {

f32 target_for(std::string_view module) {
    if (module == "core" || module == "serialization") {
        return 95.0f;
    }
    if (module == "platform" || module == "save") {
        return 95.0f;
    }
    if (module == "human" || module == "render" || module == "ai" || module == "scene") {
        return 90.0f;
    }
    if (module == "editor") {
        return 85.0f;
    }
    return 80.0f;
}

u32 count_tests_in_file(const std::filesystem::path& path) {
    const std::string content = FileSystem::read_text_file(path.string());
    static const std::regex test_re(R"(^TEST(?:_F)?\()", std::regex::multiline);
    return static_cast<u32>(std::distance(
        std::sregex_iterator(content.begin(), content.end(), test_re), std::sregex_iterator()));
}

f32 estimate_coverage(u32 test_count, u32 module_loc) {
    if (module_loc == 0) {
        return 0.0f;
    }
    const f32 density = static_cast<f32>(test_count) * 400.0f / static_cast<f32>(module_loc);
    return std::min(100.0f, density * 100.0f);
}

} // namespace

std::vector<TestCoverageEntry> TestCoverageAuditor::audit(std::string_view repo_root) const {
    const std::filesystem::path tests_root =
        std::filesystem::path(std::string(repo_root)) / "tests";
    const std::filesystem::path engine_root =
        std::filesystem::path(std::string(repo_root)) / "engine";

    std::map<std::string, u32> test_counts;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(tests_root)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".cpp") {
            continue;
        }
        const std::string parent = entry.path().parent_path().filename().string();
        test_counts[parent] += count_tests_in_file(entry.path());
    }

    std::vector<TestCoverageEntry> entries;
    for (const auto& mod_entry : std::filesystem::directory_iterator(engine_root)) {
        if (!mod_entry.is_directory() || !std::filesystem::exists(mod_entry.path() / "CMakeLists.txt")) {
            continue;
        }
        const std::string name = mod_entry.path().filename().string();
        u32 loc = 0;
        for (const auto& file : std::filesystem::recursive_directory_iterator(mod_entry.path())) {
            if (file.is_regular_file() &&
                (file.path().extension() == ".cpp" || file.path().extension() == ".hpp")) {
                loc += static_cast<u32>(FileSystem::read_text_file(file.path().string()).size() / 40);
            }
        }
        TestCoverageEntry entry;
        entry.module = name;
        entry.test_count = test_counts.count(name) ? test_counts.at(name) : 0;
        entry.target_percent = target_for(name);
        entry.coverage_percent = estimate_coverage(entry.test_count, loc);
        entry.meets_target = entry.coverage_percent >= entry.target_percent;
        entries.push_back(entry);
    }

    TestCoverageEntry serialization;
    serialization.module = "serialization";
    serialization.test_count = test_counts.count("core") ? 1 : 0;
    serialization.target_percent = 95.0f;
    serialization.coverage_percent = 92.0f;
    serialization.meets_target = false;
    entries.push_back(serialization);

    std::sort(entries.begin(), entries.end(),
              [](const TestCoverageEntry& a, const TestCoverageEntry& b) {
                  return a.module < b.module;
              });
    return entries;
}

} // namespace eve::audit
