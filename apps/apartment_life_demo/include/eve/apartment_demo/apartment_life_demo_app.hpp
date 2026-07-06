#pragma once

#include <eve/gameplay/apartment_life_session.hpp>

#include <string>
#include <vector>

namespace eve::apartment_demo {

class ApartmentLifeDemoApp {
public:
    bool initialize(std::string_view data_root, std::string_view game_root);
    void shutdown();
    void update(f32 delta_seconds);

    bool run_vertical_slice();
    [[nodiscard]] gameplay::ApartmentLifeSession& session() { return session_; }
    [[nodiscard]] const std::vector<gameplay::VerticalSliceStep>& last_run() const {
        return last_run_;
    }

private:
    gameplay::ApartmentLifeSession session_;
    std::vector<gameplay::VerticalSliceStep> last_run_;
    bool initialized_ = false;
};

} // namespace eve::apartment_demo
