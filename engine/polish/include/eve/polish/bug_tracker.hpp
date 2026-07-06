#pragma once

#include <eve/core/types.hpp>

#include <string>
#include <vector>

namespace eve::polish {

enum class BugSeverity : u8 {
    Critical = 0,
    High,
    Medium,
    Low,
};

struct BugReport {
    std::string id;
    BugSeverity severity = BugSeverity::Medium;
    std::vector<std::string> reproduction_steps;
    std::string expected_behavior;
    std::string actual_behavior;
    std::string affected_platform;
    std::string fix_owner;
    std::string regression_test;
    bool fixed = false;
};

class BugTracker {
public:
    void add_bug(BugReport bug);
    bool mark_fixed(std::string_view bug_id, std::string_view regression_test);
    [[nodiscard]] const std::vector<BugReport>& bugs() const { return bugs_; }
    [[nodiscard]] std::size_t open_count() const;
    bool save(std::string_view path) const;
    bool load(std::string_view path);
    bool save_known_issues(std::string_view path) const;

private:
    std::vector<BugReport> bugs_;
};

} // namespace eve::polish
