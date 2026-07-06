#include <eve/audit/performance_review.hpp>

#include <eve/ai/dialogue_engine.hpp>
#include <eve/ai/relationship_simulation.hpp>
#include <eve/build/build_pipeline.hpp>
#include <eve/build/export_profiles.hpp>
#include <eve/content/game_content_project.hpp>
#include <eve/core/filesystem/filesystem.hpp>
#include <eve/human/wardrobe_system.hpp>
#include <eve/platform/unified_save.hpp>

#include <chrono>

namespace eve::audit {
namespace {

f64 elapsed_ms(const std::chrono::steady_clock::time_point& start) {
    return std::chrono::duration<f64, std::milli>(
               std::chrono::steady_clock::now() - start)
        .count();
}

PerformanceMeasurement measure(std::string operation, auto&& fn, std::string notes = {}) {
    const auto start = std::chrono::steady_clock::now();
    fn();
    return {std::move(operation), elapsed_ms(start), std::move(notes)};
}

} // namespace

std::vector<PerformanceMeasurement> PerformanceReviewer::measure_all(
    std::string_view data_root, std::string_view game_root) const {
    std::vector<PerformanceMeasurement> results;
    const std::string data(data_root);
    const std::string game(game_root);

    results.push_back(measure("Editor startup (CreatorStudio init)", [&] {
        content::GameContentProject project;
        (void)project.open(game);
    }, "Proxy for editor project bootstrap"));

    results.push_back(measure("Project load time", [&] {
        content::GameContentProject project;
        if (!project.open(game)) {
            throw std::runtime_error("project open failed");
        }
        (void)project.characters().character_count();
        (void)project.wardrobe().item_count();
    }));

    results.push_back(measure("Scene load time", [&] {
        content::GameContentProject project;
        project.open(game);
        (void)project.apartments().furniture_count();
        (void)project.events().event_count();
    }));

    results.push_back(measure("Character load time", [&] {
        human::WardrobeSystem wardrobe;
        wardrobe.load_items(data + "/anime/wardrobe_items.json");
        (void)wardrobe.find_item("mira_outfit_01");
    }));

    results.push_back(measure("Wardrobe swap time", [&] {
        human::WardrobeSystem wardrobe;
        wardrobe.load_items(data + "/anime/wardrobe_items.json");
        human::LayeredOutfit outfit;
        outfit = wardrobe.equip("mira_outfit_01", outfit);
        outfit = wardrobe.equip("mira_outfit_05", outfit);
        outfit = wardrobe.equip("mira_outfit_10", outfit);
    }));

    results.push_back(measure("Dialogue evaluation", [&] {
        ai::DialogueEngine engine;
        engine.load_from_file(data + "/apartment_life/romance_sim/dialogue.json");
        for (u32 i = 0; i < 100; ++i) {
            (void)engine.node_count();
        }
    }, "100 node-count queries"));

    results.push_back(measure("Event execution", [&] {
        ai::RelationshipSimulation sim;
        sim.initialize(data, "mira", "apartment_life/romance_sim");
        sim.trigger_event("morning_greeting");
        sim.trigger_event("coffee_together");
    }));

    results.push_back(measure("Save time", [&] {
        platform::UnifiedSaveSystem save_system;
        platform::UnifiedSaveGame save;
        save.project_id = "apartment_life";
        save.character.character_id = "mira";
        save.character.outfit = "mira_outfit_01";
        const std::string path = data + "/saves/_audit_perf.json";
        if (!save_system.save(path, save)) {
            throw std::runtime_error("save failed");
        }
        std::filesystem::remove(path);
    }));

    results.push_back(measure("Load time", [&] {
        platform::UnifiedSaveSystem save_system;
        platform::UnifiedSaveGame save;
        save.project_id = "apartment_life";
        save.character.character_id = "mira";
        const std::string path = data + "/saves/_audit_perf_load.json";
        (void)save_system.save(path, save);
        platform::UnifiedSaveGame loaded;
        if (!save_system.load(path, loaded)) {
            throw std::runtime_error("load failed");
        }
        std::filesystem::remove(path);
    }));

    results.push_back(measure("Build export time", [&] {
        build::BuildPipeline pipeline;
        build::BuildPipelineConfig cfg;
        cfg.project_root = game;
        cfg.project_name = "apartment_life";
        cfg.profile = build::ExportProfile::WindowsDevelopment;
        cfg.backup_root = game + "/Backups";
        const build::BuildPipelineResult result = pipeline.export_project(cfg);
        if (!result.success) {
            throw std::runtime_error("export failed");
        }
    }, "Windows development profile"));

    return results;
}

} // namespace eve::audit
