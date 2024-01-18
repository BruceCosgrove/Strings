#pragma once

#include <concepts>
#include <cstdint>
#include <memory>

template
<
    typename _char_t,
    typename _size_t = std::size_t,
    typename _traits_t = std::char_traits<_char_t>
>
class basic_string_view
{
public:
    using value_type = _char_t;
    using size_type = _size_t;
    using traits_type = _traits_t;

    using ssize_type = std::make_signed_t<size_type>;

    using const_reference = const value_type&;
    using const_pointer = const value_type*;

    using const_iterator = const_pointer;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_type npos = std::numeric_limits<size_type>::max();
    static constexpr value_type nult = value_type();

    static_assert(std::is_same_v<value_type, typename traits_type::char_type>, "Traits must have same value type.");

public:
    constexpr basic_string_view() noexcept
        : _data(nullptr), _size(0) {}

    constexpr basic_string_view(const_pointer data)
        : _data(data), _size(traits_type::length(data)) {}

    constexpr basic_string_view(const_pointer data, size_type size)
        : _data(data), _size(size) {}

    constexpr basic_string_view(const basic_string_view&) noexcept = default;
    constexpr basic_string_view(basic_string_view&&) noexcept = default;
    constexpr basic_string_view& operator=(const basic_string_view&) noexcept = default;
    constexpr basic_string_view& operator=(basic_string_view&&) noexcept = default;
    constexpr ~basic_string_view() noexcept = default;

    basic_string_view(std::nullptr_t) = delete;

public:
    constexpr const_reference front() const noexcept { return empty() ? nult : _data[0]; }
    constexpr const_reference back() const noexcept { return empty() ? nult : _data[_size - 1]; }
    constexpr const_reference at(size_type index) const noexcept { return index < 0 || _size < index ? nult : _data[index]; }
    constexpr const_reference operator[](size_type index) const noexcept { return at(index); }
    constexpr const_pointer data() const noexcept { return _data; }

public:
    constexpr size_type size() const noexcept { return _size; }
    constexpr ssize_type ssize() const noexcept { return static_cast<ssize_type>(_size); }
    constexpr size_type length() const noexcept { return _size; }
    static constexpr size_type max_size() noexcept { return std::numeric_limits<ssize_type>::max() / sizeof(value_type); }
    constexpr bool empty() const noexcept { return _size == 0; }

public:
    constexpr basic_string_view remove_prefix(size_type n) const noexcept
    { return _size < n ? _retain_empty() : basic_string_view(_data + n, _size - n); }

    constexpr basic_string_view remove_suffix(size_type n) const noexcept
    { return _size < n ? _retain_empty() : basic_string_view(_data, _size - n); }

    constexpr void swap(basic_string_view& view) noexcept
    { std::swap(*this, view); }

public:
    constexpr basic_string_view substr(size_type offset = 0, size_type count = npos) const noexcept
    {
        if (_size < offset)
            return _retain_empty();
        return { _data + offset, std::min(_size - offset, count) };
    }

    constexpr bool starts_with(basic_string_view view) const noexcept
    { return view._size <= _size && substr(0, view._size) == view; }

    constexpr bool ends_with(basic_string_view view) const noexcept
    { return view._size <= _size && substr(_size - view._size, view._size) == view; }

    constexpr bool operator==(basic_string_view view) const noexcept
    { return view._size == _size && traits_type::compare(_data, view._data) == 0; }

    constexpr std::strong_ordering operator<=>(basic_string_view view) const noexcept
    {
        if (auto comparison = traits_type::compare(_data, view._data, std::min(_size, view._size)))
            return comparison;
        return ssize() - view.ssize();
    }

public:
    constexpr const_iterator begin() const noexcept { return _data; }
    constexpr const_iterator end() const noexcept { return _data + _size; }
    constexpr const_reverse_iterator rbegin() const noexcept { return _data + _size - 1; }
    constexpr const_reverse_iterator rend() const noexcept { return _data - 1; }

    constexpr const_iterator cbegin() const noexcept { return _data; }
    constexpr const_iterator cend() const noexcept { return _data + _size; }
    constexpr const_reverse_iterator crbegin() const noexcept { return _data + _size - 1; }
    constexpr const_reverse_iterator crend() const noexcept { return _data - 1; }

private:
    // For better debugging.
    constexpr basic_string_view _retain_empty() const noexcept { return { _data, 0 }; }

private:
    const_pointer _data;
    size_type _size;
};

using string_view = basic_string_view<char, std::size_t>;
using small_string_view = basic_string_view<char, std::uint8_t>;
using u8string_view = basic_string_view<char8_t, std::size_t>;
using small_u8string_view = basic_string_view<char8_t, std::uint8_t>;
using u16string_view = basic_string_view<char16_t, std::size_t>;
using small_u16string_view = basic_string_view<char16_t, std::uint8_t>;
using u32string_view = basic_string_view<char32_t, std::size_t>;
using small_u32string_view = basic_string_view<char32_t, std::uint8_t>;
using wstring_view = basic_string_view<wchar_t, std::size_t>;
using small_wstring_view = basic_string_view<wchar_t, std::uint8_t>;
