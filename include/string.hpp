#pragma once

// If you wish to change the default allocator of all strings, define
// this macro to be your custom allocator before including this file.
#ifndef STRING_DEFAULT_ALLOCATOR
#define STRING_DEFAULT_ALLOCATOR std::allocator<_char_t>
#endif

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
    typename _allocator_t = STRING_DEFAULT_ALLOCATOR
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
    static_assert(std::is_nothrow_default_constructible_v<allocator_type>, "Allocator must be nothrow default constructible.");
    static_assert(std::is_empty_v<allocator_type>, "Allocator must be stateless.");

public:
    [[nodiscard]] constexpr basic_string() noexcept
        : _size(0)
    {
        _init_small();
        _eos();
    }

    [[nodiscard]] constexpr basic_string(const basic_string& string)
        : _size(string._size)
    {
        _init_from_size();
        traits_type::copy(_elements(), string._elements(), _size);
    }

    [[nodiscard]] constexpr basic_string(basic_string&& string) noexcept
        : _size(string._size)
    {
        if (_size <= _internal_capacity())
        {
            _init_small();
            _eos();
            traits_type::copy(_small_buffer, string._small_buffer, _size);
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

    [[nodiscard]] constexpr basic_string(size_type count, value_type element) noexcept
        : _size(count)
    {
        _init_from_size();
        traits_type::assign(_elements(), count, element);
    }

    [[nodiscard]] explicit constexpr basic_string(string_view_type view)
        : _size(view.size())
    {
        _init_from_size();
        traits_type::copy(_elements(), view.data(), _size);
    }

    [[nodiscard]] constexpr basic_string(const_pointer elements)
        : basic_string(string_view_type(elements))
    {

    }

    basic_string(std::nullptr_t) = delete;

    constexpr basic_string& operator=(const basic_string& string)
    {
        if (this != std::addressof(string))
        {
            _size = string._size;
            if (_capacity < _size)
            {
                _become_large_or_deallocate();
                _allocate_current();
            }
            _eos();
            traits_type::copy(_elements(), string._elements(), _size);
        }
        return *this;
    }

    constexpr basic_string& operator=(basic_string&& string) noexcept
    {
        if (this != std::addressof(string))
        {
            _size = string._size;
            if (_capacity < _size)
            {
                _become_large_or_deallocate();
                _large_buffer = string._large_buffer;
                _capacity = string._capacity;
                string._become_small();
            }
            else // does not imply anything about string.small()
            {
                _eos();
                traits_type::copy(_elements(), string._elements(), _size);
                if (!string.small())
                {
                    string._deallocate_current();
                    string._become_small();
                }
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
                _become_large_or_deallocate();
                _allocate_current();
                traits_type::copy(_large_buffer, view.data(), _size);
            }
            else
                traits_type::move(_elements(), view.data(), _size);
            _eos();
        }
        else if (_size > view.size())
        {
            // view is substr(0, view.size()), so just set the size and null terminator.
            _size = view.size();
            _eos();
        }
        return *this;
    }

    constexpr basic_string& operator=(const_pointer elements) { return *this = string_view_type(elements); }

    constexpr ~basic_string() noexcept
    {
        if (!small())
            _deallocate_current();
    }

    basic_string& operator=(std::nullptr_t) = delete;

    constexpr basic_string& assign(size_type count, value_type element)
    {
        _size = count;
        if (_capacity < _size)
        {
            _become_large_or_deallocate();
            _allocate_current();
        }
        traits_type::assign(_elements(), count, element);
        _eos();
        return *this;
    }

public:
    // Returns a reference to the character at the given index in the string.
    [[nodiscard]] constexpr reference operator[](size_type index) { return at(index);}
    // Returns an immutable reference to the character at the given index in the string.
    [[nodiscard]] constexpr const_reference operator[](size_type index) const { return at(index);}

    // Returns an immutable reference to the character at the given index in the string.
    [[nodiscard]] constexpr const_reference at(size_type index) const
    { return 0 <= index && index < _size ? _elements()[index] : nult; }
    // Returns a reference to the character at the given index in the string.
    [[nodiscard]] constexpr reference at(size_type index)
    { return const_cast<pointer>(const_cast<const basic_string*>(this)->at(index)); }

    // Returns a reference to the first character in the string.
    [[nodiscard]] constexpr reference front() { return at(0); }
    // Returns an immutable reference to the first character in the string.
    [[nodiscard]] constexpr const_reference front() const { return at(0); }

    // Returns a reference to the last character in the string.
    [[nodiscard]] constexpr reference back() { return at(_size - 1); }
    // Returns an immutable reference to the last character in the string.
    [[nodiscard]] constexpr const_reference back() const { return at(_size - 1); }

    // Returns a pointer to the elements.
    [[nodiscard]] constexpr pointer data() noexcept { return _elements(); }
    // Returns a pointer to immutable elements.
    [[nodiscard]] constexpr const_pointer data() const noexcept { return _elements(); }

    // Returns a pointer to immutable elements.
    [[nodiscard]] constexpr const_pointer c_str() const noexcept { return _elements(); }

    // Converts this string into a string view.
    [[nodiscard]] constexpr operator string_view_type() const noexcept { return {_elements(), _size}; }

public:
    // Returns if the string is currently in small mode.
    [[nodiscard]] constexpr bool small() const noexcept { return _capacity <= _internal_capacity(); }

    // Returns if the string is empty.
    [[nodiscard]] constexpr bool empty() const noexcept { return _size == 0; }

    // Returns the length of the string.
    [[nodiscard]] constexpr size_type size() const noexcept { return _size; }

    // Returns the signed length of the string.
    [[nodiscard]] constexpr ssize_type ssize() const noexcept { return static_cast<ssize_type>(_size); }

    // Returns the length of the string.
    [[nodiscard]] constexpr size_type length() const noexcept { return _size; }

    // Returns the max size of the string.
    [[nodiscard]] constexpr size_type max_size() const noexcept
    {
        return std::min(
            allocator_traits::max_size(_allocator) - 1,
            (std::numeric_limits<ssize_type>::max() - 1) / sizeof(value_type)
        );
    }

    // Ensures the allocated capacity is at least `new_capacity`.
    constexpr void reserve(size_type new_capacity)
    {
        if (_capacity < new_capacity)
        {
            auto allocation = _allocate(new_capacity);
            traits_type::copy(allocation.elements, _elements(), _size);

            if (small())
                _become_large();
            else
                _deallocate_current();

            _large_buffer = allocation.elements;
            _capacity = allocation.capacity;
            _eos();
        }
    }

    // Returns the capacity of the string.
    [[nodiscard]] constexpr size_type capacity() const noexcept { return _capacity; }

    // Sets capacity equal to size, if possible.
    // Capacity will never be less than `sboc`.
    constexpr void shrink_to_fit()
    {
        if (!small() && _size != _capacity)
        {
            if (_size <= _internal_capacity())
            {
                auto old_allocation = _current_allocation();
                _become_small();
                traits_type::copy(_small_buffer, old_allocation.elements, _size);
                _deallocate(old_allocation);
            }
            else
            {
                auto allocation = _allocate(_size);
                traits_type::copy(allocation.elements, _large_buffer, _size);
                _deallocate_current();
                _large_buffer = allocation.elements;
                _capacity = allocation.capacity;
            }
            _eos();
        }
    }

public:
    // TODO: clear

    // TODO: insert

    // TODO: erase

    // TODO: push_back

    // TODO: pop_back

    // TODO: append

    // TODO: operator+=

    // TODO: replace

    // TODO: resize

    // Swaps this string with the given string.
    constexpr void swap(basic_string& string) noexcept
    {
        if (small())
        {
            if (string.small())
            {
                value_type temp[_internal_capacity()];
                traits_type::copy(temp, _small_buffer, _size);
                traits_type::copy(_small_buffer, string._small_buffer, string._size);
                traits_type::copy(string._small_buffer, temp, _size);
                std::swap(_size, string._size);
                _eos();
                string._eos();
            }
            else
            {
                auto other_allocation = string._current_allocation();
                string._become_small();
                traits_type::copy(string._small_buffer, _small_buffer, _size);
                _become_large();
                _large_buffer = other_allocation.elements;
                _capacity = other_allocation.capacity;
                std::swap(_size, string._size);
                string._eos();
            }
        }
        else if (string.small())
            return string.swap(*this); // Use the above case, but reverse the swapping order.
        else
        {
            std::swap(_large_buffer, string._large_buffer);
            std::swap(_size, string._size);
            std::swap(_capacity, string._capacity);
        }
    }

public:
    // TODO: copy

    // TODO: substr

    // TODO: compare

    // TODO: starts_with

    // TODO: ends_with

    // TODO: contains

    // TODO: find

    // TODO: rfind

    // TODO: find_first_of

    // TODO: find_first_not_of

    // TODO: fint_last_of

    // TODO: find_last_not_of

public:
    [[nodiscard]] constexpr iterator begin() noexcept { return iterator(_elements()); }
    [[nodiscard]] constexpr iterator end() noexcept { return iterator(_elements() + _size); }
    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(_elements() + _size); }
    [[nodiscard]] constexpr reverse_iterator rend() noexcept { return reverse_iterator(_elements()); }

    [[nodiscard]] constexpr const_iterator begin() const noexcept { return const_iterator(_elements()); }
    [[nodiscard]] constexpr const_iterator end() const noexcept { return const_iterator(_elements() + _size); }
    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(_elements() + _size); }
    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(_elements()); }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return const_iterator(_elements()); }
    [[nodiscard]] constexpr const_iterator cend() const noexcept { return const_iterator(_elements() + _size); }
    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(_elements() + _size); }
    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(_elements()); }

