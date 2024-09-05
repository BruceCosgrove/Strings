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

    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    using const_iterator = const_pointer;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_type npos = std::numeric_limits<size_type>::max();
    static constexpr value_type nult = value_type(); // Null Terminator

    static_assert(std::is_same_v<value_type, typename traits_type::char_type>, "Traits must have same value type.");

public:
    [[nodiscard]] constexpr basic_string_view() noexcept
        : _data(nullptr), _size(0) {}

    [[nodiscard]] constexpr basic_string_view(const_pointer data)
        : _data(data), _size(traits_type::length(data)) {}

    [[nodiscard]] constexpr basic_string_view(const_pointer data, size_type size)
        : _data(data), _size(size) {}

    constexpr basic_string_view(const basic_string_view&) noexcept = default;
    constexpr basic_string_view(basic_string_view&&) noexcept = default;
    constexpr basic_string_view& operator=(const basic_string_view&) noexcept = default;
    constexpr basic_string_view& operator=(basic_string_view&&) noexcept = default;
    constexpr ~basic_string_view() noexcept = default;

    basic_string_view(std::nullptr_t) = delete;

public:
    // Returns an immutable reference to the character at the given index in the view.
    [[nodiscard]] constexpr const_reference operator[](size_type index) const noexcept { return at(index); }

    // Returns an immutable reference to the character at the given index in the view.
    [[nodiscard]] constexpr const_reference at(size_type index) const noexcept { return 0 <= index && index < _size ? _data[index] : nult; }

    // Returns an immutable reference to the first character in the view.
    [[nodiscard]] constexpr const_reference front() const noexcept { return at(0); }

    // Returns an immutable reference to the last character in the view.
    [[nodiscard]] constexpr const_reference back() const noexcept { return at(_size - 1); }

    // Returns a pointer to immutable elements.
    [[nodiscard]] constexpr const_pointer data() const noexcept { return _data; }

public:
    // Returns if the view is empty.
    [[nodiscard]] constexpr bool empty() const noexcept { return _size == 0; }

    // Returns the length of the view.
    [[nodiscard]] constexpr size_type size() const noexcept { return _size; }

    // Returns the signed length of the view.
    [[nodiscard]] constexpr ssize_type ssize() const noexcept { return static_cast<ssize_type>(_size); }

    // Returns the length of the view.
    [[nodiscard]] constexpr size_type length() const noexcept { return _size; }

    // Returns the max size of the view.
    [[nodiscard]] static constexpr size_type max_size() noexcept { return std::numeric_limits<ssize_type>::max() / sizeof(value_type); }

public:
    // Returns a new view with the first n characters removed.
    [[nodiscard]] constexpr basic_string_view remove_prefix(size_type n) const noexcept
    { return _size < n ? _retain_empty() : basic_string_view(_data + n, _size - n); }

    // Returns a new view with the last n characters removed.
    [[nodiscard]] constexpr basic_string_view remove_suffix(size_type n) const noexcept
    { return _size < n ? _retain_empty() : basic_string_view(_data, _size - n); }

    // Swaps this view with the given view.
    constexpr void swap(basic_string_view& view) noexcept
    { std::swap(*this, view); }

public:
    // Copies a substring from this view to the destination.
    constexpr size_type copy(pointer destination, size_type count, size_type position = 0) const
    {
        if (_size <= position)
            return 0;
        count = std::min(count, _size - position);
        traits_type::copy(destination, _data + position, count);
        return count;
    }

    // Returns substring of max length `count` starting from `offset`.
    [[nodiscard]] constexpr basic_string_view substr(size_type offset = 0, size_type count = npos) const noexcept
    {
        if (_size < offset)
            return _retain_empty();
        return { _data + offset, std::min(_size - offset, count) };
    }

    // Compares two views.
    [[nodiscard]] constexpr ssize_type compare(basic_string_view view) const noexcept
    {
        if (auto comparison = _traits_t::compare(data(), view.data(), std::min(size(), view.size())))
            return comparison;
        return ssize() - view.ssize();
    }

    // Compares a substring of this view with another view.
    [[nodiscard]] constexpr ssize_type compare(size_type offset1, size_type count1, basic_string_view view) const noexcept
    { return substr(offset1, count1).compare(view); }

    // Compares a substring of this view with a substring of another view.
    [[nodiscard]] constexpr ssize_type compare(size_type offset1, size_type count1, basic_string_view view, size_type offset2, size_type count2) const noexcept
    { return substr(offset1, count1).compare(view.substr(offset2, count2)); }

    // Compares this view with a substring of another view.
    [[nodiscard]] constexpr ssize_type compare(basic_string_view view, size_type offset2, size_type count2) const noexcept
    { return compare(view.substr(offset2, count2)); }

    // Returns if this view starts with the given view.
    [[nodiscard]] constexpr bool starts_with(basic_string_view view) const noexcept
    { return view._size <= _size && substr(0, view._size) == view; }

    // Returns if this view ends with the given view.
    [[nodiscard]] constexpr bool ends_with(basic_string_view view) const noexcept
    { return view._size <= _size && substr(_size - view._size, view._size) == view; }

    // TODO: contains

    // TODO: find

    // TODO: rfind

    // TODO: find_first_of

    // TODO: find_first_not_of

    // TODO: fint_last_of

    // TODO: find_last_not_of

public:
    [[nodiscard]] constexpr const_iterator begin() const noexcept { return const_iterator(_data); }
    [[nodiscard]] constexpr const_iterator end() const noexcept { return const_iterator(_data + _size); }
    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(_data + _size); }
    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(_data); }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return const_iterator(_data); }
    [[nodiscard]] constexpr const_iterator cend() const noexcept { return const_iterator(_data + _size); }
    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(_data + _size); }
    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(_data); }

private:
    // For better debugging.
    constexpr basic_string_view _retain_empty() const noexcept { return { _data, 0 }; }

private:
    const_pointer _data;
    size_type _size;
};

// Compares two views.
template <typename _char_t, typename _size_t, typename _traits_t>
constexpr bool operator==(
    basic_string_view<_char_t, _size_t, _traits_t> a,
    basic_string_view<_char_t, _size_t, _traits_t> b
) noexcept
{
    return a.size() == b.size() && _traits_t::compare(a.data(), b.data(), a.size()) == 0;
}

// Compares two views.
template <typename _char_t, typename _size_t, typename _traits_t>
constexpr std::strong_ordering operator<=>(
    basic_string_view<_char_t, _size_t, _traits_t> a,
    basic_string_view<_char_t, _size_t, _traits_t> b
) noexcept
{
    auto comparison = a.compare(b);
    return comparison < 0 ? std::strong_ordering::less :
            comparison == 0 ? std::strong_ordering::equal :
            std::strong_ordering::greater;
}

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
