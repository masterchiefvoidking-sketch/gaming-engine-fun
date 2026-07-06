#include <eve/interaction/browser_storage.hpp>

#include <eve/core/filesystem/filesystem.hpp>

#include <filesystem>

namespace eve::interaction {

LocalFileStorage::LocalFileStorage(std::string root_dir) : root_dir_(std::move(root_dir)) {}

std::string LocalFileStorage::path_for(std::string_view slot) const {
    return root_dir_ + "/" + std::string(slot) + ".json";
}

bool LocalFileStorage::write(std::string_view slot, std::string_view content) {
    FileSystem::create_directories(root_dir_);
    return FileSystem::write_text_file(path_for(slot), content);
}

bool LocalFileStorage::read(std::string_view slot, std::string& content) {
    content = FileSystem::read_text_file(path_for(slot));
    return !content.empty();
}

bool LocalFileStorage::exists(std::string_view slot) const {
    return FileSystem::exists(path_for(slot));
}

BrowserIndexedDbStorage::BrowserIndexedDbStorage(std::string cache_dir)
    : cache_dir_(std::move(cache_dir)) {}

std::string BrowserIndexedDbStorage::path_for(std::string_view slot) const {
    return cache_dir_ + "/indexeddb/" + std::string(slot) + ".json";
}

bool BrowserIndexedDbStorage::write(std::string_view slot, std::string_view content) {
    const std::string path = path_for(slot);
    FileSystem::create_directories(cache_dir_ + "/indexeddb");
    return FileSystem::write_text_file(path, content);
}

bool BrowserIndexedDbStorage::read(std::string_view slot, std::string& content) {
    content = FileSystem::read_text_file(path_for(slot));
    return !content.empty();
}

bool BrowserIndexedDbStorage::exists(std::string_view slot) const {
    return FileSystem::exists(path_for(slot));
}

void SaveStorageRouter::set_desktop_storage(LocalFileStorage storage) {
    desktop_ = std::move(storage);
    has_desktop_ = true;
}

void SaveStorageRouter::set_browser_storage(BrowserIndexedDbStorage storage) {
    browser_ = std::move(storage);
    has_browser_ = true;
}

void SaveStorageRouter::set_active_backend(StorageBackend backend) {
    active_ = backend;
}

ISaveStorage* SaveStorageRouter::active_storage() {
    if (active_ == StorageBackend::BrowserIndexedDb && has_browser_) {
        return &browser_;
    }
    if (has_desktop_) {
        return &desktop_;
    }
    if (has_browser_) {
        return &browser_;
    }
    return nullptr;
}

bool SaveStorageRouter::save_game(const platform::UnifiedSaveSystem& system,
                                  const platform::UnifiedSaveGame& game, std::string_view slot) {
    ISaveStorage* storage = active_storage();
    if (storage == nullptr) {
        return false;
    }
    const std::filesystem::path temp = std::filesystem::temp_directory_path() / "eve_save_buffer.json";
    if (!system.save(temp.string(), game)) {
        return false;
    }
    const std::string content = FileSystem::read_text_file(temp.string());
    std::filesystem::remove(temp);
    return storage->write(slot, content);
}

bool SaveStorageRouter::load_game(const platform::UnifiedSaveSystem& system,
                                  platform::UnifiedSaveGame& game, std::string_view slot) {
    ISaveStorage* storage = active_storage();
    if (storage == nullptr) {
        return false;
    }
    std::string content;
    if (!storage->read(slot, content)) {
        return false;
    }
    const std::filesystem::path temp = std::filesystem::temp_directory_path() / "eve_save_buffer.json";
    if (!FileSystem::write_text_file(temp.string(), content)) {
        return false;
    }
    const bool ok = system.load(temp.string(), game);
    std::filesystem::remove(temp);
    return ok;
}

} // namespace eve::interaction
