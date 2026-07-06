# Phase 9: Engine Direction Reset

## Overview

Phase 9 codifies the EVE Engine mission: a lean, anime-inspired relationship and life simulation engine — **not** a general-purpose game engine. It introduces the `eve::platform` module for mission scope gates, unified saves, web publishing, mobile-ready input, local backups, project templates, and offline asset libraries.

## Mission Statement

> Beautiful, high-performance, anime-inspired, story-driven relationship and life simulation.

See `docs/ENGINE_MISSION.md` for the full direction document.

## Platform Module (`eve::platform`)

| System | Description |
|--------|-------------|
| `engine_mission` | Mission constants, FPS targets, scope checks, out-of-scope feature registry |
| `unified_save` | Single save format for all game state slices |
| `web_publisher` | One-click web deployment package (manifest, loader HTML, build info) |
| `mobile_input` | Touch gestures, orbit camera, mobile render quality settings |
| `backup_manager` | Local project backup and restore |
| `project_templates` | Template library with instantiate-to-folder |
| `local_asset_library` | Offline reusable asset packs (not a marketplace) |
| `game_session` | Session orchestrator tying relationship sim, save, web, and backup |

## Unified Save Format

Magic: `EVE_SAVE`, version `1`, JSON encoding.

| Slice | Contents |
|-------|----------|
| Character | trust, affection, comfort, mood, room, outfit |
| Relationship | trust, affection, respect, stage |
| Dialogue | unlocked nodes, story flags, completed conversations |
| Memories | id, description, emotional weight |
| Apartment | id, room, time, weather, day |
| Inventory | item ids |
| Wardrobe | active outfit, owned, favorites |
| Photos | id, path, caption, day |
| Story | completed events/scenes, unlocked chapters |
| Settings | audio, locale, quality, touch controls |

Integrates with `eve::ai::RelationshipSimulation` via capture/apply helpers.

## Web Deployment

`WebPublisher::publish()` generates:

```
dist/web/
  manifest.json     # renderer, wasm, compression, features
  index.html        # responsive WebGPU loader placeholder
  build_info.json   # build metadata
  assets/           # asset staging directory
```

WebAssembly binary build is documented as a future Emscripten CI step.

## Scope Gates

`is_in_scope_for_mission(subsystem)` returns true for mission-aligned subsystems (characters, animation, apartments, ai, dialogue, graphics, creator, etc.).

`is_out_of_scope(OutOfScopeFeature)` documents rejected features: multiplayer, marketplace, live service, telemetry, and more.

## Lean Build

CMake option `EVE_LEAN_BUILD` disables optional demos and editor targets for faster iteration:

```bash
cmake -B build -DEVE_LEAN_BUILD=ON
```

`EVE_BUILD_WEB` controls web deployment tooling (default ON).

## Project Templates

```
data/templates/apartment_life_template.json  →  Game/ApartmentLifeDemo
```

`ProjectTemplateLibrary::set_content_root()` resolves relative template source paths.

## Tests

`tests/platform/test_engine_direction.cpp` — 9 tests covering mission scope, unified save round-trip, relationship sim integration, web publisher, mobile input, backup, templates, local library, and game session.

## What We Did Not Build

- Multiplayer networking
- Marketplace / asset store
- Live service systems
- Console SDK integration
- MMO or competitive gameplay infrastructure

These are permanently out of mission scope per `engine_mission.hpp`.
