# interaction Module

## Purpose

Player interaction

## Architecture Overview

Input, web viewer, inspection mode

## Dependencies

- `eve::core`
- `eve::render`
- `eve::human`
- `eve::world`
- `eve::platform`
- `eve::ai`

## Build Target

```cmake
eve::interaction
```

## Public API Reference

Headers live under `include/eve/interaction/`. See each header for class documentation.

## Usage Examples

```cpp
// See tests/interaction/ and apps/ for integration examples.
```

## Extension Guide

Add new types in `include/eve/interaction/` and register sources in `CMakeLists.txt`.
Follow `docs/CODING_STANDARDS.md` and keep module boundaries clean.

## Known Limitations

See Audit/TechnicalDebtRegister.md for module-specific debt.

## Tests

- `tests/interaction/`

## Milestone Status

- [x] API stable (freeze)
- [x] Documented
- [ ] Benchmarked (see benchmarks/)
