# polish Module

## Purpose

Polish and validation

## Architecture Overview

Performance profiler, validation gate, graphics polish

## Dependencies

- `eve::core`
- `eve::render`
- `eve::human`
- `eve::animation`
- `eve::interaction`
- `eve::world`
- `eve::build`
- `eve::platform`
- `eve::ai`
- `eve::content`

## Build Target

```cmake
eve::polish
```

## Public API Reference

Headers live under `include/eve/polish/`. See each header for class documentation.

## Usage Examples

```cpp
// See tests/polish/ and apps/ for integration examples.
```

## Extension Guide

Add new types in `include/eve/polish/` and register sources in `CMakeLists.txt`.
Follow `docs/CODING_STANDARDS.md` and keep module boundaries clean.

## Known Limitations

See Audit/TechnicalDebtRegister.md for module-specific debt.

## Tests

- `tests/polish/`

## Milestone Status

- [x] API stable (freeze)
- [x] Documented
- [ ] Benchmarked (see benchmarks/)
