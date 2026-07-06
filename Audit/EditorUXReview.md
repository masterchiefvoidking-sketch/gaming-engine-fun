# Editor UX Review

## Current State

Creator Studio is CLI-driven with functional editor panels (character, wardrobe, dialogue, events, export).

## Findings

| Area | Status | Recommendation |
|------|--------|----------------|
| Window layout | CLI only | Add docked viewport in future phase |
| Tool discoverability | Moderate | Document panel entry points in EDITOR_GUIDE |
| Keyboard shortcuts | Not implemented | Add shortcut map when GUI lands |
| Search | Asset browser search works | Extend to dialogue/event graphs |
| Undo/Redo | Content undo_stack exists | Wire to all editor panels |
| Autosave | Enabled (120s) | Expose interval in config |
| Error messages | Log-based | Surface validation errors in export panel |
| Import workflow | AssetImporter available | Add drag-drop when GUI lands |
| Export workflow | Export panel validated | Good — keep validation gate |

## Usability Improvements (no new systems)

1. Clearer export panel error messages
2. Validation summary before export
3. Document editor workflow in module READMEs
