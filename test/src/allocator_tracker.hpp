#pragma once

#include <memory>
#include <gtest/gtest.h>

template <typename _char_t>
class basic_allocation_tracker : public std::allocator<_char_t>
{
private:
    using pointer = _char_t*;
    using base = std::allocator<_char_t>;

public:
    [[nodiscard]] pointer allocate(std::size_t size)
    {
        ++_allocationCount;
        return base::allocate(size);
    }

    void deallocate(pointer block, std::size_t size)
    {
        ++_deallocationCount;
        base::deallocate(block, size);
    }

    [[nodiscard]] static bool query_and_reset() {
        bool equal = _allocationCount == _deallocationCount;
        _allocationCount = 0;
        _deallocationCount = 0;
        return equal;
    }

private:
    static inline std::size_t _allocationCount = 0;
    static inline std::size_t _deallocationCount = 0;
};

using allocation_tracker = basic_allocation_tracker<char>;
