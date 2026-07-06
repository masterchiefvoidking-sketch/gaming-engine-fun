#include <eve/core/filesystem/filesystem.hpp>
#include <filesystem>
#include <gtest/gtest.h>

TEST(FileSystemTest, WritesAndReadsText) {
    const std::string path = "eve_test_file.txt";
    ASSERT_TRUE(eve::FileSystem::write_text_file(path, "hello"));
    EXPECT_TRUE(eve::FileSystem::exists(path));
    EXPECT_EQ(eve::FileSystem::read_text_file(path), "hello");
    std::filesystem::remove(path);
}

TEST(FileSystemTest, CreatesDirectories) {
    const std::string path = "eve_test_dir/nested";
    ASSERT_TRUE(eve::FileSystem::create_directories(path));
    EXPECT_TRUE(eve::FileSystem::is_directory(path));
    std::filesystem::remove_all("eve_test_dir");
}
