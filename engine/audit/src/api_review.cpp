#include <eve/audit/api_review.hpp>

namespace eve::audit {

std::vector<ApiReviewEntry> ApiReviewer::review() const {
    return {
        {"content", "ClothingCategory", "Duplicated in ai::wardrobe and content::wardrobe_database with different members",
         "Extract shared enum to eve::content/types.hpp"},
        {"human", "character_profile.hpp", "Header name does not match CharacterCreator class",
         "Rename to character_creator.hpp or split profile types"},
        {"editor", "DialogueGraphEditor vs DialogueGraphEditorPanel", "Confusing duplicate editor class names",
         "Unify naming: DialogueGraphEditorPanel everywhere"},
        {"build + product", "ProjectArchive vs ProjectArchiveEnhanced", "Overlapping archive APIs",
         "Keep Enhanced as sole public API; deprecate basic wrapper"},
        {"polish + product", "BugTracker vs BugTriage", "Duplicate bug tracking APIs",
         "Consolidate into product::BugTriage"},
        {"ai + human + content", "Wardrobe types", "Three parallel wardrobe type systems",
         "Introduce WardrobeTypes shared header with mapping adapters"},
        {"core", "PluginManager / JobSystem / ReflectionRegistry", "Test-only APIs with singleton globals",
         "Wire into runtime or mark experimental in docs"},
        {"scripting", "PythonScriptHost", "Stub implementation with empty bridge",
         "Document as future extension point; no public promise yet"},
        {"physics", "eve::physics", "INTERFACE target with no implementation",
         "Remove from public API surface until implemented"},
        {"platform", "UnifiedSaveSystem", "Well-documented, consistent error handling via bool returns",
         "Model for other subsystems — prefer bool + logging over exceptions"},
    };
}

} // namespace eve::audit
