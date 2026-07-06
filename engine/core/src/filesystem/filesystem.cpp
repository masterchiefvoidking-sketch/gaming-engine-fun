#include <eve/core/filesystem/filesystem.hpp>
#include <sstream>

namespace eve {

bool FileSystem::exists(std::string_view path) {
    return std::filesystem::exists(std::filesystem::path(path));
}

bool FileSystem::is_directory(std::string_view path) {
    return std::filesystem::is_directory(std::filesystem::path(path));
}

bool FileSystem::create_directories(std::string_view path) {
    return std::filesystem::create_directories(std::filesystem::path(path));
}

std::vector<std::string> FileSystem::list_directory(std::string_view path) {
    std::vector<std::string> entries;
    if (!is_directory(path)) {
        return entries;
    }
    for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::path(path))) {
        entries.push_back(entry.path().filename().string());
    }
    return entries;
}

std::string FileSystem::read_text_file(std::string_view path) {
    std::ifstream stream(std::filesystem::path(path), std::ios::binary);
    if (!stream) {
        return {};
    }
    std::ostringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}

bool FileSystem::write_text_file(std::string_view path, std::string_view content) {
    std::ofstream stream(std::filesystem::path(path), std::ios::binary | std::ios::trunc);
    if (!stream) {
        return false;
    }
    stream.write(content.data(), static_cast<std::streamsize>(content.size()));
    return stream.good();
}

std::filesystem::path FileSystem::engine_root() {
    return std::filesystem::current_path();
}

} // namespace eve
