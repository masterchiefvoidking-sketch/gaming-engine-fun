# core Module

## Purpose

Foundation layer

## Architecture Overview

Memory, logging, math, serialization, filesystem, threading, plugins

## Dependencies

- None

## Build Target

```cmake
eve::core
```

## Public API Reference

Headers live under `include/eve/core/`. See each header for class documentation.

## Usage Examples

```cpp
// See tests/core/ and apps/ for integration examples.
```

## Extension Guide

Add new types in `include/eve/core/` and register sources in `CMakeLists.txt`.
Follow `docs/CODING_STANDARDS.md` and keep module boundaries clean.

## Known Limitations

See Audit/TechnicalDebtRegister.md for module-specific debt.

## Tests

- `tests/core/`

## Milestone Status

- [x] API stable (freeze)
- [x] Documented
- [ ] Benchmarked (see benchmarks/)
