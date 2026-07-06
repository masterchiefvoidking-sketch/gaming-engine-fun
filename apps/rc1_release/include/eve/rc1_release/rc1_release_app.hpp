#pragma once

#include <eve/polish/rc1_release.hpp>

#include <string>

namespace eve::rc1_release {

class RC1ReleaseApp {
public:
    bool initialize(std::string_view data_root, std::string_view game_root);
    polish::RC1ReleaseReport run();
    [[nodiscard]] const polish::RC1ReleaseReport& last_report() const { return last_report_; }

private:
    polish::RC1ReleaseConfig config_;
    polish::RC1ReleasePipeline pipeline_;
    polish::RC1ReleaseReport last_report_;
    bool initialized_ = false;
};

} // namespace eve::rc1_release
