# EVE Coding Standards

## Language

- **C++20** is the project standard.
- Prefer standard library facilities over bespoke utilities when performance is equivalent.

## Naming

| Element | Convention | Example |
|---------|------------|---------|
| Namespaces | `lower_case`, nested by module | `eve::render` |
| Types | `PascalCase` | `VulkanContext` |
| Functions / methods | `snake_case` | `create_entity()` |
| Variables | `snake_case` | `entity_count` |
| Constants | `snake_case` with `k` prefix | `kInvalidEntity` |
| Macros | `EVE_UPPER_SNAKE` | `EVE_LOG` |
| Files | `snake_case` | `vulkan_context.hpp` |

## Headers

- Use `#pragma once` include guards.
- Public headers live in `include/eve/<module>/`.
- Include order: matching header, project headers, third-party, standard library.

## Formatting

Run `clang-format` before committing. Configuration is in `.clang-format` at the repository root.

```bash
find engine tests apps -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i
```

## Error Handling

- Use early returns and logging for recoverable errors.
- Throw `std::exception` derivatives only for unrecoverable setup failures (e.g., out of memory during engine boot).
- Log categories match module names: `"Render"`, `"Editor"`, `"ECS"`.

## Testing

- One test file per subsystem feature area.
- Tests must be deterministic and avoid network dependencies.
- Graphical tests must respect `EVE_SKIP_GRAPHICAL_TESTS`.

## Documentation

- Every public module requires a `MODULE.md` entry when promoted beyond scaffold status.
- Document non-obvious invariants in header comments; avoid narrating obvious code.

## Commits

- Use imperative mood: "Add ECS world destroy path"
- Keep commits focused on a single subsystem or concern.

## Benchmarks

When `EVE_ENABLE_BENCHMARKS` is ON, new performance-critical code should include a benchmark target under `tests/benchmarks/`.
