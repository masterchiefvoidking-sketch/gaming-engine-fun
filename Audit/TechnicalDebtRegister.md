# Technical Debt Register

| ID | Severity | Module | Description | Remediation |
|----|----------|--------|-------------|-------------|
| TD-001 | medium | Data layer depends on ai simulation types | content | Extract shared content types |
| TD-002 | medium | Wardrobe triple-stack across modules | human | Single wardrobe domain model |
| TD-003 | low | Unused INTERFACE module | physics | Remove or implement |
| TD-004 | low | PluginManager/JobSystem test-only | core | Wire to runtime or mark experimental |
| TD-005 | medium | CLI-only editor limits UX | editor | GUI viewport planned post-freeze |
| TD-006 | low | Tests link demo app libraries | tests | Extract shared test fixtures |
