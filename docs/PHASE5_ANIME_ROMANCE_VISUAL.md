# Phase 5: Anime Romance Visual Pipeline

## Overview

Phase 5 delivers a premium anime-style rendering pipeline for adult romance and life-sim experiences. The visual language is original and legally distinct — inspired by polished TV anime production values without copying any copyrighted work.

## Art Style

- Anime cel shading with soft gradient skin
- Clean outline rendering
- Hair highlight bands and eye sparkle layers
- Blush effects and stylized makeup
- Warm apartment lighting with cinematic romance framing
- Depth-of-field portrait shots

## Modules

### `eve::render` — Anime Graphics

| System | Description |
|--------|-------------|
| `AnimeHumanMaterial` | Toon ramp, blush overlay, rim light, mood tint, wetness |
| `AnimeEyeSystem` | Multi-layer highlights, gaze modes, blink/wink, emotion shapes |
| `AnimeHairRenderer` | Chunk/card/strand modes, highlight bands, physics sway |
| `AnimeOutlinePass` | Configurable outline thickness and silhouette pass |
| `RomanceCameraController` | Cinematic presets, DOF, focal length, rule-of-thirds |

### `eve::human` — Anime Character

| System | Description |
|--------|-------------|
| `AnimeCharacterStyleSystem` | Stylized proportions, silhouette presets |
| `AnimeExpressionSystem` | 16 expression presets with full channel control |
| `AnimeVisualPipeline` | Unified orchestrator wiring render + character systems |

### `eve::world` — Apartment Atmosphere

| System | Description |
|--------|-------------|
| `AnimeRoomLightingSystem` | Per-room mood presets: morning, sunset, cozy lamp, night, rain, neon |

### `eve::animation` — Anime Motion

| System | Description |
|--------|-------------|
| `AnimeMotionController` | Body language, breathing, hair/cloth sway, reaction clips |

## Shaders

```
engine/render/shaders/
  anime_skin.vert / anime_skin.frag  — Cel shading + blush + rim
  anime_eye.frag                     — Iris, pupil, sparkle highlights
  anime_hair.frag                    — Gradient tips + highlight bands
```

## Expression Presets

Neutral, Smile, Soft Smile, Laugh, Embarrassed, Blushing, Shy, Annoyed, Sad, Surprised, Flirty, Sleepy, Curious, Pouting, Confident, Romantic Gaze

## Demo Scene

```bash
./build/apps/anime_wardrobe_demo/eve-anime-wardrobe-demo data
```

**Cozy Apartment Wardrobe Preview** includes:
- Original anime-style female character (Aiko)
- Wardrobe room with warm cozy-lamp lighting
- Orbit/zoom/pan camera
- Outfit, hair, and expression switching
- Blush toggle
- Full-body and face close-up camera presets
- Screenshot capture

## Editor Tools

Panel scaffolds in `anime_visual_tools.hpp`:
- Anime material editor, toon ramp editor, outline editor
- Eye/hair highlight editors, blush editor
- Expression preset editor, room lighting presets
- Romance camera presets, screenshot studio

## Testing

```bash
cmake --build build
ctest --test-dir build --output-on-failure
```

Phase 5 adds 27 new tests covering materials, eyes, expressions, lighting, camera, motion, and the full demo flow.

## Data

```
data/anime/
  materials.json
  eye_presets.json
  hair_styles.json
  expressions.json
  character_preset.json
  room_lighting.json
  romance_cameras.json
  wardrobe_items.json
```
