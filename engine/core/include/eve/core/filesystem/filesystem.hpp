#pragma once

#include <eve/core/types.hpp>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

namespace eve {

class FileSystem {
public:
    [[nodiscard]] static bool exists(std::string_view path);
    [[nodiscard]] static bool is_directory(std::string_view path);
    [[nodiscard]] static bool create_directories(std::string_view path);
    [[nodiscard]] static std::vector<std::string> list_directory(std::string_view path);
    [[nodiscard]] static std::string read_text_file(std::string_view path);
    [[nodiscard]] static bool write_text_file(std::string_view path, std::string_view content);
    [[nodiscard]] static std::filesystem::path engine_root();
};

} // namespace eve
