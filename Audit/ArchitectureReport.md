# Architecture Report

Phase 15 engineering audit — module health inventory.

## Summary

- Modules audited: 19
- CMake link cycles: **none detected**
- Feature freeze: active (bug fixes, refactoring, docs, tests only)

## ai

| Field | Value |
|-------|-------|
| Purpose | Relationship simulation, dialogue, schedules, mood |
| Lines of Code | 2731 |
| Cyclomatic Complexity (est.) | 295 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies | eve::core, eve::ecs, eve::world |

### Public APIs

- `adaptive_schedule.hpp`
- `boundaries.hpp`
- `character_sim_state.hpp`
- `dialogue_engine.hpp`
- `memory_system.hpp`
- `module.hpp`
- `mood_system.hpp`
- `outfit_reactions.hpp`
- `relationship.hpp`
- `relationship_simulation.hpp`
- `relationship_stages.hpp`
- `romance_events.hpp`
- `romance_hooks.hpp`
- `room_behavior.hpp`
- `scene_interruption.hpp`
- `schedule.hpp`
- `social_ai.hpp`
- `wardrobe.hpp`

## animation

| Field | Value |
|-------|-------|
| Purpose | State machines, blend trees, IK, facial animation |
| Lines of Code | 621 |
| Cyclomatic Complexity (est.) | 69 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies | eve::core |

### Public APIs

- `anime_motion.hpp`
- `blend_tree.hpp`
- `facial_animation.hpp`
- `ik_solver.hpp`
- `layer_stack.hpp`
- `module.hpp`
- `state_machine.hpp`

## audit

| Field | Value |
|-------|-------|
| Purpose | Architecture freeze audit and reporting |
| Lines of Code | 978 |
| Cyclomatic Complexity (est.) | 76 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies | eve::ai, eve::build, eve::content, eve::core, eve::gameplay, eve::human, eve::platform |

### Public APIs

- `api_review.hpp`
- `architecture_freeze.hpp`
- `dependency_review.hpp`
- `memory_review.hpp`
- `module_health.hpp`
- `performance_review.hpp`
- `test_coverage.hpp`
- `types.hpp`

### Known Issues

- Hub module with 7 dependencies

### Improvement Recommendations

- Extract interfaces to reduce coupling

## build

| Field | Value |
|-------|-------|
| Purpose | Export pipeline: cook, validate, archive, migrate |
| Lines of Code | 1121 |
| Cyclomatic Complexity (est.) | 119 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies | eve::content, eve::core, eve::platform |

### Public APIs

- `asset_cooker.hpp`
- `build_pipeline.hpp`
- `content_validator.hpp`
- `dependency_manifest.hpp`
- `export_profiles.hpp`
- `personal_backup_tool.hpp`
- `personal_content_pack.hpp`
- `project_archive.hpp`
- `save_migration.hpp`

## content

| Field | Value |
|-------|-------|
| Purpose | Content database, project layout, mod system, wardrobe DB |
| Lines of Code | 1670 |
| Cyclomatic Complexity (est.) | 179 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies | eve::ai, eve::core |

### Public APIs

- `apartment_content.hpp`
- `asset_import.hpp`
- `character_content.hpp`
- `content_database.hpp`
- `content_types.hpp`
- `dialogue_content.hpp`
- `game_content_project.hpp`
- `hot_reload.hpp`
- `localization.hpp`
- `mod_system.hpp`
- `packaging.hpp`
- `project_layout.hpp`
- `undo_stack.hpp`
- `wardrobe_database.hpp`

### Known Issues

- Data layer depends on simulation layer (ai)

### Improvement Recommendations

- Invert dependency via content-only types

## core

| Field | Value |
|-------|-------|
| Purpose | Foundation: memory, logging, math, serialization, filesystem, threading |
| Lines of Code | 1480 |
| Cyclomatic Complexity (est.) | 106 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies |  |

### Public APIs

- `allocator.hpp`
- `filesystem.hpp`
- `job_system.hpp`
- `logger.hpp`
- `math.hpp`
- `plugin_manager.hpp`
- `reflection.hpp`
- `serializer.hpp`
- `types.hpp`

