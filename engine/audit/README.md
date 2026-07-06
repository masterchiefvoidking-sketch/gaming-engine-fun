# audit Module

## Purpose

Architecture audit

## Architecture Overview

Engineering audit and reporting

## Dependencies

- `eve::core`
- `eve::content`
- `eve::human`
- `eve::platform`
- `eve::build`
- `eve::ai`
- `eve::gameplay`

## Build Target

```cmake
eve::audit
```

## Public API Reference

Headers live under `include/eve/audit/`. See each header for class documentation.

## Usage Examples

```cpp
// See tests/audit/ and apps/ for integration examples.
```

## Extension Guide

Add new types in `include/eve/audit/` and register sources in `CMakeLists.txt`.
Follow `docs/CODING_STANDARDS.md` and keep module boundaries clean.

## Known Limitations

See Audit/TechnicalDebtRegister.md for module-specific debt.

## Tests

- `tests/audit/`

## Milestone Status

- [x] API stable (freeze)
- [x] Documented
- [ ] Benchmarked (see benchmarks/)
