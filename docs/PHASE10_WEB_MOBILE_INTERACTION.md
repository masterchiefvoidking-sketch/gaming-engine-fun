# Phase 10: Web and Mobile Interaction Layer

## Overview

Phase 10 rebuilds the player interaction layer around Windows, Web, and future mobile targets. It introduces the `eve::interaction` module with unified input, camera bridging, character inspection, interaction ring, responsive UI layout, quality settings, browser save storage, and the **Web Apartment Character Viewer** vertical slice.

## Interaction Module (`eve::interaction`)

| System | Description |
|--------|-------------|
| `InputController` | Mouse+keyboard, touchscreen, optional gamepad input |
| `CameraInputBridge` | Maps input frames to `InspectionCamera` orbit/pan/zoom |
| `CharacterInspectionMode` | Premium character viewer (orbit, body focus, outfit/hair/expression preview) |
| `InteractionRing` | Context menu filtered by room, relationship, mood, privacy, outfit |
| `PhotoModeController` | Photo mode with DOF, lighting, screenshot capture |
| `QualitySettings` | Resolution, texture, shadow, hair, cloth, reflections, post-FX |
| `WebPerformanceProfile` | WebGPU/WASM settings, LOD tiers, mobile fallback |
| `ResponsiveUILayout` | Desktop, tablet, phone panel layouts with large touch targets |
| `SaveStorageRouter` | Local file (desktop) and IndexedDB abstraction (web) |
| `PlayerUIState` | Panel visibility for Character, Wardrobe, Relationship, etc. |
| `WebCharacterViewer` | Full vertical slice orchestrator |

## Control Modes

### Desktop (mouse + keyboard)
- Right drag → orbit
- Mouse wheel → zoom
- Middle drag → pan
- Left click → select
- Double click → focus
- Shift + drag → fine camera movement

### Touch
- One finger drag → orbit
- Pinch → zoom
- Two finger drag → pan
- Tap → select
- Double tap → focus
- Long press → context menu (interaction ring)

### Gamepad (optional, low priority)
- Right stick orbit, trigger zoom stubs via `InputController`

## Interaction Ring Actions

Look, Talk, Compliment, Change Outfit, Pose, Use Object, Inspect, Cancel — filtered by:
- Room, relationship state, privacy, outfit, mood, activity, content settings

## Web Performance

`WebPerformanceProfile` configures:
- WebGPU renderer path, WASM build flags
- Compressed assets, progressive loading, texture streaming
- Shader quality tiers, animation/character LOD
- Hair quality, reflection quality, mobile fallback

## Mobile Performance

`apply_mobile_defaults()` enables:
- Reduced shader complexity, lower texture resolution
- Simplified hair/cloth physics flags
- Dynamic resolution scaling, battery-saving mode

## Save System

`SaveStorageRouter` supports:
- **Desktop:** local files (`LocalFileStorage`)
- **Web:** IndexedDB abstraction (`BrowserIndexedDbStorage`)
- **Future mobile:** same router, `MobileDevice` backend slot

Saves character preset, apartment state, relationship, wardrobe, dialogue, memories, photos, settings via unified save format.

## Vertical Slice: Web Apartment Character Viewer

App: `apps/web_character_viewer` → `eve-web-character-viewer`

The player can:
1. Load the apartment character scene
2. Rotate, zoom, and pan around the character
3. Tap/click body regions for inspection
4. Open wardrobe and change outfit
5. Change hairstyle and expression
6. Trigger romance/life-sim reactions (talk, compliment)
7. Use photo mode
8. Save and reload the scene
9. Publish web deployment package

## Tests

14 new tests in `tests/interaction/test_web_mobile_interaction.cpp` covering mouse input, touch input, camera orbit/zoom/pan, body-region focus, interaction ring filtering, responsive UI, quality settings, browser storage, and the full vertical slice flow.

## Explicitly Out of Scope

- Console SDK systems
- Multiplayer networking
- Marketplace infrastructure

See `docs/ENGINE_MISSION.md` for permanent mission constraints.
