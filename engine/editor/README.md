# editor Module

## Purpose

Authoring tools

## Architecture Overview

Creator Studio, apartment builder, tool panels

## Dependencies

- `eve::core`
- `eve::ecs`
- `eve::render`
- `eve::world`
- `eve::ai`
- `eve::scene`
- `eve::content`
- `eve::build`
- `eve::scripting`
- `eve::human`
- `eve::animation`

## Build Target

```cmake
eve::editor
```

## Public API Reference

Headers live under `include/eve/editor/`. See each header for class documentation.

## Usage Examples

```cpp
// See tests/editor/ and apps/ for integration examples.
```

## Extension Guide

Add new types in `include/eve/editor/` and register sources in `CMakeLists.txt`.
Follow `docs/CODING_STANDARDS.md` and keep module boundaries clean.

## Known Limitations

CLI-only; GUI viewport planned post-freeze.

## Tests

- `tests/editor/`

## Milestone Status

- [x] API stable (freeze)
- [x] Documented
- [ ] Benchmarked (see benchmarks/)
