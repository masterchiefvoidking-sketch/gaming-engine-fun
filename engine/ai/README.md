# ai Module

## Purpose

AI and romance simulation

## Architecture Overview

Relationship, dialogue, schedules, mood

## Dependencies

- `eve::core`
- `eve::ecs`
- `eve::world`

## Build Target

```cmake
eve::ai
```

## Public API Reference

Headers live under `include/eve/ai/`. See each header for class documentation.

## Usage Examples

```cpp
// See tests/ai/ and apps/ for integration examples.
```

## Extension Guide

Add new types in `include/eve/ai/` and register sources in `CMakeLists.txt`.
Follow `docs/CODING_STANDARDS.md` and keep module boundaries clean.

## Known Limitations

See Audit/TechnicalDebtRegister.md for module-specific debt.

## Tests

- `tests/ai/`

## Milestone Status

- [x] API stable (freeze)
- [x] Documented
- [ ] Benchmarked (see benchmarks/)