private:
    // Initializes the string to be small or large, depending on the size.
    constexpr void _init_from_size()
    {
        if (_size <= _internal_capacity())
            _init_small();
        else
        {
            _init_large();
            _allocate_current();
        }
        _eos();
    }

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

    // Puts the string in large mode, or if already in
    // large mode, deallocates the current allocation.
    constexpr void _become_large_or_deallocate() noexcept
    {
        if (small())
            _become_large();
        else
            _deallocate_current();
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
    struct _allocation
    {
        pointer elements;
        size_type capacity;
    };

    constexpr _allocation _allocate(size_type element_count)
    {
        // g++ still doesn't have allocator size feedback.
        return { allocator_traits::allocate(_allocator, element_count + 1), element_count };
    }

    constexpr void _deallocate(_allocation allocation) noexcept
    {
        allocator_traits::deallocate(_allocator, allocation.elements, allocation.capacity + 1);
    }

    constexpr void _allocate_current()
    {
        auto allocation = _allocate(_size);
        _large_buffer = allocation.elements;
        _capacity = allocation.capacity;
    }

    constexpr _allocation _current_allocation() { return { _large_buffer, _capacity }; }

    constexpr void _deallocate_current() noexcept { _deallocate(_current_allocation()); }

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
    // Remove trailing padding for the union so _size and _capacity are placed in it.
    #pragma pack(push, 1)
    union
    {
        value_type _small_buffer[_internal_capacity() + 1]; // + 1 for null terminator.
        pointer _large_buffer;
    };
    #pragma pack(pop)
    size_type _size;
    size_type _capacity;
    [[no_unique_address]] allocator_type _allocator;
    // The previous pragma push, for some reason, applies to the whole class, this overrides that.
} __attribute__((aligned(alignof(pointer))));

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
