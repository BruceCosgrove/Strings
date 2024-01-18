#include "string.hpp"
#include <iostream>

#define DEBUG_DATA(x, ...) #x ": " __VA_ARGS__ << (x)

#define DEBUG_STRING(string_type, ...) \
    std::cout << DEBUG_DATA(+string_type::sboc __VA_OPT__(,) __VA_ARGS__) << ", " \
              << DEBUG_DATA(sizeof(string_type) __VA_OPT__(,) __VA_ARGS__) << ", " \
              << DEBUG_DATA(+small_##string_type::sboc __VA_OPT__(,) __VA_ARGS__) << ", " \
              << DEBUG_DATA(sizeof(small_##string_type) __VA_OPT__(,) __VA_ARGS__) << std::endl

int main()
{
    constexpr string_view strview = "hello world...\n";
    constexpr string str = string(strview);
    std::cout << str.c_str();

    DEBUG_STRING(string, "   ");
    DEBUG_STRING(u8string, " ");
    DEBUG_STRING(u16string);
    DEBUG_STRING(u32string);
    DEBUG_STRING(wstring, "  ");

    return 0;
}
