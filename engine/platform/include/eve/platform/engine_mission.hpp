#pragma once

#include <eve/core/types.hpp>

#include <string_view>

namespace eve::platform {

// Engine mission: anime-inspired story-driven relationship & life simulation.
// NOT a general-purpose engine. See docs/ENGINE_MISSION.md.

constexpr const char* kEngineMission =
    "Beautiful, high-performance, anime-inspired, story-driven relationship and life simulation.";

constexpr u32 kTargetFpsDesktopMin = 60;
constexpr u32 kTargetFpsDesktopMax = 144;
constexpr u32 kTargetFpsWeb = 60;

enum class SupportedPlatform : u8 {
    Windows = 0,
    Web,
    AndroidFuture,
    IosFuture,
};

enum class OutOfScopeFeature : u8 {
    MultiplayerNetworking = 0,
    DedicatedServers,
    ConsoleSdks,
    MarketplaceInfrastructure,
    MmoSystems,
    CompetitiveGameplay,
    LiveService,
    BattlePass,
    Subscriptions,
    AlwaysOnline,
    TelemetryRequired,
};

[[nodiscard]] constexpr bool is_in_scope_for_mission(std::string_view subsystem) noexcept {
    return subsystem == "characters" || subsystem == "animation" || subsystem == "apartments" ||
           subsystem == "ai" || subsystem == "dialogue" || subsystem == "graphics" ||
           subsystem == "creator" || subsystem == "scene" || subsystem == "relationship" ||
           subsystem == "wardrobe" || subsystem == "save" || subsystem == "content";
}

[[nodiscard]] constexpr bool is_out_of_scope(OutOfScopeFeature feature) noexcept {
    (void)feature;
    return true;
}

[[nodiscard]] const char* platform_name(SupportedPlatform platform);
[[nodiscard]] const char* out_of_scope_reason(OutOfScopeFeature feature);

} // namespace eve::platform
