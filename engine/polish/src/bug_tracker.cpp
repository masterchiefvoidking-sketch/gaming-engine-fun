#include <eve/polish/bug_tracker.hpp>

#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::polish {

namespace {

const char* severity_string(BugSeverity severity) {
    switch (severity) {
    case BugSeverity::Critical:
        return "critical";
    case BugSeverity::High:
        return "high";
    case BugSeverity::Medium:
        return "medium";
    case BugSeverity::Low:
        return "low";
    }
    return "medium";
}

} // namespace

void BugTracker::add_bug(BugReport bug) {
    bugs_.push_back(std::move(bug));
}

bool BugTracker::mark_fixed(std::string_view bug_id, std::string_view regression_test) {
    for (BugReport& bug : bugs_) {
        if (bug.id == bug_id) {
            bug.fixed = true;
            bug.regression_test = std::string(regression_test);
            return true;
        }
    }
    return false;
}

std::size_t BugTracker::open_count() const {
    std::size_t count = 0;
    for (const BugReport& bug : bugs_) {
        if (!bug.fixed) {
            ++count;
        }
    }
    return count;
}

bool BugTracker::save(std::string_view path) const {
    nlohmann::json json = nlohmann::json::array();
    for (const BugReport& bug : bugs_) {
        json.push_back({{"id", bug.id},
                        {"severity", severity_string(bug.severity)},
                        {"reproduction_steps", bug.reproduction_steps},
                        {"expected_behavior", bug.expected_behavior},
                        {"actual_behavior", bug.actual_behavior},
                        {"affected_platform", bug.affected_platform},
                        {"fix_owner", bug.fix_owner},
                        {"regression_test", bug.regression_test},
                        {"fixed", bug.fixed}});
    }
    return FileSystem::write_text_file(path, json.dump(2));
}

bool BugTracker::load(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        bugs_.clear();
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            BugReport bug;
            bug.id = entry.value("id", "");
            bug.expected_behavior = entry.value("expected_behavior", "");
            bug.actual_behavior = entry.value("actual_behavior", "");
            bug.affected_platform = entry.value("affected_platform", "");
            bug.fix_owner = entry.value("fix_owner", "");
            bug.regression_test = entry.value("regression_test", "");
            bug.fixed = entry.value("fixed", false);
            if (entry.contains("reproduction_steps")) {
                for (const auto& step : entry["reproduction_steps"]) {
                    bug.reproduction_steps.push_back(step.get<std::string>());
                }
            }
            bugs_.push_back(std::move(bug));
        }
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

bool BugTracker::save_known_issues(std::string_view path) const {
    nlohmann::json json;
    json["open_count"] = open_count();
    json["issues"] = nlohmann::json::array();
    for (const BugReport& bug : bugs_) {
        if (!bug.fixed) {
            json["issues"].push_back({{"id", bug.id},
                                      {"severity", severity_string(bug.severity)},
                                      {"summary", bug.actual_behavior},
                                      {"platform", bug.affected_platform}});
        }
    }
    return FileSystem::write_text_file(path, json.dump(2));
}

} // namespace eve::polish
