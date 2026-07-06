#pragma once

#include <eve/platform/unified_save.hpp>

#include <string>

namespace eve::interaction {

enum class StorageBackend : u8 {
    LocalFile = 0,
    BrowserIndexedDb,
    MobileDevice,
};

class ISaveStorage {
public:
    virtual ~ISaveStorage() = default;
    virtual bool write(std::string_view slot, std::string_view content) = 0;
    virtual bool read(std::string_view slot, std::string& content) = 0;
    virtual bool exists(std::string_view slot) const = 0;
    [[nodiscard]] virtual StorageBackend backend() const = 0;
};

class LocalFileStorage : public ISaveStorage {
public:
    explicit LocalFileStorage(std::string root_dir);
    bool write(std::string_view slot, std::string_view content) override;
    bool read(std::string_view slot, std::string& content) override;
    bool exists(std::string_view slot) const override;
    [[nodiscard]] StorageBackend backend() const override { return StorageBackend::LocalFile; }

private:
    std::string path_for(std::string_view slot) const;
    std::string root_dir_;
};

class BrowserIndexedDbStorage : public ISaveStorage {
public:
    explicit BrowserIndexedDbStorage(std::string cache_dir);
    bool write(std::string_view slot, std::string_view content) override;
    bool read(std::string_view slot, std::string& content) override;
    bool exists(std::string_view slot) const override;
    [[nodiscard]] StorageBackend backend() const override { return StorageBackend::BrowserIndexedDb; }

private:
    std::string path_for(std::string_view slot) const;
    std::string cache_dir_;
};

class SaveStorageRouter {
public:
    void set_desktop_storage(LocalFileStorage storage);
    void set_browser_storage(BrowserIndexedDbStorage storage);
    void set_active_backend(StorageBackend backend);

    bool save_game(const platform::UnifiedSaveSystem& system, const platform::UnifiedSaveGame& game,
                   std::string_view slot);
    bool load_game(const platform::UnifiedSaveSystem& system, platform::UnifiedSaveGame& game,
                   std::string_view slot);
    [[nodiscard]] StorageBackend active_backend() const { return active_; }

private:
    ISaveStorage* active_storage();
    LocalFileStorage desktop_{""};
    BrowserIndexedDbStorage browser_{""};
    StorageBackend active_ = StorageBackend::LocalFile;
    bool has_desktop_ = false;
    bool has_browser_ = false;
};

} // namespace eve::interaction
