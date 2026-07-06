#include <algorithm>
#include <cstring>
#include <eve/core/memory/allocator.hpp>
#include <stdexcept>

namespace eve {

namespace {

std::size_t align_up(std::size_t value, std::size_t alignment) {
    return (value + alignment - 1) & ~(alignment - 1);
}

} // namespace

LinearAllocator::LinearAllocator(std::size_t capacity)
    : capacity_(capacity) {
    buffer_ = static_cast<u8*>(std::malloc(capacity_));
    if (buffer_ == nullptr) {
        throw std::bad_alloc();
    }
}

LinearAllocator::~LinearAllocator() {
    std::free(buffer_);
}

void* LinearAllocator::allocate(std::size_t size, std::size_t alignment) {
    const std::size_t aligned_offset = align_up(offset_, alignment);
    if (aligned_offset + size > capacity_) {
        return nullptr;
    }
    void* ptr = buffer_ + aligned_offset;
    offset_ = aligned_offset + size;
    return ptr;
}

void LinearAllocator::reset() {
    offset_ = 0;
}

} // namespace eve
