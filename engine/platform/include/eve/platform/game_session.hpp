#pragma once

#include <eve/ai/relationship_simulation.hpp>
#include <eve/platform/backup_manager.hpp>
#include <eve/platform/mobile_input.hpp>
#include <eve/platform/unified_save.hpp>
#include <eve/platform/web_publisher.hpp>

#include <string>

namespace eve::platform {

struct GameSessionConfig {
    std::string project_id;
    std::string data_root;
    std::string save_slot = "slot_01";
    bool offline_only = true;
};

class GameSession {
public:
    bool initialize(const GameSessionConfig& config);
    bool save_game(std::string_view path) const;
    bool load_game(std::string_view path);
    bool publish_web(std::string_view output_dir) const;
    bool create_backup(std::string_view backup_root) const;

    [[nodiscard]] ai::RelationshipSimulation& relationship() { return relationship_; }
    [[nodiscard]] MobileInputController& mobile_input() { return mobile_input_; }
    [[nodiscard]] const UnifiedSaveGame& save_data() const { return save_data_; }

private:
    GameSessionConfig config_;
    ai::RelationshipSimulation relationship_;
    UnifiedSaveSystem save_system_;
    UnifiedSaveGame save_data_;
    MobileInputController mobile_input_;
    WebPublisher web_publisher_;
    BackupManager backup_manager_;
    bool initialized_ = false;
};

} // namespace eve::platform