## ecs

| Field | Value |
|-------|-------|
| Purpose | Entity-component-system world and registry |
| Lines of Code | 203 |
| Cyclomatic Complexity (est.) | 2 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies | eve::core |

### Public APIs

- `world.hpp`

## editor

| Field | Value |
|-------|-------|
| Purpose | Authoring shell: creator studio, apartment builder, tools |
| Lines of Code | 1505 |
| Cyclomatic Complexity (est.) | 50 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies | eve::ai, eve::animation, eve::build, eve::content, eve::core, eve::ecs, eve::human, eve::render, eve::scene, eve::scripting, eve::world |

### Public APIs

- `anime_visual_tools.hpp`
- `apartment_builder.hpp`
- `creator_studio.hpp`
- `editor_application.hpp`
- `export_panel.hpp`
- `interaction_editor.hpp`
- `relationship_inspector.hpp`
- `romance_sim_tools.hpp`
- `scene_director_tools.hpp`
- `schedule_editor.hpp`
- `wardrobe_tools.hpp`

### Known Issues

- Hub module with 11 dependencies

### Improvement Recommendations

- Extract interfaces to reduce coupling

## gameplay

| Field | Value |
|-------|-------|
| Purpose | Integrated gameplay session orchestration |
| Lines of Code | 577 |
| Cyclomatic Complexity (est.) | 46 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies | eve::ai, eve::animation, eve::build, eve::content, eve::core, eve::human, eve::interaction, eve::platform, eve::polish, eve::scene, eve::world |

### Public APIs

- `apartment_life_session.hpp`

### Known Issues

- Hub module with 11 dependencies

### Improvement Recommendations

- Extract interfaces to reduce coupling

## human

| Field | Value |
|-------|-------|
| Purpose | Character creator, wardrobe system, anime visuals |
| Lines of Code | 2411 |
| Cyclomatic Complexity (est.) | 302 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies | eve::animation, eve::core, eve::ecs, eve::render, eve::world |

### Public APIs

- `anime_character_style.hpp`
- `anime_expression_system.hpp`
- `anime_visual_pipeline.hpp`
- `body_interaction_system.hpp`
- `body_regions.hpp`
- `character_framework.hpp`
- `character_interaction.hpp`
- `character_interaction_system.hpp`
- `character_profile.hpp`
- `dressing_room.hpp`
- `interaction_rules.hpp`
- `module.hpp`
- `photo_mode_bridge.hpp`
- `render_sync.hpp`
- `wardrobe_system.hpp`

## interaction

| Field | Value |
|-------|-------|
| Purpose | Input, web viewer, inspection mode, UI state |
| Lines of Code | 1504 |
| Cyclomatic Complexity (est.) | 103 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies | eve::ai, eve::core, eve::human, eve::platform, eve::render, eve::world |

### Public APIs

- `browser_storage.hpp`
- `camera_input_bridge.hpp`
- `character_inspection_mode.hpp`
- `input_controller.hpp`
- `input_types.hpp`
- `interaction_ring.hpp`
- `photo_mode_controller.hpp`
- `player_ui_state.hpp`
- `quality_settings.hpp`
- `responsive_ui_layout.hpp`
- `web_character_viewer.hpp`
- `web_performance_profile.hpp`

## physics

| Field | Value |
|-------|-------|
| Purpose | Placeholder interface for future physics integration |
| Lines of Code | 7 |
| Cyclomatic Complexity (est.) | 0 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies | eve::core |

### Public APIs

- `module.hpp`

### Known Issues

- INTERFACE stub only — no implementation

### Improvement Recommendations

- Implement or remove before physics features

## platform

| Field | Value |
|-------|-------|
| Purpose | Unified save, web deploy, mobile input, backups |
| Lines of Code | 1047 |
| Cyclomatic Complexity (est.) | 83 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies | eve::ai, eve::content, eve::core |

### Public APIs

