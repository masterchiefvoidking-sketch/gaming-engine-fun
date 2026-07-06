# content Module

## Purpose

Content database

## Architecture Overview

Project layout, wardrobe DB, hot reload

## Dependencies

- `eve::core`
- `eve::ai`

## Build Target

```cmake
eve::content
```

## Public API Reference

Headers live under `include/eve/content/`. See each header for class documentation.

## Usage Examples

```cpp
// See tests/content/ and apps/ for integration examples.
```

## Extension Guide

Add new types in `include/eve/content/` and register sources in `CMakeLists.txt`.
Follow `docs/CODING_STANDARDS.md` and keep module boundaries clean.

## Known Limitations

Depends on ai types — technical debt TD-001.

## Tests

- `tests/content/`

## Milestone Status

- [x] API stable (freeze)
- [x] Documented
- [ ] Benchmarked (see benchmarks/)
