#pragma once

#include <eve/content/apartment_content.hpp>
#include <eve/content/character_content.hpp>
#include <eve/content/content_database.hpp>
#include <eve/content/dialogue_content.hpp>
#include <eve/content/hot_reload.hpp>
#include <eve/content/localization.hpp>
#include <eve/content/mod_system.hpp>
#include <eve/content/packaging.hpp>
#include <eve/content/project_layout.hpp>
#include <eve/content/undo_stack.hpp>
#include <eve/content/wardrobe_database.hpp>

namespace eve::content {

class GameContentProject {
public:
    bool open(std::string_view game_root);
    bool create_new(std::string_view game_root, std::string_view title);
    bool save_all() const;
    bool load_all();
    bool export_build(const PackagingConfig& config) const;

    [[nodiscard]] ProjectLayout& layout() { return layout_; }
    [[nodiscard]] ContentDatabase& database() { return database_; }
    [[nodiscard]] CharacterContentLibrary& characters() { return characters_; }
    [[nodiscard]] WardrobeDatabase& wardrobe() { return wardrobe_; }
    [[nodiscard]] ApartmentContentLibrary& apartments() { return apartments_; }
    [[nodiscard]] DialogueContentLibrary& dialogue() { return dialogue_; }
    [[nodiscard]] EventContentLibrary& events() { return events_; }
    [[nodiscard]] LocalizationTable& localization() { return localization_; }
    [[nodiscard]] ModSystem& mods() { return mods_; }
    [[nodiscard]] UndoStack& undo() { return undo_; }
    [[nodiscard]] HotReloadManager& hot_reload() { return hot_reload_; }

private:
    ProjectLayout layout_;
    ContentDatabase database_;
    CharacterContentLibrary characters_;
    WardrobeDatabase wardrobe_;
    ApartmentContentLibrary apartments_;
    DialogueContentLibrary dialogue_;
    EventContentLibrary events_;
    LocalizationTable localization_;
    ModSystem mods_;
    UndoStack undo_;
    HotReloadManager hot_reload_;
    ProjectPackager packager_;
};

} // namespace eve::content
