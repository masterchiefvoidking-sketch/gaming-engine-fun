#pragma once

#include <eve/core/types.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::content {

struct HotReloadWatch {
    std::string path;
    u64 last_modified = 0;
};

class HotReloadManager {
public:
    void watch(std::string_view path);
    [[nodiscard]] std::vector<std::string> poll_changed_files();
    void mark_reloaded(std::string_view path);

private:
    std::unordered_map<std::string, HotReloadWatch> watches_;
};

} // namespace eve::content
