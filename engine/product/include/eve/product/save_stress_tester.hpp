#pragma once

#include <eve/gameplay/apartment_life_session.hpp>

#include <string>
#include <vector>

namespace eve::product {

struct SaveStressConfig {
    std::string data_root;
    std::string game_root;
    u32 slot_count = 10;
    u32 cycle_count = 100;
};

struct SaveStressResult {
    bool passed = false;
    u32 cycles_completed = 0;
    u32 slots_tested = 0;
    std::vector<std::string> failures;
};

class SaveStressTester {
public:
    SaveStressResult run(const SaveStressConfig& config);
    bool simulate_corruption_restore(std::string_view save_path,
                                     std::string_view backup_path) const;
};

} // namespace eve::product
