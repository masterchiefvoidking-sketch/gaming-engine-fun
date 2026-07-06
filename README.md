# EVE Adult Engine

**Version 0.1** — Foundation milestone

EVE is an open-source engine for mature, story-driven, character-focused experiences (18+). This repository establishes the modular foundation for decades of development across rendering, animation, human characters, physics, AI, world building, and editor tooling.

## Philosophy

- **Beauty** — cinematic presentation and expressive digital humans
- **Expression** — facial animation, emotion, and relationship simulation
- **Realism** — PBR materials, advanced skin/hair/cloth shading
- **Performance** — benchmarked subsystems and fast iteration
- **Modularity** — clean boundaries between engine modules

## Repository Layout

```
engine/
  core/         Memory, logging, math, reflection, serialization, filesystem, threading, plugins
  ecs/          Entity-component-system foundation
  render/       Vulkan renderer initialization
  editor/       Editor shell library
  animation/    Scaffolded (milestone 2)
  human/        Scaffolded (milestone 2)
  physics/      Scaffolded (milestone 2)
  ai/           Scaffolded (milestone 2)
  world/        Scaffolded (milestone 2)
apps/editor/    EVE Editor executable
tests/          Automated subsystem tests
docs/           Architecture, coding standards, templates
cmake/          Build configuration modules
```

## Requirements

- CMake 3.24+
- C++20 compiler (GCC 12+, Clang 15+, MSVC 2022+)
- Vulkan SDK
- X11/Wayland development libraries (Linux editor builds)

### Linux dependencies (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install -y \
  build-essential g++-12 cmake ninja-build \
  libvulkan-dev vulkan-tools \
  libx11-dev libxrandr-dev libxi-dev libxxf86vm-dev libxcb1-dev \
  libwayland-dev wayland-protocols pkg-config libxkbcommon-dev \
  libxinerama-dev libxcursor-dev libgl1-mesa-dev
```

## Build

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++-12
cmake --build build
ctest --test-dir build --output-on-failure
```

### Options

| Option | Default | Description |
|--------|---------|-------------|
| `EVE_BUILD_EDITOR` | ON | Build the editor application |
| `EVE_BUILD_TESTS` | ON | Build automated tests |
| `EVE_ENABLE_VULKAN_VALIDATION` | OFF | Enable Vulkan validation layers |
| `EVE_BUILD_RC1_RELEASE` | ON | RC1 release pipeline |
| `EVE_BUILD_PERSONAL_PRODUCT_CUT` | ON | Personal product cut (Phase 14) |

## Run the Editor

```bash
./build/apps/editor/eve-editor
```

## Personal Product Cut (Phase 14)

```bash
./build/apps/personal_product_cut/eve-personal-product-cut
```

See [docs/PHASE14_PERSONAL_PRODUCT_CUT.md](docs/PHASE14_PERSONAL_PRODUCT_CUT.md) for the full personal-use workflow.

## Testing

```bash
ctest --test-dir build --output-on-failure
```

Set `EVE_SKIP_GRAPHICAL_TESTS=1` to skip windowed editor initialization tests in headless environments.

## Documentation

- [Architecture](docs/ARCHITECTURE.md)
- [Coding Standards](docs/CODING_STANDARDS.md)
- [Module Template](docs/templates/MODULE.md)
- [Subsystem Template](docs/templates/SUBSYSTEM.md)

## License

License to be determined. Contributions welcome once governance is established.

## Status

This is the **first milestone**: repository structure, CMake, core systems, ECS, Vulkan initialization, editor shell, tests, and CI. Rendering, UI docking, animation, and character systems are scaffolded for subsequent milestones.

## Phase 2 — Apartment Life Simulation

See [Phase 2 Documentation](docs/PHASE2_APARTMENT_SIM.md) for the interactive apartment framework, relationship system, social AI, data-driven interactions, and scripting hooks.

## Phase 3 — Digital Human Framework

See [Phase 3 Documentation](docs/PHASE3_DIGITAL_HUMAN.md) for character customization, inspection camera, animation state machine, PBR human shaders, hair rendering, photo mode, and the example apartment scene.
