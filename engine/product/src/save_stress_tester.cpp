#include <eve/product/save_stress_tester.hpp>

#include <eve/core/filesystem/filesystem.hpp>
#include <eve/platform/unified_save.hpp>

#include <chrono>
#include <filesystem>

namespace eve::product {

namespace {

platform::UnifiedSaveGame make_save(std::string_view slot, u32 cycle) {
    platform::UnifiedSaveGame save;
    save.project_id = "apartment_life";
    save.save_slot = std::string(slot);
    save.character.character_id = "mira";
    save.character.outfit = cycle % 2 == 0 ? "mira_outfit_01" : "mira_outfit_05";
    save.character.room = cycle % 3 == 0 ? "bedroom" : "living_room";
    save.wardrobe.active_outfit_id = save.character.outfit;
    save.dialogue.unlocked_nodes = {"greeting_01", "coffee_01"};
    save.dialogue.story_flags = {"met_mira"};
    save.story.completed_events = {"morning_greeting"};
    save.apartment.apartment_id = "sunset_apartment";
    save.apartment.active_room = save.character.room;
    save.relationship.trust = 0.3f + static_cast<f32>(cycle % 10) * 0.05f;
    save.relationship.affection = 0.4f;
    save.memories.push_back({"first_meeting", "Met in the living room", 0.8f});
    save.timestamp = static_cast<u64>(
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
    return save;
}

} // namespace

SaveStressResult SaveStressTester::run(const SaveStressConfig& config) {
    SaveStressResult result;
    const std::string save_dir = config.game_root + "/Saves/_stress";
    FileSystem::create_directories(save_dir);
    platform::UnifiedSaveSystem save_system;

    for (u32 slot = 0; slot < config.slot_count; ++slot) {
        const std::string slot_name = "slot_" + std::to_string(slot);
        const std::string path = save_dir + "/" + slot_name + ".json";
        const platform::UnifiedSaveGame save = make_save(slot_name, slot);
        if (!save_system.save(path, save)) {
            result.failures.push_back("Initial save failed for " + slot_name);
            continue;
        }
        ++result.slots_tested;
    }

    for (u32 cycle = 0; cycle < config.cycle_count; ++cycle) {
        const u32 slot = cycle % config.slot_count;
        const std::string slot_name = "slot_" + std::to_string(slot);
        const std::string path = save_dir + "/" + slot_name + ".json";
        platform::UnifiedSaveGame loaded;
        if (!save_system.load(path, loaded)) {
            result.failures.push_back("Load failed cycle " + std::to_string(cycle));
            continue;
        }
        loaded.character.outfit =
            cycle % 2 == 0 ? "mira_outfit_02" : "mira_outfit_08";
        loaded.wardrobe.active_outfit_id = loaded.character.outfit;
        loaded.dialogue.unlocked_nodes.push_back("cycle_" + std::to_string(cycle));
        loaded.story.completed_events.push_back("event_" + std::to_string(cycle % 10));
        if (!save_system.save(path, loaded)) {
            result.failures.push_back("Rewrite failed cycle " + std::to_string(cycle));
            continue;
        }
        ++result.cycles_completed;
    }

    const std::string corrupt_path = save_dir + "/slot_0.json";
    const std::string backup_path = save_dir + "/slot_0.json.bak";
    if (!simulate_corruption_restore(corrupt_path, backup_path)) {
        result.failures.push_back("Backup restore simulation failed");
    }

    result.passed = result.failures.empty() && result.slots_tested == config.slot_count &&
                    result.cycles_completed == config.cycle_count;
    return result;
}

bool SaveStressTester::simulate_corruption_restore(std::string_view save_path,
                                                   std::string_view backup_path) const {
    if (!FileSystem::exists(save_path)) {
        return false;
    }
    std::filesystem::copy_file(std::string(save_path), std::string(backup_path),
                               std::filesystem::copy_options::overwrite_existing);
    FileSystem::write_text_file(save_path, "{ invalid json corruption");

    platform::UnifiedSaveSystem save_system;
    platform::UnifiedSaveGame loaded;
    if (save_system.load(save_path, loaded)) {
        return false;
    }

    std::filesystem::copy_file(std::string(backup_path), std::string(save_path),
                               std::filesystem::copy_options::overwrite_existing);
    return save_system.load(save_path, loaded);
}

} // namespace eve::product
