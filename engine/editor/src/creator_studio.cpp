#include <eve/editor/creator_studio.hpp>
#include <eve/core/filesystem/filesystem.hpp>
#include <eve/core/logging/logger.hpp>

#include <nlohmann/json.hpp>

namespace eve::editor {

bool CharacterEditorPanel::create_character(std::string_view id, std::string_view name) {
    if (project_ == nullptr) {
        return false;
    }
    content::CharacterContentDefinition def;
    def.metadata.id = std::string(id);
    def.metadata.display_name = std::string(name);
    project_->characters().register_character(std::move(def));
    character_count_ = project_->characters().character_count();
    return true;
}

void CharacterEditorPanel::set_project(content::GameContentProject& project) {
    project_ = &project;
    character_count_ = project.characters().character_count();
}

bool WardrobeEditorPanel::add_item(const content::ClothingItemDefinition& item) {
    if (wardrobe_ == nullptr) {
        return false;
    }
    wardrobe_->register_item(item);
    item_count_ = wardrobe_->item_count();
    return true;
}

void WardrobeEditorPanel::set_database(content::WardrobeDatabase& wardrobe) {
    wardrobe_ = &wardrobe;
    item_count_ = wardrobe.item_count();
}

bool DialogueGraphEditor::add_node(std::string_view package_id,
                                   const content::DialogueNodeContent& node) {
    if (dialogue_ == nullptr) {
        return false;
    }
    content::DialoguePackage* pkg = dialogue_->find(package_id);
    if (pkg == nullptr) {
        content::DialoguePackage new_pkg;
        new_pkg.metadata.id = std::string(package_id);
        new_pkg.nodes.push_back(node);
        dialogue_->register_package(std::move(new_pkg));
        package_count_ = dialogue_->package_count();
        return true;
    }
    pkg->nodes.push_back(node);
    return true;
}

void DialogueGraphEditor::set_library(content::DialogueContentLibrary& dialogue) {
    dialogue_ = &dialogue;
    package_count_ = dialogue.package_count();
}

bool EventGraphEditor::add_node(std::string_view event_id, const content::EventGraphNode& node) {
    if (events_ == nullptr) {
        return false;
    }
    content::EventContentDefinition* evt = events_->find(event_id);
    if (evt == nullptr) {
        return false;
    }
    evt->nodes.push_back(node);
    return true;
}

void EventGraphEditor::set_library(content::EventContentLibrary& events) {
    events_ = &events;
    event_count_ = events.event_count();
}

void TimelineEditor::add_track(TimelineTrack track) {
    tracks_.push_back(std::move(track));
}

bool PhotoStudioPanel::capture_screenshot(std::string_view path) const {
    nlohmann::json meta = {{"background", settings_.background_id},
                           {"pose", settings_.pose_id},
                           {"expression", settings_.expression_id},
                           {"camera", settings_.camera_preset_id},
                           {"dof", settings_.depth_of_field},
                           {"transparent", settings_.transparent_background}};
    return FileSystem::write_text_file(path, meta.dump(2));
}

bool PhotoStudioPanel::batch_capture(const std::vector<std::string>& paths) const {
    bool ok = true;
    for (const std::string& path : paths) {
        ok = capture_screenshot(path) && ok;
    }
    return ok;
}

void AssetBrowserPanel::set_database(content::ContentDatabase& database) {
    database_ = &database;
    asset_count_ = database.asset_count();
}

std::vector<content::ContentSearchResult> AssetBrowserPanel::search(
    std::string_view query) const {
    if (database_ == nullptr) {
        return {};
    }
    return database_->search(query);
}

bool CreatorStudio::initialize(const CreatorStudioConfig& config) {
    config_ = config;
    setup_panels();
    initialized_ = true;
    EVE_LOG(Info, "CreatorStudio", "Creator Studio initialized");
    return true;
}

bool CreatorStudio::open_project(std::string_view path) {
    if (!project_.open(path)) {
        return false;
    }
    setup_panels();
    EVE_LOG(Info, "CreatorStudio", "Opened project: ", path);
    return true;
}

bool CreatorStudio::create_project(std::string_view path, std::string_view title) {
    if (!project_.create_new(path, title)) {
        return false;
    }
    setup_panels();
    return true;
}

bool CreatorStudio::save_project() {
    return project_.save_all();
}

bool CreatorStudio::export_game(content::ExportTarget target) {
    content::PackagingConfig config;
    config.project_name = config_.title;
    config.output_dir = project_.layout().paths().root + "/Build";
    config.targets = {target};
    return project_.export_build(config);
}

void CreatorStudio::update(f32 delta_seconds) {
    poll_hot_reload();
    tick_autosave(delta_seconds);
}

void CreatorStudio::setup_panels() {
    character_editor_.set_project(project_);
    wardrobe_editor_.set_database(project_.wardrobe());
    dialogue_editor_.set_library(project_.dialogue());
    event_editor_.set_library(project_.events());
    asset_browser_.set_database(project_.database());
}

void CreatorStudio::poll_hot_reload() {
    const std::vector<std::string> changed = project_.hot_reload().poll_changed_files();
    for (const std::string& path : changed) {
        EVE_LOG(Info, "CreatorStudio", "Hot reload: ", path);
        project_.load_all();
        project_.hot_reload().mark_reloaded(path);
    }
}

void CreatorStudio::tick_autosave(f32 delta_seconds) {
    if (!config_.autosave_enabled) {
        return;
    }
    autosave_timer_ += delta_seconds;
    if (autosave_timer_ >= config_.autosave_interval_seconds) {
        save_project();
        autosave_timer_ = 0.0f;
    }
}

} // namespace eve::editor