- `backup_manager.hpp`
- `engine_mission.hpp`
- `game_session.hpp`
- `local_asset_library.hpp`
- `mobile_input.hpp`
- `project_templates.hpp`
- `unified_save.hpp`
- `web_publisher.hpp`

## polish

| Field | Value |
|-------|-------|
| Purpose | Performance profiling, validation gate, graphics polish |
| Lines of Code | 1743 |
| Cyclomatic Complexity (est.) | 125 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies | eve::ai, eve::animation, eve::build, eve::content, eve::core, eve::human, eve::interaction, eve::platform, eve::render, eve::world |

### Public APIs

- `animation_polish.hpp`
- `apartment_atmosphere.hpp`
- `bug_tracker.hpp`
- `camera_polish.hpp`
- `dialogue_polish.hpp`
- `graphics_polish.hpp`
- `performance_profiler.hpp`
- `rc1_release.hpp`
- `validation_gate.hpp`
- `wardrobe_polish.hpp`

### Known Issues

- Hub module with 10 dependencies

### Improvement Recommendations

- Extract interfaces to reduce coupling

## product

| Field | Value |
|-------|-------|
| Purpose | Release tooling: checklist, stress test, bundle |
| Lines of Code | 1076 |
| Cyclomatic Complexity (est.) | 77 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies | eve::build, eve::content, eve::core, eve::editor, eve::gameplay, eve::platform, eve::polish |

### Public APIs

- `bug_triage.hpp`
- `content_completion.hpp`
- `personal_product_cut.hpp`
- `project_archive_enhanced.hpp`
- `release_bundle.hpp`
- `release_checklist.hpp`
- `save_stress_tester.hpp`

### Known Issues

- Hub module with 7 dependencies

### Improvement Recommendations

- Extract interfaces to reduce coupling

## render

| Field | Value |
|-------|-------|
| Purpose | Vulkan renderer, cameras, shaders, photo mode |
| Lines of Code | 1901 |
| Cyclomatic Complexity (est.) | 150 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies | eve::core |

### Public APIs

- `anime_eye_system.hpp`
- `anime_hair_renderer.hpp`
- `anime_human_material.hpp`
- `anime_outline_pass.hpp`
- `hair_renderer.hpp`
- `human_shader_pipeline.hpp`
- `inspection_camera.hpp`
- `mirror_renderer.hpp`
- `photo_mode.hpp`
- `romance_camera.hpp`
- `vulkan_context.hpp`

## scene

| Field | Value |
|-------|-------|
| Purpose | Scene director, cues, event scheduling |
| Lines of Code | 2071 |
| Cyclomatic Complexity (est.) | 249 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies | eve::ai, eve::animation, eve::core, eve::human, eve::render, eve::world |

### Public APIs

- `animation_cue_system.hpp`
- `camera_shot_sequencer.hpp`
- `dialogue_scene_sequencer.hpp`
- `emotional_outcomes.hpp`
- `event_scheduler.hpp`
- `lighting_cue_system.hpp`
- `music_cue_system.hpp`
- `room_event_triggers.hpp`
- `scene_conditions.hpp`
- `scene_director.hpp`
- `scene_state_machine.hpp`
- `scene_types.hpp`

## scripting

| Field | Value |
|-------|-------|
| Purpose | Script host abstraction and Python bridge stub |
| Lines of Code | 173 |
| Cyclomatic Complexity (est.) | 6 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies | eve::core |

### Public APIs

- `python_bridge.hpp`
- `script_host.hpp`

## world

| Field | Value |
|-------|-------|
| Purpose | Apartment simulation, interactions, persistence |
| Lines of Code | 1197 |
| Cyclomatic Complexity (est.) | 173 |
| Test Coverage (est.) | see Test Coverage Report |
| Dependencies | eve::core, eve::ecs |

### Public APIs

- `anime_room_lighting.hpp`
- `apartment.hpp`
- `apartment_types.hpp`
- `interaction.hpp`
- `interaction_system.hpp`
- `module.hpp`
- `persistence.hpp`
- `simulation.hpp`

