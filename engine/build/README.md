# build Module

## Purpose

Build and export

## Architecture Overview

Cook, validate, archive, migrate

## Dependencies

- `eve::core`
- `eve::content`
- `eve::platform`

## Build Target

```cmake
eve::build
```

## Public API Reference

Headers live under `include/eve/build/`. See each header for class documentation.

## Usage Examples

```cpp
// See tests/build/ and apps/ for integration examples.
```

## Extension Guide

Add new types in `include/eve/build/` and register sources in `CMakeLists.txt`.
Follow `docs/CODING_STANDARDS.md` and keep module boundaries clean.

## Known Limitations

See Audit/TechnicalDebtRegister.md for module-specific debt.

## Tests

- `tests/build/`

## Milestone Status

- [x] API stable (freeze)
- [x] Documented
- [ ] Benchmarked (see benchmarks/)
