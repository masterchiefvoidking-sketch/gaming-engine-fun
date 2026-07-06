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
| `EVE_ENABLE_BENCHMARKS` | OFF | Build subsystem benchmarks |

## Run the Editor

```bash
./build/apps/editor/eve-editor
```

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
