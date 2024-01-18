#pragma once

#include "string_view.hpp"
#include <algorithm>
#include <cstdint>
#include <memory>
#include <concepts>

template
<
    typename _char_t,
    typename _size_t,
    _size_t _min_internal_capacity = 15,
    typename _traits_t = std::char_traits<_char_t>,
    typename _allocator_t = std::allocator<_char_t>
>
class basic_string
{
public:
    using value_type = _char_t;
    using size_type = _size_t;
    using traits_type = _traits_t;
    using allocator_type = _allocator_t;

    using ssize_type = std::make_signed_t<size_type>;
    using allocator_traits = std::allocator_traits<allocator_type>;
    using string_view_type = basic_string_view<value_type, size_type, traits_type>;

    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_type npos = std::numeric_limits<size_type>::max();
    static constexpr value_type nult = value_type();

    static_assert(std::is_same_v<value_type, typename traits_type::char_type>, "Traits must have same value type.");
    static_assert(std::is_same_v<value_type, typename allocator_type::value_type>, "Allocator must have same value type.");
    static_assert(std::is_empty_v<allocator_type>, "Allocator must be stateless.");

public:
    constexpr basic_string() noexcept
        : _size(0)
    {
        _init_small();
        _eos();
    }

    constexpr basic_string(const basic_string& string)
        : _size(string._size)
    {
        if (string.small())
            _init_small();
        else
        {
            _init_large();
            auto allocation = _allocate(_size);
            _large_buffer = allocation.elements;
            _capacity = allocation.capacity;
        }
        traits_type::copy(_elements(), string._elements(), _size);
        _eos();
    }

    constexpr basic_string(basic_string&& string) noexcept
        : _size(string._size)
    {
        if (_size <= _internal_capacity())
        {
            _init_small();
            traits_type::copy(_small_buffer, string._small_buffer, _size);
            _eos();
        }
        else
        {
            _init_large();
            _large_buffer = string._large_buffer;
            _capacity = string._capacity;
            string._become_small();
        }
        string._size = 0;
        string._eos();
    }

    constexpr basic_string(const_pointer elements)
        : _size(traits_type::length(elements))
    {
        if (_size <= _internal_capacity())
            _init_small();
        else
        {
            _init_large();
            auto allocation = _allocate(_size);
            _large_buffer = allocation.elements;
            _capacity = allocation.capacity;
        }
        traits_type::copy(_elements(), elements, _size);
        _eos();
    }

    explicit constexpr basic_string(string_view_type view)
        : _size(view.size())
    {
        if (_size <= _internal_capacity())
            _init_small();
        else
        {
            _init_large();
            auto allocation = _allocate(_size);
            _large_buffer = allocation.elements;
            _capacity = allocation.capacity;
        }
        traits_type::copy(_elements(), view.data(), _size);
        _eos();
    }

public:
    constexpr size_type size() const noexcept { return _size; }
    constexpr ssize_type ssize() const noexcept { return static_cast<ssize_type>(_size); }
    constexpr size_type capacity() const noexcept { return _capacity; }
    constexpr bool small() const noexcept { return _capacity <= _internal_capacity(); }
    constexpr bool empty() const noexcept { return _size == 0; }

public:
    constexpr reference front() { return at(0); }
    constexpr const_reference front() const { return at(0); }
    constexpr reference back() { return at(_size - 1); }
    constexpr const_reference back() const { return at(_size - 1); }

    constexpr const_reference at(size_type index) const
    { return 0 <= index && index < _size ? _elements()[index] : nult; }

    // g++ still doesn't have "deducing this".
    constexpr reference at(size_type index)
    {
        return const_cast<pointer>(const_cast<const basic_string*>(this)->at(index));
    }

    constexpr pointer data() noexcept { return _elements(); }
    // g++ still doesn't have "deducing this".
    constexpr const_pointer data() const noexcept { return _elements(); }
    constexpr const_pointer c_str() const noexcept { return _elements(); }

private:
    // Starts the string in large mode. Assumes initially not in either mode.
    constexpr void _init_large() noexcept
    {
        if consteval
        {
            // Start lifetime of large buffer elements.
            std::construct_at(std::addressof(_large_buffer));
        }
    }

