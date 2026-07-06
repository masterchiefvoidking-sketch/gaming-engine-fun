# animation Module

## Purpose

Animation subsystem

## Architecture Overview

State machines, blend trees, IK, facial animation

## Dependencies

- `eve::core`

## Build Target

```cmake
eve::animation
```

## Public API Reference

Headers live under `include/eve/animation/`. See each header for class documentation.

## Usage Examples

```cpp
// See tests/animation/ and apps/ for integration examples.
```

## Extension Guide

Add new types in `include/eve/animation/` and register sources in `CMakeLists.txt`.
Follow `docs/CODING_STANDARDS.md` and keep module boundaries clean.

## Known Limitations

See Audit/TechnicalDebtRegister.md for module-specific debt.

## Tests

- `tests/animation/`

## Milestone Status

- [x] API stable (freeze)
- [x] Documented
- [ ] Benchmarked (see benchmarks/)
