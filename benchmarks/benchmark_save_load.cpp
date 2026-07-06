#include <eve/platform/unified_save.hpp>

#include <chrono>
#include <iostream>

int main() {
    constexpr eve::u32 kIterations = 1000;
    eve::platform::UnifiedSaveSystem save_system;
    eve::platform::UnifiedSaveGame save;
    save.project_id = "benchmark";
    save.character.character_id = "mira";

    const auto save_start = std::chrono::steady_clock::now();
    for (eve::u32 i = 0; i < kIterations; ++i) {
        save.save_slot = "slot_" + std::to_string(i % 10);
        if (!save_system.save("/tmp/eve_bench_save.json", save)) {
            return 1;
        }
    }
    const auto save_ms = std::chrono::duration<double, std::milli>(
                             std::chrono::steady_clock::now() - save_start)
                             .count();

    eve::platform::UnifiedSaveGame loaded;
    const auto load_start = std::chrono::steady_clock::now();
    for (eve::u32 i = 0; i < kIterations; ++i) {
        if (!save_system.load("/tmp/eve_bench_save.json", loaded)) {
            return 1;
        }
    }
    const auto load_ms = std::chrono::duration<double, std::milli>(
                             std::chrono::steady_clock::now() - load_start)
                             .count();

    std::cout << "Save/load benchmark (" << kIterations << " iterations)\n";
    std::cout << "Save total: " << save_ms << " ms (" << save_ms / kIterations << " ms/op)\n";
    std::cout << "Load total: " << load_ms << " ms (" << load_ms / kIterations << " ms/op)\n";
    return 0;
}
