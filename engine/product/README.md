# product Module

## Purpose

Release tooling

## Architecture Overview

Release checklist, stress test, bundle

## Dependencies

- `eve::core`
- `eve::build`
- `eve::platform`
- `eve::content`
- `eve::gameplay`
- `eve::polish`
- `eve::editor`

## Build Target

```cmake
eve::product
```

## Public API Reference

Headers live under `include/eve/product/`. See each header for class documentation.

## Usage Examples

```cpp
// See tests/product/ and apps/ for integration examples.
```

## Extension Guide

Add new types in `include/eve/product/` and register sources in `CMakeLists.txt`.
Follow `docs/CODING_STANDARDS.md` and keep module boundaries clean.

## Known Limitations

See Audit/TechnicalDebtRegister.md for module-specific debt.

## Tests

- `tests/product/`

## Milestone Status

- [x] API stable (freeze)
- [x] Documented
- [ ] Benchmarked (see benchmarks/)
