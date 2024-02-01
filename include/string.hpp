#pragma once

#include "string_view.hpp"
#include <algorithm>
#include <concepts>
#include <cstdint>
#include <limits>
#include <memory>

template
<
    typename _char_t,
    typename _size_t = std::size_t,
    _size_t _min_internal_capacity = 15 / sizeof(_char_t),
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
    static constexpr value_type nult = value_type(); // Null Terminator

    static_assert(std::is_same_v<value_type, typename traits_type::char_type>, "Traits must have same value type.");
    static_assert(std::is_same_v<value_type, typename allocator_type::value_type>, "Allocator must have same value type.");

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
        if (_size <= _internal_capacity())
            _init_small();
        else
        {
            _init_large();
            _allocate_current(_size);
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

    explicit constexpr basic_string(string_view_type view)
        : _size(view.size())
    {
        if (_size <= _internal_capacity())
            _init_small();
        else
        {
            _init_large();
            _allocate_current(_size);
        }
        traits_type::copy(_elements(), view.data(), _size);
        _eos();
    }

    constexpr basic_string(const_pointer elements)
        : basic_string(string_view_type(elements))
    {

    }

    constexpr basic_string& operator=(const basic_string& string)
    {
        if (this != std::addressof(string))
        {
            _size = string._size;
            if (small())
            {
                if (!string.small())
                {
                    _become_large();
                    _allocate_current(_size);
                }
                traits_type::copy(_elements(), string._elements(), _size);
            }
            else if (string.small())
            {
                _deallocate_current();
                _become_small();
                traits_type::copy(_small_buffer, string._small_buffer, _size);
            }
            else
            {
                if (_capacity < _size)
                {
                    _deallocate_current();
                    _allocate_current(_size);
                }
                traits_type::copy(_large_buffer, string._large_buffer, _size);
            }
            _eos();
        }
        return *this;
    }

    constexpr basic_string& operator=(basic_string&& string)
    {
        if (this != std::addressof(string))
        {
            _size = string._size;
            if (!small())
                _deallocate_current();

            if (string.small())
            {
                if (!small())
                    _become_small();
                traits_type::copy(_small_buffer, string._small_buffer, _size);
                _eos();
            }
            else
            {
                _large_buffer = string._large_buffer;
                _capacity = string._capacity;
                string._become_small();
            }
            string._size = 0;
            string._eos();
        }
        return *this;
    }

    constexpr basic_string& operator=(string_view_type view)
    {
        if (_elements() != view.data())
        {
            _size = view.size();
            if (_capacity < _size)
            {
                auto allocation = _allocate(_size);
                _capacity = allocation.capacity;
                // Copy before setting _large_buffer because elements might be in _large_buffer.
                traits_type::copy(allocation.elements, view.data(), _size);
                _large_buffer = allocation.elements;
            }
            else
                traits_type::move(_elements(), view.data(), _size);
            _eos();
        }
        return *this;
    }

    constexpr basic_string& operator=(const_pointer elements)
    {
        return *this = string_view_type(elements);
    }

    basic_string(std::nullptr_t) = delete;
    basic_string& operator=(std::nullptr_t) = delete;

public:
    // Returns the length of the string.
    constexpr size_type size() const noexcept { return _size; }

    // Returns the signed length of the string.
    constexpr ssize_type ssize() const noexcept { return static_cast<ssize_type>(_size); }

    // Returns the length of the string.
    constexpr size_type length() const noexcept { return _size; }

    // Returns the capacity of the string.
    constexpr size_type capacity() const noexcept { return _capacity; }

    // Returns the max size of the string.
    constexpr size_type max_size() const noexcept
    {
        return std::min(
            allocator_traits::max_size(_allocator) - 1,
            (std::numeric_limits<ssize_type>::max()- 1) / sizeof(value_type)
        );
    }

    // Returns if the string is currently in small mode.
    constexpr bool small() const noexcept { return _capacity <= _internal_capacity(); }

    // Returns if the string is empty.
    constexpr bool empty() const noexcept { return _size == 0; }

public:
    // Returns a reference to the first character in the string.
    constexpr reference front() { return at(0); }
    // Returns a reference to the first character in the string.
    constexpr const_reference front() const { return at(0); }
    // Returns a reference to the last character in the string.
    constexpr reference back() { return at(_size - 1); }
    // Returns a reference to the last character in the string.
    constexpr const_reference back() const { return at(_size - 1); }

    // Returns a reference to the character at the given index in the string.
    constexpr const_reference at(size_type index) const
    {
        return 0 <= index && index < _size ? _elements()[index] : nult;
    }

    // Returns a reference to the character at the given index in the string.
    constexpr reference at(size_type index)
    {
        return const_cast<pointer>(const_cast<const basic_string*>(this)->at(index));
    }

    // Returns a pointer to the elements.
    constexpr pointer data() noexcept { return _elements(); }
    // Returns a pointer to the elements.
    constexpr const_pointer data() const noexcept { return _elements(); }
    // Returns a pointer to immutable elements.
    constexpr const_pointer c_str() const noexcept { return _elements(); }
    // Converts this string into a string view.
    constexpr operator string_view_type() const noexcept { return {_elements(), _size}; }

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

    constexpr void _allocate_current(size_type element_count)
    {
        auto allocation = _allocate(_size);
        _large_buffer = allocation.elements;
        _capacity = allocation.capacity;
    }

    constexpr void _deallocate_current()
    {
        deallocate({ _large_buffer, _capacity });
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
    size_type _size;
    size_type _capacity;
    [[no_unique_address]] allocator_type _allocator;
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
