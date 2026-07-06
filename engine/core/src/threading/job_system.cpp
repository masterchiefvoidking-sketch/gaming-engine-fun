#include <eve/core/threading/job_system.hpp>

namespace eve {

JobSystem::JobSystem(u32 worker_count) {
    if (worker_count == 0) {
        worker_count = 1;
    }
    workers_.reserve(worker_count);
    for (u32 i = 0; i < worker_count; ++i) {
        workers_.emplace_back([this]() { worker_loop(); });
    }
}

JobSystem::~JobSystem() {
    {
        std::lock_guard lock(mutex_);
        running_ = false;
    }
    cv_.notify_all();
    for (auto& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void JobSystem::wait_idle() {
    while (active_jobs_.load() > 0) {
        std::this_thread::yield();
    }
    std::lock_guard lock(mutex_);
    while (!jobs_.empty()) {
        std::this_thread::yield();
    }
}

void JobSystem::worker_loop() {
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock lock(mutex_);
            cv_.wait(lock, [this]() { return !running_ || !jobs_.empty(); });
            if (!running_ && jobs_.empty()) {
                return;
            }
            job = std::move(jobs_.front());
            jobs_.pop();
            active_jobs_.fetch_add(1);
        }

        job();
        active_jobs_.fetch_sub(1);
    }
}

} // namespace eve
