#pragma once

#include <cstdlib>
#include <eve/core/types.hpp>
#include <memory>
#include <new>
#include <utility>
#include <vector>

namespace eve {

class LinearAllocator {
public:
    explicit LinearAllocator(std::size_t capacity);
    ~LinearAllocator();

    LinearAllocator(const LinearAllocator&) = delete;
    LinearAllocator& operator=(const LinearAllocator&) = delete;

    void* allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t));
    void reset();
    [[nodiscard]] std::size_t used() const { return offset_; }
    [[nodiscard]] std::size_t capacity() const { return capacity_; }

private:
    u8* buffer_ = nullptr;
    std::size_t capacity_ = 0;
    std::size_t offset_ = 0;
};

template <typename T> class PoolAllocator {
public:
    explicit PoolAllocator(std::size_t capacity)
        : capacity_(capacity) {
        storage_ =
            static_cast<T*>(::operator new(sizeof(T) * capacity_, std::align_val_t{alignof(T)}));
        for (std::size_t i = 0; i < capacity_; ++i) {
            free_list_.push_back(capacity_ - 1 - i);
        }
    }

    ~PoolAllocator() { ::operator delete(storage_, std::align_val_t{alignof(T)}); }

    PoolAllocator(const PoolAllocator&) = delete;
    PoolAllocator& operator=(const PoolAllocator&) = delete;

    template <typename... Args> T* construct(Args&&... args) {
        if (free_list_.empty()) {
            return nullptr;
        }
        const std::size_t index = free_list_.back();
        free_list_.pop_back();
        T* object = new (storage_ + index) T(std::forward<Args>(args)...);
        used_indices_.push_back(index);
        return object;
    }

    void destroy(T* object) {
        if (object == nullptr) {
            return;
        }
        const std::size_t index = static_cast<std::size_t>(object - storage_);
        object->~T();
        free_list_.push_back(index);
    }

    [[nodiscard]] std::size_t available() const { return free_list_.size(); }

private:
    T* storage_ = nullptr;
    std::size_t capacity_ = 0;
    std::vector<std::size_t> free_list_;
    std::vector<std::size_t> used_indices_;
};

} // namespace eve
