# gameplay Module

## Purpose

Gameplay orchestration

## Architecture Overview

ApartmentLifeSession integrated demo

## Dependencies

- `eve::core`
- `eve::content`
- `eve::ai`
- `eve::scene`
- `eve::interaction`
- `eve::platform`
- `eve::build`
- `eve::human`
- `eve::animation`
- `eve::world`
- `eve::polish`

## Build Target

```cmake
eve::gameplay
```

## Public API Reference

Headers live under `include/eve/gameplay/`. See each header for class documentation.

## Usage Examples

```cpp
// See tests/gameplay/ and apps/ for integration examples.
```

## Extension Guide

Add new types in `include/eve/gameplay/` and register sources in `CMakeLists.txt`.
Follow `docs/CODING_STANDARDS.md` and keep module boundaries clean.

## Known Limitations

See Audit/TechnicalDebtRegister.md for module-specific debt.

## Tests

- `tests/gameplay/`

## Milestone Status

- [x] API stable (freeze)
- [x] Documented
- [ ] Benchmarked (see benchmarks/)
