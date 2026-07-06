#include <chrono>
#include <eve/human/character_profile.hpp>
#include <iostream>

int main() {
    eve::human::CharacterCreator creator;
    const auto start = std::chrono::steady_clock::now();
    constexpr int iterations = 10000;
    for (int i = 0; i < iterations; ++i) {
        creator.apply_body_preset(static_cast<eve::human::BodyTypePreset>(i % 5));
        creator.set_morph("smile", static_cast<eve::f32>(i % 100) / 100.0f);
        (void)creator.compute_body_transform();
    }
    const auto end = std::chrono::steady_clock::now();
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Character customization benchmark: " << iterations << " iterations in " << ms
              << " ms\n";
    return 0;
}
