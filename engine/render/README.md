# render Module

## Purpose

Vulkan renderer

## Architecture Overview

Cameras, shaders, photo mode, Vulkan context

## Dependencies

- `eve::core`

## Build Target

```cmake
eve::render
```

## Public API Reference

Headers live under `include/eve/render/`. See each header for class documentation.

## Usage Examples

```cpp
// See tests/render/ and apps/ for integration examples.
```

## Extension Guide

Add new types in `include/eve/render/` and register sources in `CMakeLists.txt`.
Follow `docs/CODING_STANDARDS.md` and keep module boundaries clean.

## Known Limitations

See Audit/TechnicalDebtRegister.md for module-specific debt.

## Tests

- `tests/render/`

## Milestone Status

- [x] API stable (freeze)
- [x] Documented
- [ ] Benchmarked (see benchmarks/)
