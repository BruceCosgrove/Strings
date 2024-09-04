#pragma once

#include "string.hpp"
#include <ostream>
#include <string_view>

template <typename _char_t, typename _size_t, _size_t _min_internal_capacity, typename _traits_t, typename _allocator_t>
std::basic_ostream<_char_t, _traits_t>& operator<<(
    std::basic_ostream<_char_t, _traits_t>& ostream,
    const basic_string<_char_t, _size_t, _min_internal_capacity, _traits_t, _allocator_t>& str
)
{
    return ostream << std::basic_string_view<_char_t, _traits_t>(str.data(), str.size());
}

template <typename _char_t, typename _size_t, typename _traits_t>
std::basic_ostream<_char_t, _traits_t>& operator<<(
    std::basic_ostream<_char_t, _traits_t>& ostream,
    basic_string_view<_char_t, _size_t, _traits_t> view
)
{
    return ostream << std::basic_string_view<_char_t, _traits_t>(view.data(), view.size());
}
