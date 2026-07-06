#include <eve/platform/game_session.hpp>
#include <eve/core/logging/logger.hpp>

namespace eve::platform {

bool GameSession::initialize(const GameSessionConfig& config) {
    config_ = config;
    if (!relationship_.initialize(config_.data_root)) {
        return false;
    }
    save_data_.project_id = config_.project_id;
    save_data_.save_slot = config_.save_slot;
    initialized_ = true;
    EVE_LOG(Info, "GameSession", "Session initialized (offline=", config_.offline_only, ")");
    return true;
}

bool GameSession::save_game(std::string_view path) const {
    if (!initialized_) {
        return false;
    }
    UnifiedSaveGame save = save_system_.capture_from_relationship_sim(relationship_, config_.project_id);
    save.save_slot = config_.save_slot;
    return save_system_.save(path, save);
}

bool GameSession::load_game(std::string_view path) {
    if (!initialized_) {
        return false;
    }
    UnifiedSaveGame loaded;
    if (!save_system_.load(path, loaded)) {
        return false;
    }
    save_data_ = loaded;
    save_system_.apply_to_relationship_sim(relationship_, save_data_);
    return true;
}

bool GameSession::publish_web(std::string_view output_dir) const {
    WebDeployConfig config;
    config.project_name = config_.project_id;
    config.output_dir = std::string(output_dir);
    const WebDeployResult result = web_publisher_.publish(config, config_.data_root);
    return result.success;
}

bool GameSession::create_backup(std::string_view backup_root) const {
    return backup_manager_.create_backup(config_.data_root, backup_root, config_.project_id);
}

} // namespace eve::platform
