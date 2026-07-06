#pragma once

#include <eve/product/personal_product_cut.hpp>

#include <string>

namespace eve::personal_product_cut {

class PersonalProductCutApp {
public:
    bool initialize(std::string_view data_root, std::string_view game_root,
                    std::string_view releases_root);
    product::PersonalProductReport run();
    [[nodiscard]] const product::PersonalProductReport& last_report() const { return last_report_; }

private:
    product::PersonalProductConfig config_;
    product::PersonalProductCut pipeline_;
    product::PersonalProductReport last_report_;
    bool initialized_ = false;
};

} // namespace eve::personal_product_cut
