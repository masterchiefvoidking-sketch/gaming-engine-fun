#include <eve/product/bug_triage.hpp>

#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::product {

void BugTriage::add_bug(TriagedBug bug) {
    bugs_.push_back(std::move(bug));
}

bool BugTriage::mark_fixed(std::string_view bug_id, std::string_view regression_test) {
    for (TriagedBug& bug : bugs_) {
        if (bug.id == bug_id) {
            bug.fixed = true;
            bug.regression_test = std::string(regression_test);
            return true;
        }
    }
    return false;
}

BugTriageReport BugTriage::evaluate() const {
    BugTriageReport report;
    for (const TriagedBug& bug : bugs_) {
        if (bug.fixed) {
            continue;
        }
        switch (bug.priority) {
        case BugPriority::P0:
            ++report.p0_count;
            break;
        case BugPriority::P1:
            ++report.p1_count;
            break;
        case BugPriority::P2:
            ++report.p2_count;
            break;
        case BugPriority::P3:
            ++report.p3_count;
            break;
        }
        report.bugs.push_back(bug);
    }
    report.release_allowed = (report.p0_count == 0 && report.p1_count == 0);
    return report;
}

bool BugTriage::save_known_issues(std::string_view path) const {
    const BugTriageReport report = evaluate();
    nlohmann::json json;
    json["release_allowed"] = report.release_allowed;
    json["p0"] = report.p0_count;
    json["p1"] = report.p1_count;
    json["p2"] = report.p2_count;
    json["p3"] = report.p3_count;
    json["issues"] = nlohmann::json::array();
    for (const TriagedBug& bug : report.bugs) {
        json["issues"].push_back({{"id", bug.id},
                                  {"priority", static_cast<int>(bug.priority)},
                                  {"summary", bug.summary},
                                  {"workaround", bug.workaround}});
    }
    return FileSystem::write_text_file(path, json.dump(2));
}

bool BugTriage::load(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return true;
    }
    try {
        bugs_.clear();
        const auto json = nlohmann::json::parse(content);
        if (!json.contains("issues")) {
            return true;
        }
        for (const auto& entry : json["issues"]) {
            TriagedBug bug;
            bug.id = entry.value("id", "");
            bug.summary = entry.value("summary", "");
            bug.workaround = entry.value("workaround", "");
            bug.priority = static_cast<BugPriority>(entry.value("priority", 3));
            bugs_.push_back(std::move(bug));
        }
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

} // namespace eve::product
