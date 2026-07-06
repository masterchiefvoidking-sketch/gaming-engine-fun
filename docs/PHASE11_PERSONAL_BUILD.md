# Phase 11: Personal Build + Export Pipeline

## Overview

Phase 11 delivers a lean, offline-first build and export pipeline for personal single-player projects. No multiplayer, marketplace, cloud services, or live-service systems.

**Targets:** Windows (Vulkan), Web (WebAssembly + WebGPU), future Android/iOS.

## Build Module (`eve::build`)

| System | Description |
|--------|-------------|
| `ExportProfile` | Windows/Web development and release profiles |
| `AssetCooker` | Raw assets → optimized runtime packages |
| `ContentValidator` | Pre-export validation (missing assets, broken refs, graph errors) |
| `DependencyManifestWriter` | Dependency manifests for cooked assets |
| `ProjectArchive` | Export/import personal project archives |
| `PersonalContentPack` | Local-only content packs (no marketplace) |
| `PersonalBackupTool` | Manual/auto backup and restore |
| `BuildPipeline` | One-click validate → cook → export orchestrator |

## Export Profiles

| Profile | Platform | Renderer | Notes |
|---------|----------|----------|-------|
| `WindowsDevelopment` | Windows | Vulkan | Debug console, local saves, full quality |
| `WindowsRelease` | Windows | Vulkan | Compressed assets, no debug overlay |
| `WebDevelopment` | Web | WebGPU | IndexedDB saves, touch controls, progressive loading |
| `WebRelease` | Web | WebGPU | Compressed textures, battery-conscious settings |

Each profile controls: renderer backend, asset compression, texture size, shader/hair/cloth quality, screenshot support, debug console, save location.

## Project Structure

```
Engine/          Runtime modules (core, render, ai, content, build, …)
Game/            Personal projects (e.g. ApartmentLifeDemo)
data/templates/  Blank, ApartmentSim, VisualNovel, LifeSim templates
Build/           Generated exports per profile
```

## Asset Cooking

Input: textures, meshes, animations, dialogue, events, audio, materials, character/wardrobe presets, apartment scenes.

Output: `cooked/` runtime packages, `runtime_package.json`, platform-specific settings, `dependency_manifest.json`.

## Editor Export Panel

`eve::editor::ExportPanel` — select project, platform profile, validate, cook, build, open output folder.

Integrated into `CreatorStudio::export_with_profile()`.

## Vertical Slice

`apps/personal_export` → `eve-personal-export`

Exports **Apartment Life Demo** to:
1. Windows development build
2. Web development build

Includes character, apartment rooms, wardrobe, dialogue, relationship state, photo mode support, local save paths.

## Tests

15 tests in `tests/build/test_personal_build.cpp` — profiles, cooking, validation, broken refs, save migration, manifests, archives, content packs, backup, pipeline, export panel, run locally, full demo.

## Project Structure

See `docs/PROJECT_STRUCTURE.md` for mapping from spec layout to repository paths.

## Out of Scope

Multiplayer, console SDKs, marketplace, cloud services, live-service, online accounts.

See `docs/ENGINE_MISSION.md`.
