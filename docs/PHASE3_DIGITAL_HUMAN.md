# Phase 3: Digital Human Framework

## Overview

Phase 3 delivers a production-ready character framework focused on expressive digital humans, high-fidelity rendering foundations, professional inspection camera tooling, and apartment-integrated character interaction.

## Modules

### `eve::human` — Character Framework

| System | Description |
|--------|-------------|
| `CharacterCreator` | Full appearance customization with JSON persistence |
| `CharacterProfile` | Body, skin, face, hair, hands, legs/feet parameters |
| `CharacterInteractionSystem` | Reach, grab, sit, look, open/close, idle behaviors |
| `CharacterFramework` | Unified entry point wiring all character subsystems |

### `eve::render` — Graphics

| System | Description |
|--------|-------------|
| `InspectionCamera` | Orbit, free, shoulder, FP/TP, cinematic, photo modes |
| `HumanShaderPipeline` | PBR skin with SSS, eye/hair/cloth material params |
| `HairRenderer` | Strand/card modes, LOD, wind, collision hooks |
| `PhotoMode` | DOF, exposure, lighting presets, HDR screenshots |

### `eve::animation` — Animation

| System | Description |
|--------|-------------|
| `AnimationStateMachine` | States, transitions, blend timing |
| `BlendTree` | 1D/2D/additive blend nodes |
| `IKSolver` | Two-bone, finger, look-at IK |
| `FacialAnimationController` | Micro-expressions, blend shapes |
| `ProceduralSecondaryMotion` | Breathing, idle sway |

## Shaders

```
engine/render/shaders/
  skin.vert / skin.frag   — PBR + subsurface scattering
  hair.vert / hair.frag   — Strand highlight shading
```

## Example Scene

```bash
./build/apps/example_scene/eve-example-scene data
```

Demonstrates apartment loading, room navigation, character customization, object interaction, and photo mode capture.

## Benchmarks

Enable with `-DEVE_ENABLE_BENCHMARKS=ON`:

- `eve_benchmark_character_creator` — customization throughput
- `eve_benchmark_inspection_camera` — camera update/matrix cost

## Testing

```bash
ctest --test-dir build --output-on-failure
```

New suites: `eve_human_tests`, `eve_animation_tests`, extended `eve_render_tests`, `eve_example_scene_tests`

## Plugin Interfaces

- `IScriptHost` (Phase 2) for scripted character behaviors
- `IPlugin` (core) for rendering and animation backend extensions
- `UpscaleTechnology` abstraction ready for DLSS/FSR/XeSS

## Customization Coverage

- **Body**: height, weight, proportions, muscle, presets, age, skin tone/details
- **Face**: shape, jaw, lips, eyes, brows, lashes, makeup, expressions
- **Hair**: style, length, curl, colors, highlights, physics/wind flags
- **Hands**: fingers, nails, polish, rings, bracelets
- **Legs/Feet**: proportions, shoes, socks, heels, procedural placement
