# ecs Module

## Purpose

Entity-component-system

## Architecture Overview

World and component registry

## Dependencies

- `eve::core`

## Build Target

```cmake
eve::ecs
```

## Public API Reference

Headers live under `include/eve/ecs/`. See each header for class documentation.

## Usage Examples

```cpp
// See tests/ecs/ and apps/ for integration examples.
```

## Extension Guide

Add new types in `include/eve/ecs/` and register sources in `CMakeLists.txt`.
Follow `docs/CODING_STANDARDS.md` and keep module boundaries clean.

## Known Limitations

See Audit/TechnicalDebtRegister.md for module-specific debt.

## Tests

- `tests/ecs/`

## Milestone Status

- [x] API stable (freeze)
- [x] Documented
- [ ] Benchmarked (see benchmarks/)
