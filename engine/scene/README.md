# scene Module

## Purpose

Scene direction

## Architecture Overview

Scene director, cues, event scheduling

## Dependencies

- `eve::core`
- `eve::ai`
- `eve::render`
- `eve::animation`
- `eve::human`
- `eve::world`

## Build Target

```cmake
eve::scene
```

## Public API Reference

Headers live under `include/eve/scene/`. See each header for class documentation.

## Usage Examples

```cpp
// See tests/scene/ and apps/ for integration examples.
```

## Extension Guide

Add new types in `include/eve/scene/` and register sources in `CMakeLists.txt`.
Follow `docs/CODING_STANDARDS.md` and keep module boundaries clean.

## Known Limitations

See Audit/TechnicalDebtRegister.md for module-specific debt.

## Tests

- `tests/scene/`

## Milestone Status

- [x] API stable (freeze)
- [x] Documented
- [ ] Benchmarked (see benchmarks/)
