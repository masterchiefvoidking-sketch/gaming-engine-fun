# human Module

## Purpose

Digital human

## Architecture Overview

Character creator, wardrobe, anime visuals

## Dependencies

- `eve::core`
- `eve::ecs`
- `eve::render`
- `eve::animation`
- `eve::world`

## Build Target

```cmake
eve::human
```

## Public API Reference

Headers live under `include/eve/human/`. See each header for class documentation.

## Usage Examples

```cpp
// See tests/human/ and apps/ for integration examples.
```

## Extension Guide

Add new types in `include/eve/human/` and register sources in `CMakeLists.txt`.
Follow `docs/CODING_STANDARDS.md` and keep module boundaries clean.

## Known Limitations

See Audit/TechnicalDebtRegister.md for module-specific debt.

## Tests

- `tests/human/`

## Milestone Status

- [x] API stable (freeze)
- [x] Documented
- [ ] Benchmarked (see benchmarks/)
