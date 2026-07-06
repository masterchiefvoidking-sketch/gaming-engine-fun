#include <atomic>
#include <eve/core/threading/job_system.hpp>
#include <gtest/gtest.h>

TEST(ThreadingTest, ExecutesSubmittedJobs) {
    eve::JobSystem jobs(2);
    std::atomic<int> counter{0};

    auto future_a = jobs.submit([&counter]() { counter.fetch_add(1); });
    auto future_b = jobs.submit([&counter]() { counter.fetch_add(10); });

    future_a.get();
    future_b.get();
    jobs.wait_idle();

    EXPECT_EQ(counter.load(), 11);
}
