#include <cstdlib>
#include <eve/editor/editor_application.hpp>
#include <gtest/gtest.h>

TEST(EditorTest, InitializesShell) {
    if (std::getenv("EVE_SKIP_GRAPHICAL_TESTS") != nullptr) {
        GTEST_SKIP() << "Graphical tests disabled";
    }

    eve::editor::EditorApplication editor;
    eve::editor::EditorConfig config{};
    config.width = 640;
    config.height = 480;
    config.title = "Test Editor";

    ASSERT_TRUE(editor.initialize(config));
    EXPECT_TRUE(editor.is_running());
    EXPECT_NE(editor.window(), nullptr);
    editor.shutdown();
}
