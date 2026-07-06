# world Module

## Purpose

World simulation

## Architecture Overview

Apartment, interactions, persistence

## Dependencies

- `eve::core`
- `eve::ecs`

## Build Target

```cmake
eve::world
```

## Public API Reference

Headers live under `include/eve/world/`. See each header for class documentation.

## Usage Examples

```cpp
// See tests/world/ and apps/ for integration examples.
```

## Extension Guide

Add new types in `include/eve/world/` and register sources in `CMakeLists.txt`.
Follow `docs/CODING_STANDARDS.md` and keep module boundaries clean.

## Known Limitations

See Audit/TechnicalDebtRegister.md for module-specific debt.

## Tests

- `tests/world/`

## Milestone Status

- [x] API stable (freeze)
- [x] Documented
- [ ] Benchmarked (see benchmarks/)
