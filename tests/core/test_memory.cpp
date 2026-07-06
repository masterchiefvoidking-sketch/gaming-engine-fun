#include <eve/core/memory/allocator.hpp>
#include <gtest/gtest.h>

TEST(MemoryTest, LinearAllocatorAllocatesAndResets) {
    eve::LinearAllocator allocator(1024);
    void* a = allocator.allocate(64, 16);
    void* b = allocator.allocate(128, 16);
    ASSERT_NE(a, nullptr);
    ASSERT_NE(b, nullptr);
    EXPECT_GT(allocator.used(), 0u);
    allocator.reset();
    EXPECT_EQ(allocator.used(), 0u);
}

TEST(MemoryTest, PoolAllocatorConstructsAndDestroys) {
    eve::PoolAllocator<int> pool(4);
    int* value = pool.construct(42);
    ASSERT_NE(value, nullptr);
    EXPECT_EQ(*value, 42);
    pool.destroy(value);
    EXPECT_EQ(pool.available(), 4u);
}
