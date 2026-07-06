#pragma once

#include <eve/core/types.hpp>

#include <deque>
#include <string>
#include <vector>

namespace eve::polish {

struct FrameTimingSample {
    f32 total_ms = 0.0f;
    f32 cpu_ms = 0.0f;
    f32 gpu_ms = 0.0f;
    f32 animation_ms = 0.0f;
    f32 hair_sim_ms = 0.0f;
    f32 cloth_sim_ms = 0.0f;
    u32 draw_calls = 0;
};

struct PerformanceSnapshot {
    f32 fps = 0.0f;
    f32 frame_time_ms = 0.0f;
    f32 cpu_time_ms = 0.0f;
    f32 gpu_time_ms = 0.0f;
    u64 memory_bytes = 0;
    u64 texture_memory_bytes = 0;
    u32 draw_calls = 0;
    f32 animation_cost_ms = 0.0f;
    f32 hair_sim_cost_ms = 0.0f;
    f32 cloth_sim_cost_ms = 0.0f;
    f32 asset_load_ms = 0.0f;
    u32 frame_cap = 144;
    bool within_budget = true;
};

struct PerformanceBudget {
    f32 target_fps = 60.0f;
    f32 max_frame_time_ms = 16.67f;
    f32 max_memory_mb = 2048.0f;
    u32 max_draw_calls = 5000;
};

class PerformanceProfiler {
public:
    void begin_frame();
    void end_frame(f32 delta_seconds);
    void record_cpu_time(f32 ms);
    void record_gpu_time(f32 ms);
    void record_animation_cost(f32 ms);
    void record_hair_sim_cost(f32 ms);
    void record_cloth_sim_cost(f32 ms);
    void record_draw_calls(u32 count);
    void record_texture_memory(u64 bytes);
    void record_asset_load_time(f32 ms);
    void set_memory_usage(u64 bytes);
    void set_frame_cap(u32 cap);
    void set_budget(const PerformanceBudget& budget);

    [[nodiscard]] PerformanceSnapshot snapshot() const;
    [[nodiscard]] const std::deque<f32>& frame_time_history() const { return frame_times_; }
    [[nodiscard]] bool check_budget() const;
    [[nodiscard]] std::string format_overlay() const;
    bool save_report(std::string_view path) const;

private:
    void update_fps(f32 delta_seconds);

    std::deque<f32> frame_times_;
    FrameTimingSample current_{};
    PerformanceBudget budget_{};
    f32 fps_ = 0.0f;
    f32 fps_accum_ = 0.0f;
    u32 fps_frames_ = 0;
    u64 memory_bytes_ = 0;
    u64 texture_memory_bytes_ = 0;
    f32 asset_load_ms_ = 0.0f;
    u32 frame_cap_ = 144;
    static constexpr std::size_t kHistorySize = 120;
};

} // namespace eve::polish
