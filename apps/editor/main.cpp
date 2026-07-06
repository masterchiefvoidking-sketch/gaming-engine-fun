#include <eve/core/logging/logger.hpp>
#include <eve/editor/editor_application.hpp>

int main() {
    eve::Logger::instance().set_level(eve::LogLevel::Info);
    EVE_LOG(Info, "Engine", eve::kEngineName, " v", eve::kEngineVersion);

    eve::editor::EditorApplication editor;
    eve::editor::EditorConfig config{};
    config.title = "EVE Editor 0.1";
    config.enable_validation =
#if defined(EVE_VULKAN_VALIDATION)
        true;
#else
        false;
#endif

    if (!editor.initialize(config)) {
        EVE_LOG(Error, "Editor", "Failed to initialize editor shell");
        return 1;
    }

    editor.run();
    editor.shutdown();
    return 0;
}
