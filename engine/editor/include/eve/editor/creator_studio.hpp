#pragma once

#include <eve/content/game_content_project.hpp>
#include <eve/content/asset_import.hpp>
#include <eve/editor/apartment_builder.hpp>

#include <string>
#include <utility>
#include <vector>

namespace eve::editor {

struct CreatorStudioConfig {
    std::string project_root;
    std::string title = "Untitled Game";
    bool dark_theme = true;
    bool autosave_enabled = true;
    f32 autosave_interval_seconds = 120.0f;
};

class CharacterEditorPanel {
public:
    void set_project(content::GameContentProject& project);
    bool create_character(std::string_view id, std::string_view name);
    [[nodiscard]] std::size_t character_count() const { return character_count_; }

private:
    content::GameContentProject* project_ = nullptr;
    std::size_t character_count_ = 0;
};

class WardrobeEditorPanel {
public:
    void set_database(content::WardrobeDatabase& wardrobe);
    bool add_item(const content::ClothingItemDefinition& item);
    [[nodiscard]] std::size_t item_count() const { return item_count_; }

private:
    content::WardrobeDatabase* wardrobe_ = nullptr;
    std::size_t item_count_ = 0;
};

class DialogueGraphEditor {
public:
    void set_library(content::DialogueContentLibrary& dialogue);
    bool add_node(std::string_view package_id, const content::DialogueNodeContent& node);
    [[nodiscard]] std::size_t package_count() const { return package_count_; }

private:
    content::DialogueContentLibrary* dialogue_ = nullptr;
    std::size_t package_count_ = 0;
};

class EventGraphEditor {
public:
    void set_library(content::EventContentLibrary& events);
    bool add_node(std::string_view event_id, const content::EventGraphNode& node);
    [[nodiscard]] std::size_t event_count() const { return event_count_; }

private:
    content::EventContentLibrary* events_ = nullptr;
    std::size_t event_count_ = 0;
};

struct TimelineTrack {
    std::string name;
    std::string type;
    std::vector<std::pair<f32, std::string>> keyframes;
};

class TimelineEditor {
public:
    void add_track(TimelineTrack track);
    [[nodiscard]] std::size_t track_count() const { return tracks_.size(); }
    [[nodiscard]] const std::vector<TimelineTrack>& tracks() const { return tracks_; }

private:
    std::vector<TimelineTrack> tracks_;
};

struct PhotoStudioSettings {
    std::string background_id;
    std::string pose_id;
    std::string expression_id;
    std::string camera_preset_id;
    f32 depth_of_field = 0.5f;
    bool transparent_background = false;
    std::string filter_id;
};

class PhotoStudioPanel {
public:
    void set_settings(const PhotoStudioSettings& settings) { settings_ = settings; }
    [[nodiscard]] const PhotoStudioSettings& settings() const { return settings_; }
    bool capture_screenshot(std::string_view path) const;
    bool batch_capture(const std::vector<std::string>& paths) const;

private:
    PhotoStudioSettings settings_;
};

class AssetBrowserPanel {
public:
    void set_database(content::ContentDatabase& database);
    [[nodiscard]] std::vector<content::ContentSearchResult> search(std::string_view query) const;
    [[nodiscard]] std::size_t asset_count() const { return asset_count_; }

private:
    content::ContentDatabase* database_ = nullptr;
    std::size_t asset_count_ = 0;
};

class CreatorStudio {
public:
    bool initialize(const CreatorStudioConfig& config);
    bool open_project(std::string_view path);
    bool create_project(std::string_view path, std::string_view title);
    bool save_project();
    bool export_game(content::ExportTarget target);
    void update(f32 delta_seconds);

    [[nodiscard]] content::GameContentProject& project() { return project_; }
    [[nodiscard]] ApartmentBuilder& apartment_builder() { return apartment_builder_; }
    CharacterEditorPanel& character_editor() { return character_editor_; }
    WardrobeEditorPanel& wardrobe_editor() { return wardrobe_editor_; }
    DialogueGraphEditor& dialogue_editor() { return dialogue_editor_; }
    EventGraphEditor& event_editor() { return event_editor_; }
    TimelineEditor& timeline() { return timeline_; }
    PhotoStudioPanel& photo_studio() { return photo_studio_; }
    AssetBrowserPanel& asset_browser() { return asset_browser_; }
    content::AssetImporter& importer() { return importer_; }

private:
    void setup_panels();
    void poll_hot_reload();
    void tick_autosave(f32 delta_seconds);

    CreatorStudioConfig config_;
    content::GameContentProject project_;
    content::AssetImporter importer_;
    ApartmentBuilder apartment_builder_;
    CharacterEditorPanel character_editor_;
    WardrobeEditorPanel wardrobe_editor_;
    DialogueGraphEditor dialogue_editor_;
    EventGraphEditor event_editor_;
    TimelineEditor timeline_;
    PhotoStudioPanel photo_studio_;
    AssetBrowserPanel asset_browser_;
    f32 autosave_timer_ = 0.0f;
    bool initialized_ = false;
};

} // namespace eve::editor
