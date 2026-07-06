# Memory Report

| Category | Est. Bytes | Notes |
|----------|------------|-------|
| Texture memory (metadata) | 926 | JSON material definitions; GPU textures not yet loaded |
| Mesh memory (metadata) | 9898 | Character mesh references in JSON |
| Animation memory | 651 | Animation clip definitions |
| Audio memory | 0 | No audio middleware integrated yet |
| Shader cache | 124 | Cooked shader variant manifest |
| Asset cache | 47317 | Cooked runtime package |
| Save data | 2546 | Unified save JSON files |
| Runtime allocations | 0 | Use PerformanceProfiler and allocator hooks for runtime tracking |
