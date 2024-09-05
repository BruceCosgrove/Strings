# Strings

This is my attempt at implementing strings, which includes the following:
1. Small String Buffer Optimization, aka SSBO.
2. Customizable minimum SSBO capacity.
3. Customizable size type.
4. Aware of char_traits and allocator.
5. Very similar to `std::string` and `std::string_view`.
   - Most methods from `std::string` and `std::string_view` are present.
   - The allocator is required to be stateless.
   - Includes an `ssize` member function.
   - No explicit exceptions. Valid values are returned in error cases, e.g. `basic_string::nult`.
   - Every public member function that accesses elements is bounds checked to maintain memory safety.
   - `basic_string` can be constexpr as long as it's final value fits in its small buffer.
   - Includes small string variants of `string`, `wstring`, `u8string`, etc. that use `std::uint8_t` as their size type instead of `std::size_t`.
     As per the [Implementation](#implementation) section, this increases their SSBO capacity.

## Implementation
The main star of the show is `basic_string::_internal_capacity()`, which calculates the actual capacity of the SSBO.
It will always be at least the provided minimum SSBO capacity, but will often be a little more.
Depending on the provided size and value types, it will extend the buffer, pushing the rest of the member variables into where there would otherwise be unused padding bytes.
As a result, this implementation can hold up to six (6) more elements while being at most the same size as `std::string`.
