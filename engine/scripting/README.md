# scripting Module

## Purpose

Script host abstraction

## Architecture Overview

ScriptEngine and Python bridge stub

## Dependencies

- `eve::core`

## Build Target

```cmake
eve::scripting
```

## Public API Reference

Headers live under `include/eve/scripting/`. See each header for class documentation.

## Usage Examples

```cpp
// See tests/scripting/ and apps/ for integration examples.
```

## Extension Guide

Add new types in `include/eve/scripting/` and register sources in `CMakeLists.txt`.
Follow `docs/CODING_STANDARDS.md` and keep module boundaries clean.

## Known Limitations

Python bridge is a stub.

## Tests

- `tests/scripting/`

## Milestone Status

- [x] API stable (freeze)
- [x] Documented
- [ ] Benchmarked (see benchmarks/)
