#include <eve/ai/relationship_simulation.hpp>
#include <eve/core/filesystem/filesystem.hpp>
#include <eve/core/logging/logger.hpp>

#include <nlohmann/json.hpp>

namespace eve::ai {

bool RelationshipSimulation::initialize(std::string_view data_root) {
    data_root_ = std::string(data_root);
    const std::string base = data_root_ + "/romance_sim";

    profiles_.load_profile(base + "/character_profile.json");
    profiles_.load_boundaries(base + "/boundaries.json");
    dialogue_.load_from_file(base + "/dialogue.json");
    memories_.load_templates(base + "/memory_templates.json");
    room_behavior_.load_from_file(base + "/room_reactions.json");
    outfit_reactions_.load_from_file(base + "/outfit_reactions.json");
    events_.load_templates(base + "/event_templates.json");

    const std::string schedule_content = FileSystem::read_text_file(base + "/daily_schedule.json");
    if (!schedule_content.empty()) {
        try {
            const auto json = nlohmann::json::parse(schedule_content);
            DailySchedule schedule;
            schedule.character_id = json.value("character_id", "aiko");
            for (const auto& entry : json["entries"]) {
                ScheduleEntry se;
                se.start_hour = entry.value("start", 0.0f);
                se.end_hour = entry.value("end", 0.0f);
                se.room_id = entry.value("room", "");
                se.label = entry.value("label", "");
                schedule.entries.push_back(se);
            }
            schedules_.add_schedule(std::move(schedule));
        } catch (const nlohmann::json::exception&) {
        }
    }

    CharacterSimState* character = profiles_.find("aiko");
    if (character != nullptr) {
        character->consent = ConsentState::Granted;
        character->current_room_id = "living_room";
        character->current_outfit_id = "cozy_sweater";
    }

    RelationshipModel& rel = relationships_.get_or_create(sim_state_.player_id, sim_state_.character_id);
    rel.adjust_scores(0.0f, 0.0f, 0.0f, 0.0f);
    sync_relationship_stage();

    initialized_ = true;
    EVE_LOG(Info, "RomanceSim", "Relationship simulation initialized");
    return true;
}

InteractionResult RelationshipSimulation::talk(std::string_view dialogue_node_id) {
    return execute_dialogue_node(dialogue_node_id);
}

InteractionResult RelationshipSimulation::offer_compliment() {
    CharacterSimState* character = profiles_.find("aiko");
    if (character == nullptr) {
        return {};
    }
    const ConsentCheckResult consent =
        boundaries_.check_action("compliment", *character, current_stage());
    if (!consent.allowed) {
        InteractionResult result;
        result.rejection_reason = consent.rejection_line;
        mood_.apply_rejection(*character);
        return result;
    }
    mood_.apply_compliment(*character);
    RelationshipModel& rel =
        relationships_.get_or_create(sim_state_.player_id, sim_state_.character_id);
    rel.adjust_scores(0.04f, 0.06f, 0.02f, 0.0f);
    rel.add_memory(memories_.create_memory("compliment_received", "player", sim_state_.day_index));

    auto matches = dialogue_.matching_lines(build_dialogue_context(), "compliment");
    InteractionResult result;
    result.success = true;
    result.dialogue_line =
        matches.empty() ? "Thank you... that's really sweet." : matches[0]->line;
    result.memory_created = rel.shared_memories.back();
    sync_relationship_stage();
    result.new_stage = current_stage();
    result.new_mood = mood_.derive_mood(*character);
    return result;
}

InteractionResult RelationshipSimulation::invite_to_dinner() {
    CharacterSimState* character = profiles_.find("aiko");
    if (character == nullptr) {
        return {};
    }
    const ConsentCheckResult consent =
        boundaries_.check_action("invite_dinner", *character, current_stage());
    if (!consent.allowed) {
        InteractionResult result;
        result.rejection_reason = consent.rejection_line;
        return result;
    }
    mood_.apply_shared_meal(*character);
    RelationshipModel& rel =
        relationships_.get_or_create(sim_state_.player_id, sim_state_.character_id);
    rel.adjust_scores(0.06f, 0.05f, 0.03f, 0.0f);

    RomanceEventResult event =
        events_.trigger("first_shared_meal", *character, rel, memories_, mood_);

    InteractionResult result;
    result.success = true;
    auto matches = dialogue_.matching_lines(build_dialogue_context(), "date");
    result.dialogue_line = matches.empty() ? "I'd love to have dinner with you." : matches[0]->line;
    if (event.triggered) {
        result.memory_created = event.memory_created;
    }
    sync_relationship_stage();
    result.new_stage = current_stage();
    result.new_mood = mood_.derive_mood(*character);
    return result;
}

InteractionResult RelationshipSimulation::trigger_outfit_dialogue() {
    CharacterSimState* character = profiles_.find("aiko");
    if (character == nullptr) {
        return {};
    }
    const OutfitReaction* reaction = outfit_reactions_.find_reaction(
        character->current_outfit_id, {"casual", "cozy"}, current_stage());
    InteractionResult result;
    if (reaction == nullptr) {
        result.rejection_reason = "No outfit reaction available";
        return result;
    }
    outfit_reactions_.apply_reaction(*reaction, *character);
    result.success = true;
    result.dialogue_line = reaction->dialogue_line;
    RelationshipModel& rel =
        relationships_.get_or_create(sim_state_.player_id, sim_state_.character_id);
    if (!reaction->memory_tag.empty()) {
        rel.add_memory(memories_.create_memory(reaction->memory_tag, "player", sim_state_.day_index));
        result.memory_created = rel.shared_memories.back();
    }
    sync_relationship_stage();
    result.new_stage = current_stage();
    result.new_mood = mood_.derive_mood(*character);
    return result;
}

InteractionResult RelationshipSimulation::trigger_event(std::string_view event_id) {
    CharacterSimState* character = profiles_.find("aiko");
    if (character == nullptr) {
        return {};
    }
    RelationshipModel& rel =
        relationships_.get_or_create(sim_state_.player_id, sim_state_.character_id);
    RomanceEventResult event = events_.trigger(event_id, *character, rel, memories_, mood_);

    InteractionResult result;
    result.success = event.triggered;
    result.rejection_reason = event.message;
    if (event.triggered) {
        result.memory_created = event.memory_created;
        auto node = dialogue_.evaluate_node(event.dialogue_id, build_dialogue_context());
        if (node.success && node.node != nullptr) {
            result.dialogue_line = node.node->line;
        }
    }
    sync_relationship_stage();
    result.new_stage = current_stage();
    if (character != nullptr) {
        result.new_mood = mood_.derive_mood(*character);
    }
    return result;
}

InteractionResult RelationshipSimulation::make_choice(std::string_view node_id,
                                                      std::string_view choice_id) {
    CharacterSimState* character = profiles_.find("aiko");
    if (character == nullptr) {
        return {};
    }
    const DialogueResult node_result = dialogue_.evaluate_node(node_id, build_dialogue_context());
    if (!node_result.success) {
        InteractionResult result;
        result.rejection_reason = node_result.rejection_reason;
        return result;
    }
    for (const DialogueChoice& choice : node_result.available_choices) {
        if (choice.id == choice_id) {
            dialogue_.apply_choice(choice, *character);
            RelationshipModel& rel =
                relationships_.get_or_create(sim_state_.player_id, sim_state_.character_id);
            rel.adjust_scores(choice.effect.trust_delta, choice.effect.affection_delta, 0.0f, 0.0f);
            if (!choice.effect.memory_tag.empty()) {
                rel.add_memory(memories_.create_memory(choice.effect.memory_tag, "player",
                                                         sim_state_.day_index));
            }
            InteractionResult result;
            result.success = true;
            if (!choice.effect.next_node_id.empty()) {
                return execute_dialogue_node(choice.effect.next_node_id);
            }
            sync_relationship_stage();
            result.new_stage = current_stage();
            result.new_mood = mood_.derive_mood(*character);
            return result;
        }
    }
    return {};
}

bool RelationshipSimulation::save_state(std::string_view path) const {
    const CharacterSimState* character = profiles_.find("aiko");
    const RelationshipModel* rel = relationships_.find(sim_state_.player_id, sim_state_.character_id);
    if (character == nullptr || rel == nullptr) {
        return false;
    }
    nlohmann::json json;
    json["time_of_day"] = sim_state_.time_of_day;
    json["weather"] = sim_state_.weather;
    json["day_index"] = sim_state_.day_index;
    json["character"]["trust"] = character->trust;
    json["character"]["affection"] = character->affection;
    json["character"]["comfort"] = character->comfort;
    json["character"]["mood"] = to_string(character->mood);
    json["character"]["room"] = character->current_room_id;
    json["character"]["outfit"] = character->current_outfit_id;
    json["relationship"]["trust"] = rel->scores.trust;
    json["relationship"]["affection"] = rel->scores.affection;
    json["memories"] = nlohmann::json::array();
    for (const MemoryEntry& mem : rel->shared_memories) {
        nlohmann::json mem_json = {{"id", mem.id},
                                   {"description", mem.description},
                                   {"weight", mem.emotional_weight}};
        mem_json["tags"] = mem.tags;
        json["memories"].push_back(std::move(mem_json));
    }
    return FileSystem::write_text_file(path, json.dump(2));
}

bool RelationshipSimulation::load_state(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        sim_state_.time_of_day = json.value("time_of_day", 18.0f);
        sim_state_.weather = json.value("weather", "rain");
        sim_state_.day_index = json.value("day_index", 0);

        CharacterSimState* character = profiles_.find("aiko");
        RelationshipModel& rel =
            relationships_.get_or_create(sim_state_.player_id, sim_state_.character_id);
        if (character != nullptr && json.contains("character")) {
            character->trust = json["character"].value("trust", character->trust);
            character->affection = json["character"].value("affection", character->affection);
            character->comfort = json["character"].value("comfort", character->comfort);
            character->current_room_id = json["character"].value("room", character->current_room_id);
            character->current_outfit_id =
                json["character"].value("outfit", character->current_outfit_id);
        }
        if (json.contains("relationship")) {
            rel.scores.trust = json["relationship"].value("trust", rel.scores.trust);
            rel.scores.affection = json["relationship"].value("affection", rel.scores.affection);
        }
        if (json.contains("memories")) {
            rel.shared_memories.clear();
            for (const auto& mem : json["memories"]) {
                MemoryEntry entry;
                entry.id = mem.value("id", "");
                entry.description = mem.value("description", "");
                entry.emotional_weight = mem.value("weight", 0.5f);
                if (mem.contains("tags")) {
                    for (const auto& tag : mem["tags"]) {
                        entry.tags.push_back(tag.get<std::string>());
                    }
                } else if (!entry.id.empty()) {
                    entry.tags.push_back(entry.id);
                }
                rel.add_memory(std::move(entry));
            }
        }
        sync_relationship_stage();
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

CharacterSimState* RelationshipSimulation::character() {
    return profiles_.find("aiko");
}

const CharacterSimState* RelationshipSimulation::character() const {
    return profiles_.find("aiko");
}

const RelationshipModel* RelationshipSimulation::relationship() const {
    return relationships_.find(sim_state_.player_id, sim_state_.character_id);
}

RelationshipModel* RelationshipSimulation::relationship() {
    return relationships_.find(sim_state_.player_id, sim_state_.character_id);
}

RelationshipStage RelationshipSimulation::current_stage() const {
    const CharacterSimState* character = profiles_.find("aiko");
    if (character == nullptr) {
        return RelationshipStage::Stranger;
    }
    RelationshipProgressionScores scores;
    scores.trust = character->trust;
    scores.affection = character->affection;
    scores.familiarity = character->familiarity;
    return compute_stage(scores);
}

DialogueContext RelationshipSimulation::build_dialogue_context() const {
    DialogueContext ctx;
    ctx.character = const_cast<CharacterSimState*>(profiles_.find("aiko"));
    ctx.stage = current_stage();
    ctx.time_of_day = sim_state_.time_of_day;
    if (ctx.character != nullptr) {
        ctx.active_room = ctx.character->current_room_id;
        ctx.active_outfit = ctx.character->current_outfit_id;
    }
    const RelationshipModel* rel = relationship();
    if (rel != nullptr) {
        ctx.known_memory_tags = memories_.all_memory_tags(*rel);
    }
    return ctx;
}

void RelationshipSimulation::sync_relationship_stage() {
    CharacterSimState* character = profiles_.find("aiko");
    if (character == nullptr) {
        return;
    }
    RelationshipProgressionScores scores;
    scores.trust = character->trust;
    scores.affection = character->affection;
    scores.familiarity = character->familiarity;
    character->relationship_stage = compute_stage(scores);

    RelationshipModel& rel = relationships_.get_or_create(sim_state_.player_id, sim_state_.character_id);
    rel.scores.trust = character->trust;
    rel.scores.affection = character->affection;
}

InteractionResult RelationshipSimulation::execute_dialogue_node(std::string_view node_id) {
    CharacterSimState* character = profiles_.find("aiko");
    if (character == nullptr) {
        return {};
    }
    const DialogueResult node = dialogue_.evaluate_node(node_id, build_dialogue_context());
    InteractionResult result;
    if (!node.success) {
        result.rejection_reason = node.rejection_reason;
        return result;
    }
    result.success = true;
    result.dialogue_line = node.node->line;
    RelationshipModel& rel =
        relationships_.get_or_create(sim_state_.player_id, sim_state_.character_id);
    rel.record_conversation(node.node->line);
    character->record_interaction(node_id);
    sync_relationship_stage();
    result.new_stage = current_stage();
    result.new_mood = mood_.derive_mood(*character);
    return result;
}

} // namespace eve::ai
