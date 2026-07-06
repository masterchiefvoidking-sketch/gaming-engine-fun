#pragma once

#include <atomic>
#include <condition_variable>
#include <eve/core/types.hpp>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace eve {

class JobSystem {
public:
    explicit JobSystem(u32 worker_count = std::thread::hardware_concurrency());
    ~JobSystem();

    JobSystem(const JobSystem&) = delete;
    JobSystem& operator=(const JobSystem&) = delete;

    template <typename Fn> auto submit(Fn&& fn) -> std::future<std::invoke_result_t<Fn>>;

    void wait_idle();

private:
    void worker_loop();

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> jobs_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> running_{true};
    std::atomic<u32> active_jobs_{0};
};

template <typename Fn> auto JobSystem::submit(Fn&& fn) -> std::future<std::invoke_result_t<Fn>> {
    using Result = std::invoke_result_t<Fn>;
    auto task = std::make_shared<std::packaged_task<Result()>>(std::forward<Fn>(fn));
    std::future<Result> future = task->get_future();

    {
        std::lock_guard lock(mutex_);
        jobs_.emplace([task]() { (*task)(); });
    }
    cv_.notify_one();
    return future;
}

class SpinLock {
public:
    void lock() {
        while (flag_.test_and_set(std::memory_order_acquire)) {
        }
    }

    void unlock() { flag_.clear(std::memory_order_release); }

private:
    std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
};

} // namespace eve
