# platform Module

## Purpose

Platform services

## Architecture Overview

Unified save, web deploy, backups

## Dependencies

- `eve::core`
- `eve::ai`
- `eve::content`

## Build Target

```cmake
eve::platform
```

## Public API Reference

Headers live under `include/eve/platform/`. See each header for class documentation.

## Usage Examples

```cpp
// See tests/platform/ and apps/ for integration examples.
```

## Extension Guide

Add new types in `include/eve/platform/` and register sources in `CMakeLists.txt`.
Follow `docs/CODING_STANDARDS.md` and keep module boundaries clean.

## Known Limitations

See Audit/TechnicalDebtRegister.md for module-specific debt.

## Tests

- `tests/platform/`

## Milestone Status

- [x] API stable (freeze)
- [x] Documented
- [ ] Benchmarked (see benchmarks/)
