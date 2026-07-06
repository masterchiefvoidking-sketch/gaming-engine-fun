# Phase 13: Polish + Performance Hardening

## Overview

Phase 13 turns Apartment Life Demo from a working vertical slice into a **Release Candidate 1** build — polished, smooth, stable, and shippable for personal offline play on Windows and Web.

**Focus:** Graphics, animation, camera, wardrobe, apartment atmosphere, save/load reliability, web/Windows performance.  
**Excluded:** Multiplayer, console, marketplace, cloud, online accounts.

## Polish Module (`eve::polish`)

| System | Description |
|--------|-------------|
| `GraphicsPolish` | 6 presets (Low/Medium/High/Ultra/WebSafe/MobileFuture), material/outline/pipeline tuning |
| `AnimationPolish` | Idle breathing, sit/stand, mirror poses, dialogue reactions, eye contact |
| `CameraPolish` | Smooth accel/decel, room limits, 8 room camera presets, privacy-safe bathroom |
| `WardrobePolish` | Fast switching, favorites, presets, filters, compatibility/clipping warnings |
| `ApartmentAtmosphere` | Morning/afternoon/sunset/rainy/night lighting, lamp/TV glow, audio hooks |
| `DialoguePolish` | Warmer compliments, apologies, outfit/room/memory/mood reactions |
| `PerformanceProfiler` | FPS, frame time graph, CPU/GPU timing, memory, draw calls, subsystem costs |
| `ValidationGate` | Pre-export checks: assets, dialogue, events, wardrobe, save schema |
| `BugTracker` | Structured bug format with regression test requirement |
| `RC1ReleasePipeline` | All 4 export profiles + 6 deliverable reports |

## Graphics Presets

| Preset | Target |
|--------|--------|
| Low | Minimum spec, no bloom/DOF |
| Medium | 60 FPS default |
| High | Desktop demo default |
| Ultra | Maximum anime romance quality |
| WebSafe | Modern desktop browser, 60 FPS |
| MobileFuture | Battery-conscious, reduced effects |

## Camera Presets

Loaded from `data/camera/room_presets.json`:

- Mirror, sofa conversation, dining table, bedroom goodnight
- Bathroom privacy-safe, photo mode, full-body, face focus
- Per-room distance limits and collision clamping

## Performance Targets

| Platform | Target |
|----------|--------|
| Windows | 60 FPS minimum (midrange GPU), 120 FPS editor viewport |
| Web | 60 FPS on modern desktop browser, progressive loading |

Profiler overlay format: `PerformanceProfiler::format_overlay()`

## Validation Gates

Before export, `ValidationGate` checks:

- No missing assets or broken references
- Valid dialogue nodes and event graphs
- Wardrobe compatibility
- Save schema compatibility
- Platform/renderer compatibility
- Clipping severity warnings

## Bug Tracker Format

Each bug requires: ID, severity, reproduction steps, expected/actual behavior, platform, fix owner, regression test. A bug is not fixed without a regression test.

## RC1 Deliverables

`apps/rc1_release` → `eve-rc1-release`

Produces under `Game/ApartmentLifeDemo/Build/rc1/`:

1. Windows development build
2. Windows release build
3. Web development build
4. Web release build
5. `performance_report.json`
6. `known_issues.json`
7. `test_report.json`
8. `asset_validation_report.json`
9. `EXPORT_INSTRUCTIONS.md`
10. `NEXT_PHASE_RECOMMENDATIONS.md`

## Run RC1

```bash
cmake --build build --target eve-rc1-release
./build/apps/rc1_release/eve-rc1-release
```

## Integration

`ApartmentLifeSession` now wires polish systems:

- Fixed lighting path: `data/anime/room_lighting.json`
- Graphics polish applied to pipeline on init
- Camera polish on every frame update
- Atmosphere per room change
- Validation gate for content checks
- Performance profiler during gameplay

## Tests

`tests/polish/test_polish_performance.cpp` — 14 tests covering renderer, materials, wardrobe, camera, dialogue, events, save/load, validation, profiler budget, RC1 validation.

## Known Limitations (RC1)

- GPU post-process passes (bloom/DOF) are settings-driven; full GPU passes planned for next phase
- Wardrobe UI panels remain layout scaffolding (logic complete)
- Asset cooking copies files; texture compression planned for next phase
