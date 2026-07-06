#pragma once

#include <eve/core/types.hpp>

#include <string>
#include <vector>

namespace eve::product {

enum class ChecklistCategory : u8 {
    Build = 0,
    Runtime,
    Editor,
    Data,
};

struct ChecklistItem {
    ChecklistCategory category;
    std::string id;
    std::string label;
    bool passed = false;
    std::string detail;
};

struct ReleaseChecklistReport {
    bool passed = false;
    u32 passed_count = 0;
    u32 failed_count = 0;
    std::vector<ChecklistItem> items;
};

struct ReleaseChecklistConfig {
    std::string data_root;
    std::string game_root;
    std::string releases_root;
};

class ReleaseChecklist {
public:
    ReleaseChecklistReport run_all(const ReleaseChecklistConfig& config) const;
    bool save_report(std::string_view path, const ReleaseChecklistReport& report) const;
};

} // namespace eve::product
