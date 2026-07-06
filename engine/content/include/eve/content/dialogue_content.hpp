#pragma once

#include <eve/content/content_types.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::content {

struct DialogueNodeContent {
    std::string id;
    std::string speaker;
    std::string line;
    std::vector<std::string> conditions;
    std::vector<std::string> choice_ids;
    std::string expression_id;
    std::string animation_id;
    std::string camera_id;
    std::string music_cue_id;
};

struct DialoguePackage {
    ContentMetadata metadata;
    std::vector<DialogueNodeContent> nodes;
};

struct EventGraphNode {
    std::string id;
    std::string type; // start, condition, dialogue, animation, camera, end, etc.
    std::string target_id;
    std::vector<std::string> outputs;
};

struct EventContentDefinition {
    ContentMetadata metadata;
    std::vector<EventGraphNode> nodes;
};

class DialogueContentLibrary {
public:
    bool load_package(std::string_view path);
    void register_package(DialoguePackage pkg);
    [[nodiscard]] DialoguePackage* find(std::string_view id);
    [[nodiscard]] std::size_t package_count() const { return packages_.size(); }

private:
    std::unordered_map<std::string, DialoguePackage> packages_;
};

class EventContentLibrary {
public:
    bool load_event(std::string_view path);
    [[nodiscard]] EventContentDefinition* find(std::string_view id);
    [[nodiscard]] std::size_t event_count() const { return events_.size(); }

private:
    std::unordered_map<std::string, EventContentDefinition> events_;
};

} // namespace eve::content
