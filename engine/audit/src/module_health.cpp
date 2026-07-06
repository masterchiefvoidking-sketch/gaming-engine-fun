#include <eve/audit/module_health.hpp>

#include <eve/core/filesystem/filesystem.hpp>

#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>

namespace eve::audit {
namespace {

u32 count_lines(const std::filesystem::path& path) {
    std::ifstream in(path);
    return static_cast<u32>(std::count(std::istreambuf_iterator<char>(in),
                                       std::istreambuf_iterator<char>(), '\n'));
}

u32 estimate_complexity(const std::filesystem::path& path) {
    const std::string content = FileSystem::read_text_file(path.string());
    u32 score = 1;
    static const std::regex branches(R"(\b(if|else|for|while|case|catch|&&|\|\|)\b)");
    for (auto it = std::sregex_iterator(content.begin(), content.end(), branches);
         it != std::sregex_iterator(); ++it) {
        ++score;
    }
    return score;
}

std::vector<std::string> list_public_headers(const std::filesystem::path& include_dir) {
    std::vector<std::string> headers;
    if (!std::filesystem::exists(include_dir)) {
        return headers;
    }
    for (const auto& entry : std::filesystem::recursive_directory_iterator(include_dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".hpp") {
            headers.push_back(entry.path().filename().string());
        }
    }
    std::sort(headers.begin(), headers.end());
    return headers;
}

std::vector<std::string> parse_cmake_deps(const std::filesystem::path& cmake_path,
                                          std::string_view module_name) {
    const std::string content = FileSystem::read_text_file(cmake_path.string());
    std::regex dep_re(R"(eve::(\w+))");
    std::vector<std::string> deps;
    for (auto it = std::sregex_iterator(content.begin(), content.end(), dep_re);
         it != std::sregex_iterator(); ++it) {
        const std::string dep = (*it)[1].str();
        if (dep != std::string(module_name)) {
            deps.push_back("eve::" + dep);
        }
    }
    std::sort(deps.begin(), deps.end());
    deps.erase(std::unique(deps.begin(), deps.end()), deps.end());
    return deps;
}

std::string purpose_for(std::string_view module) {
    static const std::pair<const char*, const char*> kPurposes[] = {
        {"core", "Foundation: memory, logging, math, serialization, filesystem, threading"},
        {"ecs", "Entity-component-system world and registry"},
        {"render", "Vulkan renderer, cameras, shaders, photo mode"},
        {"animation", "State machines, blend trees, IK, facial animation"},
        {"scripting", "Script host abstraction and Python bridge stub"},
        {"world", "Apartment simulation, interactions, persistence"},
        {"ai", "Relationship simulation, dialogue, schedules, mood"},
        {"scene", "Scene director, cues, event scheduling"},
        {"content", "Content database, project layout, mod system, wardrobe DB"},
        {"platform", "Unified save, web deploy, mobile input, backups"},
        {"interaction", "Input, web viewer, inspection mode, UI state"},
        {"build", "Export pipeline: cook, validate, archive, migrate"},
        {"human", "Character creator, wardrobe system, anime visuals"},
        {"physics", "Placeholder interface for future physics integration"},
        {"editor", "Authoring shell: creator studio, apartment builder, tools"},
        {"polish", "Performance profiling, validation gate, graphics polish"},
        {"gameplay", "Integrated gameplay session orchestration"},
        {"product", "Release tooling: checklist, stress test, bundle"},
        {"audit", "Architecture freeze audit and reporting"},
    };
    for (const auto& [name, purpose] : kPurposes) {
        if (module == name) {
            return purpose;
        }
    }
    return "Engine subsystem";
}

} // namespace

std::vector<ModuleHealthReport> ModuleHealthAuditor::audit_all(std::string_view repo_root) const {
    std::vector<ModuleHealthReport> reports;
    const std::filesystem::path engine_root = std::filesystem::path(std::string(repo_root)) / "engine";

    for (const auto& entry : std::filesystem::directory_iterator(engine_root)) {
        if (!entry.is_directory()) {
            continue;
        }
        const std::filesystem::path cmake = entry.path() / "CMakeLists.txt";
        if (!std::filesystem::exists(cmake)) {
            continue;
        }

        ModuleHealthReport report;
        report.name = entry.path().filename().string();
        report.purpose = purpose_for(report.name);
        report.dependencies = parse_cmake_deps(cmake, report.name);
        report.public_apis =
            list_public_headers(entry.path() / "include" / "eve" / report.name);

        u32 loc = 0;
        u32 complexity = 0;
        for (const auto& file :
             std::filesystem::recursive_directory_iterator(entry.path())) {
            if (!file.is_regular_file()) {
                continue;
            }
            const auto ext = file.path().extension().string();
            if (ext == ".cpp" || ext == ".hpp") {
                loc += count_lines(file.path());
                if (ext == ".cpp") {
                    complexity += estimate_complexity(file.path());
                }
            }
        }
        report.lines_of_code = loc;
        report.cyclomatic_complexity = complexity;

        if (report.name == "physics") {
            report.known_issues.push_back("INTERFACE stub only — no implementation");
            report.recommendations.push_back("Implement or remove before physics features");
        }
        if (report.dependencies.size() > 6) {
            report.known_issues.push_back("Hub module with " +
                                          std::to_string(report.dependencies.size()) + " dependencies");
            report.recommendations.push_back("Extract interfaces to reduce coupling");
        }
        if (report.name == "content" &&
            std::find(report.dependencies.begin(), report.dependencies.end(), "eve::ai") !=
                report.dependencies.end()) {
            report.known_issues.push_back("Data layer depends on simulation layer (ai)");
            report.recommendations.push_back("Invert dependency via content-only types");
        }
        if (report.public_apis.empty()) {
            report.recommendations.push_back("Add public API headers");
        }
        reports.push_back(std::move(report));
    }

    std::sort(reports.begin(), reports.end(),
              [](const ModuleHealthReport& a, const ModuleHealthReport& b) {
                  return a.name < b.name;
              });
    return reports;
}

} // namespace eve::audit
