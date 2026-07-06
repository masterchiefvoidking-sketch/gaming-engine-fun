#include <eve/render/vulkan_context.hpp>
#include <gtest/gtest.h>

TEST(VulkanTest, InitializesWithoutWindow) {
    eve::render::VulkanContext context;
    ASSERT_TRUE(context.initialize("EVE Test", false));
    EXPECT_TRUE(context.is_initialized());
    EXPECT_FALSE(context.capabilities().device_name.empty());
    context.shutdown();
    EXPECT_FALSE(context.is_initialized());
}
