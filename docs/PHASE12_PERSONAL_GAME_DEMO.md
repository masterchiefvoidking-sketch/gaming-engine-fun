# Phase 12: Apartment Life Demo — Personal Game Vertical Slice

## Overview

Phase 12 delivers the first complete playable vertical slice for EVE Engine: **Apartment Life Demo**. This proves the engine can power a polished, single-player, adult-only, anime-inspired apartment relationship simulator for Windows and Web.

**Platforms:** Windows desktop (Vulkan), Web (WebGPU + WebAssembly).  
**Excluded:** Multiplayer, console SDKs, marketplace, cloud services, online accounts.

## Gameplay Module (`eve::gameplay`)

| Class | Description |
|-------|-------------|
| `ApartmentLifeConfig` | Session configuration (data paths, character, save slot) |
| `ApartmentLifeSession` | Orchestrates content, relationship sim, scene director, viewer, save/load, export |

`ApartmentLifeSession` wires together:

- `GameContentProject` — Mira, 20+ wardrobe items, 6-room apartment, dialogue, events
- `RelationshipSimulation` — Mira profile from `data/apartment_life/romance_sim/`
- `SceneDirector` — cinematic events with camera, lighting, motion, expressions
- `WebCharacterViewer` — orbit camera, interaction ring, wardrobe preview, photo mode
- `UnifiedSaveSystem` — character, relationship, apartment, wardrobe, story slices
- `BuildPipeline` — Windows/Web development export validation

## Character: Mira

- Age 22, original adult anime-style illustrator
- 100 runtime dialogue lines (room, outfit, mood, memory, relationship categories)
- 10 playable events with conditions, cooldowns, and memory creation
- 20 wardrobe items across casual, sleepwear, formal, athletic, dresses, shoes, accessories
- Customizable hair, expressions, outfit colors

## Apartment: Sunset Apartment

Six rooms with corrected furniture placement:

| Room | Interactions |
|------|-------------|
| Bedroom | Sit, sleep, read, change outfit |
| Bathroom | Mirror, grooming (privacy-gated) |
| Living room | Sofa, TV, music, photo pose |
| Kitchen / Dining | Prepare food, eat together, tea |
| Wardrobe | Outfit change, mirror preview, save outfit |

## Runtime Data

| Path | Purpose |
|------|---------|
| `Game/ApartmentLifeDemo/` | Authoring content (character, apartment, events) |
| `data/apartment_life/romance_sim/` | Mira relationship sim (100 dialogue, 10 events) |
| `data/anime/wardrobe_items.json` | Runtime wardrobe (synced from game content) |
| `data/scene_director/` | Scene templates, camera shots, lighting cues |

## Application

`apps/apartment_life_demo` → `eve-apartment-life-demo`

Runs the full vertical slice: room navigation, wardrobe customization, dialogue, 10 events, photo mode, save/load, Windows + Web export.

```bash
cmake --build build --target eve-apartment-life-demo
./build/apps/apartment_life_demo/eve-apartment-life-demo
```

## Web + Windows Builds

Export via session API or `eve-personal-export`:

1. **Windows development** — Vulkan, local saves, debug overlay
2. **Web development** — WebGPU, touch controls, browser saves, progressive loading

## Editor Integration

Editable through Creator Studio panels:

- Character, wardrobe, apartment, dialogue, event editors
- Camera preset and lighting preset editors
- Save/debug inspector
- Export panel (Phase 11)

## Tests

`tests/gameplay/test_apartment_life_game.cpp` covers:

- Character customization, wardrobe compatibility, camera controls
- Dialogue branching, relationship updates, event conditions
- Scene cooldowns, save/load, asset validation
- Windows/Web export, vertical slice loop

## Vertical Slice Checklist

- [x] Original adult anime-style character (Mira)
- [x] Beautiful apartment rendering (6 rooms, corrected layout)
- [x] Wardrobe customization (20 items, randomizer)
- [x] Relationship dialogue (100 lines, branching)
- [x] Room interactions per room type
- [x] 10 cinematic events
- [x] Smooth camera controls (mouse + touch)
- [x] Save/load (manual + auto)
- [x] Photo mode
- [x] Offline-first, personal, no cloud
