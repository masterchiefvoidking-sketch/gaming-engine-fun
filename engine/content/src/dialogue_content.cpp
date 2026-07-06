#include <eve/content/dialogue_content.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::content {

bool DialogueContentLibrary::load_package(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        DialoguePackage pkg;
        pkg.metadata.id = json.value("id", "");
        pkg.metadata.display_name = json.value("name", "");
        if (json.contains("nodes")) {
            for (const auto& node_json : json["nodes"]) {
                DialogueNodeContent node;
                node.id = node_json.value("id", "");
                node.speaker = node_json.value("speaker", "");
                node.line = node_json.value("line", "");
                node.expression_id = node_json.value("expression", "");
                node.animation_id = node_json.value("animation", "");
                node.camera_id = node_json.value("camera", "");
                node.music_cue_id = node_json.value("music", "");
                if (node_json.contains("conditions")) {
                    for (const auto& c : node_json["conditions"]) {
                        node.conditions.push_back(c.get<std::string>());
                    }
                }
                pkg.nodes.push_back(std::move(node));
            }
        }
        packages_[pkg.metadata.id] = std::move(pkg);
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

DialoguePackage* DialogueContentLibrary::find(std::string_view id) {
    const auto it = packages_.find(std::string(id));
    return it != packages_.end() ? &it->second : nullptr;
}

void DialogueContentLibrary::register_package(DialoguePackage pkg) {
    packages_[pkg.metadata.id] = std::move(pkg);
}

bool EventContentLibrary::load_event(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        EventContentDefinition def;
        def.metadata.id = json.value("id", "");
        def.metadata.display_name = json.value("name", "");
        if (json.contains("nodes")) {
            for (const auto& node_json : json["nodes"]) {
                EventGraphNode node;
                node.id = node_json.value("id", "");
                node.type = node_json.value("type", "dialogue");
                node.target_id = node_json.value("target", "");
                if (node_json.contains("outputs")) {
                    for (const auto& out : node_json["outputs"]) {
                        node.outputs.push_back(out.get<std::string>());
                    }
                }
                def.nodes.push_back(std::move(node));
            }
        }
        events_[def.metadata.id] = std::move(def);
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

EventContentDefinition* EventContentLibrary::find(std::string_view id) {
    const auto it = events_.find(std::string(id));
    return it != events_.end() ? &it->second : nullptr;
}

} // namespace eve::content
