#include <eve/polish/performance_profiler.hpp>

#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>
#include <sstream>

namespace eve::polish {

void PerformanceProfiler::begin_frame() {
    current_ = {};
}

void PerformanceProfiler::end_frame(f32 delta_seconds) {
    current_.total_ms = delta_seconds * 1000.0f;
    frame_times_.push_back(current_.total_ms);
    if (frame_times_.size() > kHistorySize) {
        frame_times_.pop_front();
    }
    update_fps(delta_seconds);
}

void PerformanceProfiler::record_cpu_time(f32 ms) {
    current_.cpu_ms = ms;
}

void PerformanceProfiler::record_gpu_time(f32 ms) {
    current_.gpu_ms = ms;
}

void PerformanceProfiler::record_animation_cost(f32 ms) {
    current_.animation_ms = ms;
}

void PerformanceProfiler::record_hair_sim_cost(f32 ms) {
    current_.hair_sim_ms = ms;
}

void PerformanceProfiler::record_cloth_sim_cost(f32 ms) {
    current_.cloth_sim_ms = ms;
}

void PerformanceProfiler::record_draw_calls(u32 count) {
    current_.draw_calls = count;
}

void PerformanceProfiler::record_texture_memory(u64 bytes) {
    texture_memory_bytes_ = bytes;
}

void PerformanceProfiler::record_asset_load_time(f32 ms) {
    asset_load_ms_ = ms;
}

void PerformanceProfiler::set_memory_usage(u64 bytes) {
    memory_bytes_ = bytes;
}

void PerformanceProfiler::set_frame_cap(u32 cap) {
    frame_cap_ = cap;
}

void PerformanceProfiler::set_budget(const PerformanceBudget& budget) {
    budget_ = budget;
}

void PerformanceProfiler::update_fps(f32 delta_seconds) {
    fps_accum_ += delta_seconds;
    ++fps_frames_;
    if (fps_accum_ >= 0.5f) {
        fps_ = static_cast<f32>(fps_frames_) / fps_accum_;
        fps_frames_ = 0;
        fps_accum_ = 0.0f;
    }
}

PerformanceSnapshot PerformanceProfiler::snapshot() const {
    PerformanceSnapshot snap;
    snap.fps = fps_ > 0.0f ? fps_
                           : (current_.total_ms > 0.0f ? 1000.0f / current_.total_ms : 0.0f);
    snap.frame_time_ms = current_.total_ms;
    snap.cpu_time_ms = current_.cpu_ms;
    snap.gpu_time_ms = current_.gpu_ms;
    snap.memory_bytes = memory_bytes_;
    snap.texture_memory_bytes = texture_memory_bytes_;
    snap.draw_calls = current_.draw_calls;
    snap.animation_cost_ms = current_.animation_ms;
    snap.hair_sim_cost_ms = current_.hair_sim_ms;
    snap.cloth_sim_cost_ms = current_.cloth_sim_ms;
    snap.asset_load_ms = asset_load_ms_;
    snap.frame_cap = frame_cap_;
    snap.within_budget = check_budget();
    return snap;
}

bool PerformanceProfiler::check_budget() const {
    if (current_.total_ms > budget_.max_frame_time_ms) {
        return false;
    }
    if (current_.draw_calls > budget_.max_draw_calls) {
        return false;
    }
    const f32 memory_mb = static_cast<f32>(memory_bytes_) / (1024.0f * 1024.0f);
    if (memory_mb > budget_.max_memory_mb) {
        return false;
    }
    return fps_ >= budget_.target_fps || fps_ == 0.0f;
}

std::string PerformanceProfiler::format_overlay() const {
    std::ostringstream out;
    out << "FPS: " << static_cast<int>(fps_) << "\n";
    out << "Frame: " << current_.total_ms << " ms\n";
    out << "CPU: " << current_.cpu_ms << " ms  GPU: " << current_.gpu_ms << " ms\n";
    out << "Draw: " << current_.draw_calls << "\n";
    out << "Anim: " << current_.animation_ms << " ms  Hair: " << current_.hair_sim_ms
        << " ms  Cloth: " << current_.cloth_sim_ms << " ms\n";
    out << "Mem: " << (memory_bytes_ / (1024 * 1024)) << " MB\n";
    out << "Budget: " << (check_budget() ? "OK" : "OVER");
    return out.str();
}

bool PerformanceProfiler::save_report(std::string_view path) const {
    const PerformanceSnapshot snap = snapshot();
    nlohmann::json json;
    json["fps"] = snap.fps;
    json["frame_time_ms"] = snap.frame_time_ms;
    json["cpu_time_ms"] = snap.cpu_time_ms;
    json["gpu_time_ms"] = snap.gpu_time_ms;
    json["memory_bytes"] = snap.memory_bytes;
    json["texture_memory_bytes"] = snap.texture_memory_bytes;
    json["draw_calls"] = snap.draw_calls;
    json["animation_cost_ms"] = snap.animation_cost_ms;
    json["hair_sim_cost_ms"] = snap.hair_sim_cost_ms;
    json["cloth_sim_cost_ms"] = snap.cloth_sim_cost_ms;
    json["asset_load_ms"] = snap.asset_load_ms;
    json["within_budget"] = snap.within_budget;
    json["frame_history"] = frame_times_;
    return FileSystem::write_text_file(path, json.dump(2));
}

} // namespace eve::polish
