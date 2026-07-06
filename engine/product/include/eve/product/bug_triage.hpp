#pragma once

#include <eve/core/types.hpp>

#include <string>
#include <vector>

namespace eve::product {

enum class BugPriority : u8 {
    P0 = 0,
    P1,
    P2,
    P3,
};

struct TriagedBug {
    std::string id;
    BugPriority priority = BugPriority::P3;
    std::string summary;
    std::string workaround;
    bool fixed = false;
    std::string regression_test;
};

struct BugTriageReport {
    u32 p0_count = 0;
    u32 p1_count = 0;
    u32 p2_count = 0;
    u32 p3_count = 0;
    std::vector<TriagedBug> bugs;
    bool release_allowed = false;
};

class BugTriage {
public:
    void add_bug(TriagedBug bug);
    bool mark_fixed(std::string_view bug_id, std::string_view regression_test);
    [[nodiscard]] BugTriageReport evaluate() const;
    bool save_known_issues(std::string_view path) const;
    bool load(std::string_view path);

private:
    std::vector<TriagedBug> bugs_;
};

} // namespace eve::product
