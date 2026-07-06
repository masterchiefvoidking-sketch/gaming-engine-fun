# EVE Engine Architecture

## Overview

EVE Adult Engine is organized as a set of layered modules with explicit dependencies. Each module exposes a public API under `engine/<module>/include/eve/<module>/` and compiles to a static or interface library target `eve::<module>`.

## Layer Model

```
┌─────────────────────────────────────────────────────────────┐
│  apps/     Editor, demos, release pipelines, audit tools    │
├─────────────────────────────────────────────────────────────┤
│  product, audit, gameplay, polish, editor                   │
├─────────────────────────────────────────────────────────────┤
│  interaction, build, scene, human, ai, world, animation     │
├─────────────────────────────────────────────────────────────┤
│  content, platform, scripting                               │
├─────────────────────────────────────────────────────────────┤
│  render, ecs, physics (stub)                                │
├─────────────────────────────────────────────────────────────┤
│  core           Foundation utilities                        │
└─────────────────────────────────────────────────────────────┘
```

All 19 engine modules have `README.md` files. See `docs/PHASE15_ARCHITECTURE_FREEZE.md` for the engineering audit.

## Core Principles

1. **Dependency direction flows upward** — higher layers depend on lower layers, never the reverse.
2. **Modules are independently testable** — each subsystem has dedicated unit tests under `tests/`.
3. **Interfaces over implementations** — plugin and renderer backends use abstract interfaces where extension is expected.
4. **Data-oriented ECS** — gameplay and simulation state lives in components; systems process matching archetypes.

## Subsystem Boundaries

| Module | Responsibility | Key Types |
|--------|----------------|-----------|
| `core` | Engine infrastructure | `Logger`, `Serializer`, `ReflectionRegistry`, `JobSystem` |
| `ecs` | Scene entity model | `World`, `ComponentRegistry` |
| `render` | GPU presentation | `VulkanContext`, `PhotoMode`, `RomanceCameraController` |
| `animation` | Motion systems | `AnimationStateMachine`, `AnimationLayerStack` |
| `scripting` | Script host | `ScriptEngine`, `IScriptHost` |
| `world` | Apartment simulation | `ApartmentLoader`, `InteractionSystem`, `ApartmentPersistence` |
| `ai` | Romance/relationship sim | `RelationshipSimulation`, `DialogueEngine` |
| `scene` | Scene direction | `SceneDirector`, `EventScheduler` |
| `content` | Game content authoring | `ContentDatabase`, `GameContentProject`, `WardrobeDatabase` |
| `platform` | Mission scope, save, web | `UnifiedSaveSystem`, `WebPublisher`, `GameSession` |
| `interaction` | Player input, viewer | `InputController`, `WebCharacterViewer` |
| `build` | Personal export pipeline | `BuildPipeline`, `AssetCooker`, `ContentValidator` |
| `human` | Digital humans | `CharacterCreator`, `WardrobeSystem`, `AnimeVisualPipeline` |
| `physics` | Physics stub (future) | `kModuleName` only |
| `editor` | Authoring shell | `EditorApplication`, `CreatorStudio` |
| `polish` | Performance and validation | `PerformanceProfiler`, `ValidationGate` |
| `gameplay` | Session orchestration | `ApartmentLifeSession` |
| `product` | Release tooling | `PersonalProductCut`, `ReleaseBundle` |
| `audit` | Engineering audit | `ArchitectureFreeze`, `ModuleHealthAuditor` |

## Extension Points

- **Plugins** — dynamic libraries exporting `eve_plugin_create` / `eve_plugin_destroy`
- **Reflection** — register `TypeDescriptor` metadata for serialization and editor inspectors
- **ECS components** — register via `ComponentRegistry` and attach to `World` entities

## Milestone Roadmap

### Milestone 1 (current)
Foundation: CMake, core, ECS, Vulkan init, editor shell, tests, CI.

### Milestone 2
Swapchain and frame graph, ImGui docking UI, animation state machines, character creator MVP.

### Milestone 3
PBR skin/hair shaders, cloth simulation hooks, dialogue and emotion systems, apartment builder.

### Phase 9 (current direction)
Engine mission reset: lean scope, unified save, web deployment tooling, mobile-ready input, local backups and templates. See `docs/ENGINE_MISSION.md`.

### Phase 10
Web and mobile interaction layer: unified input, character inspection, interaction ring, responsive UI, quality settings, browser save storage, Web Apartment Character Viewer. See `docs/PHASE10_WEB_MOBILE_INTERACTION.md`.

### Phase 11
Personal build + export pipeline: export profiles, asset cooking, validation, archives, local content packs, backup tools, editor export panel. See `docs/PHASE11_PERSONAL_BUILD.md`.

## Threading Model

- **Main thread** — window events, editor UI, render submission
- **Job system** — parallel asset processing, animation pose search, offline baking
- **Render thread** (future) — optional dedicated submission queue

## Memory Strategy

- **Linear allocators** — per-frame transient allocations
- **Pool allocators** — homogeneous component pools (future ECS optimization)
- **Persistent assets** — reference-counted GPU resources (milestone 2)
