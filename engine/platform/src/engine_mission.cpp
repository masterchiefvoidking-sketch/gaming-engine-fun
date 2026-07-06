#include <eve/platform/engine_mission.hpp>

namespace eve::platform {

const char* platform_name(SupportedPlatform platform) {
    switch (platform) {
    case SupportedPlatform::Web:
        return "web";
    case SupportedPlatform::AndroidFuture:
        return "android";
    case SupportedPlatform::IosFuture:
        return "ios";
    default:
        return "windows";
    }
}

const char* out_of_scope_reason(OutOfScopeFeature feature) {
    switch (feature) {
    case OutOfScopeFeature::MultiplayerNetworking:
        return "Single-player local simulation only";
    case OutOfScopeFeature::DedicatedServers:
        return "No multiplayer infrastructure";
    case OutOfScopeFeature::ConsoleSdks:
        return "Windows and Web primary targets";
    case OutOfScopeFeature::MarketplaceInfrastructure:
        return "Local asset library instead of marketplace";
    case OutOfScopeFeature::MmoSystems:
        return "Intimate single-apartment experiences";
    case OutOfScopeFeature::CompetitiveGameplay:
        return "Story-driven, not competitive";
    case OutOfScopeFeature::LiveService:
        return "Fully offline-capable engine";
    case OutOfScopeFeature::BattlePass:
        return "No live service monetization";
    case OutOfScopeFeature::Subscriptions:
        return "No subscription systems";
    case OutOfScopeFeature::AlwaysOnline:
        return "Engine works completely offline";
    default:
        return "Out of mission scope";
    }
}

} // namespace eve::platform
