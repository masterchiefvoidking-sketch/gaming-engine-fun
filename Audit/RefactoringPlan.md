# Refactoring Plan

## Priority 1 — Type consolidation

1. Unify `ClothingCategory` across ai, content, human
2. Consolidate wardrobe runtime/authoring types
3. Merge `BugTracker` and `BugTriage`

## Priority 2 — Coupling reduction

1. Invert content→ai dependency
2. Extract interfaces from hub modules (gameplay, editor, product)
3. Decouple tests from demo app libraries

## Priority 3 — Dead code

1. Implement or remove `physics` INTERFACE module
2. Wire or document core singletons (PluginManager, JobSystem)
3. Complete scripting Python bridge or mark experimental

## API cleanup

- **content** `ClothingCategory`: Extract shared enum to eve::content/types.hpp
- **human** `character_profile.hpp`: Rename to character_creator.hpp or split profile types
- **editor** `DialogueGraphEditor vs DialogueGraphEditorPanel`: Unify naming: DialogueGraphEditorPanel everywhere
- **build + product** `ProjectArchive vs ProjectArchiveEnhanced`: Keep Enhanced as sole public API; deprecate basic wrapper
- **polish + product** `BugTracker vs BugTriage`: Consolidate into product::BugTriage
- **ai + human + content** `Wardrobe types`: Introduce WardrobeTypes shared header with mapping adapters
- **core** `PluginManager / JobSystem / ReflectionRegistry`: Wire into runtime or mark experimental in docs
- **scripting** `PythonScriptHost`: Document as future extension point; no public promise yet
- **physics** `eve::physics`: Remove from public API surface until implemented
- **platform** `UnifiedSaveSystem`: Model for other subsystems — prefer bool + logging over exceptions
