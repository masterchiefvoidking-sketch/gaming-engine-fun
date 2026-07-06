# EVE Engine Architecture

## Overview

EVE Adult Engine is organized as a set of layered modules with explicit dependencies. Each module exposes a public API under `engine/<module>/include/eve/<module>/` and compiles to a static or interface library target `eve::<module>`.

## Layer Model

```
┌─────────────────────────────────────────────┐
│  apps/          Editor, tools, samples      │
├─────────────────────────────────────────────┤
│  editor         Docking UI shell            │
├─────────────────────────────────────────────┤
│  world, ai, human, animation, physics       │
├─────────────────────────────────────────────┤
│  render         Vulkan, materials, shaders  │
├─────────────────────────────────────────────┤
│  ecs            Entity-component-system       │
├─────────────────────────────────────────────┤
│  core           Foundation utilities          │
└─────────────────────────────────────────────┘
```

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
| `render` | GPU presentation | `VulkanContext` |
| `editor` | Authoring shell | `EditorApplication` |

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

## Threading Model

- **Main thread** — window events, editor UI, render submission
- **Job system** — parallel asset processing, animation pose search, offline baking
- **Render thread** (future) — optional dedicated submission queue

## Memory Strategy

- **Linear allocators** — per-frame transient allocations
- **Pool allocators** — homogeneous component pools (future ECS optimization)
- **Persistent assets** — reference-counted GPU resources (milestone 2)
