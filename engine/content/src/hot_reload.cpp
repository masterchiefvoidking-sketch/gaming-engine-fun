#include <eve/content/hot_reload.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <filesystem>

namespace eve::content {

void HotReloadManager::watch(std::string_view path) {
    HotReloadWatch watch;
    watch.path = std::string(path);
    if (FileSystem::exists(path)) {
        watch.last_modified =
            static_cast<u64>(std::filesystem::last_write_time(watch.path).time_since_epoch().count());
    }
    watches_[watch.path] = watch;
}

std::vector<std::string> HotReloadManager::poll_changed_files() {
    std::vector<std::string> changed;
    for (auto& [path, watch] : watches_) {
        if (!FileSystem::exists(path)) {
            continue;
        }
        const u64 modified =
            static_cast<u64>(std::filesystem::last_write_time(path).time_since_epoch().count());
        if (modified > watch.last_modified) {
            changed.push_back(path);
        }
    }
    return changed;
}

void HotReloadManager::mark_reloaded(std::string_view path) {
    const auto it = watches_.find(std::string(path));
    if (it == watches_.end()) {
        return;
    }
    it->second.last_modified =
        static_cast<u64>(std::filesystem::last_write_time(it->second.path).time_since_epoch().count());
}

} // namespace eve::content