    // Starts the string in small mode. Assumes initially not in either mode.
    constexpr void _init_small() noexcept
    {
        _capacity = _internal_capacity();
        if consteval
        {
            // Start lifetime of small buffer elements.
            for (size_type i = 0; i <= _internal_capacity(); ++i)
                std::construct_at(std::addressof(_small_buffer[i]));
        }
    }

    // Puts the string in large mode. Assumes initially in small mode.
    constexpr void _become_large() noexcept
    {
        if consteval
        {
            // End lifetime of small buffer.
            std::destroy_at(std::addressof(_small_buffer));
            // Start lifetime of large buffer.
            std::construct_at(std::addressof(_large_buffer));
        }
    }

    // Puts the string in small mode. Assumes initially in large mode.
    constexpr void _become_small() noexcept
    {
        _capacity = _internal_capacity();
        if consteval
        {
            // End lifetime of large buffer.
            std::destroy_at(std::addressof(_large_buffer));
            // Start lifetime of small buffer.
            std::construct_at(std::addressof(_small_buffer));
        }
    }

    // Sets the element at _size to the null terminator.
    constexpr void _eos() noexcept
    {
        traits_type::assign(_elements()[_size], nult);
    }

    // Gets the appropriate elements.
    constexpr pointer _elements() noexcept
    {
        return small() ? _small_buffer : _large_buffer;
    }

    // g++ still doesn't have "deducing this".
    constexpr const_pointer _elements() const noexcept
    {
        return small() ? _small_buffer : _large_buffer;
    }

private:
    struct _allocation_result
    {
        pointer elements;
        size_type capacity;
    };

    constexpr _allocation_result _allocate(size_type element_count)
    {
        // g++ still doesn't have allocator size feedback.
        return { allocator_traits::allocate(_allocator, element_count), element_count };
    }

    constexpr void deallocate(_allocation_result allocation) noexcept
    {
        allocator_traits::deallocate(_allocator, allocation.elements, allocation.capacity);
    }

private:
    // Calculate the number of elements the small buffer can hold.
    static consteval size_type _internal_capacity() noexcept
    {
        static_assert(alignof(value_type) <= alignof(pointer));

        // There's a guaranteed alignof(pointer) bytes available.
        size_type internal_capacity = alignof(pointer) / sizeof(value_type) - 1; // - 1 for null terminator.
        // Expand small buffer if there would be usable padding after
        // _size and _capacity. This effectively moves it before them,
        // appending it to the small buffer so it can be used.
        if (2 * sizeof(size_type) < alignof(pointer))
            internal_capacity += (alignof(pointer) - 2 * sizeof(size_type)) / sizeof(value_type);

        // Grow the small buffer until it can store the required number of elements.
        while (internal_capacity < _min_internal_capacity)
            internal_capacity += alignof(pointer) / sizeof(value_type);
        return internal_capacity;
    }

public: // Must be after _internal_capacity is defined. Thanks C++.
    // Small Buffer Optimization Capacity.
    static constexpr size_type sboc = _internal_capacity();

private:
    union
    {
        value_type _small_buffer[_internal_capacity() + 1]; // + 1 for null terminator.
        pointer _large_buffer;
    };
    [[no_unique_address]] allocator_type _allocator;
    size_type _size;
    size_type _capacity;
};

using string = basic_string<char, std::size_t>;
using small_string = basic_string<char, std::uint8_t>;
using u8string = basic_string<char8_t, std::size_t>;
using small_u8string = basic_string<char8_t, std::uint8_t>;
using u16string = basic_string<char16_t, std::size_t>;
using small_u16string = basic_string<char16_t, std::uint8_t>;
using u32string = basic_string<char32_t, std::size_t>;
using small_u32string = basic_string<char32_t, std::uint8_t>;
using wstring = basic_string<wchar_t, std::size_t>;
using small_wstring = basic_string<wchar_t, std::uint8_t>;
