#include "string.hpp"
#include "string_ostream.hpp"
#include <iostream>
#include <iomanip>

#define DEBUG_DATA(x, spacing, ...) #x ": " spacing __VA_ARGS__ << (x)

#define DEBUG_STRING(string_type, spacing) \
    std::cout << DEBUG_DATA(+string_type::sboc, spacing, << std::setw(2)) << ", " \
              << DEBUG_DATA(sizeof(string_type), spacing, << std::setw(2)) << ", " \
              << DEBUG_DATA(+small_##string_type::sboc, spacing, << std::setw(2)) << ", " \
              << DEBUG_DATA(sizeof(small_##string_type), spacing, << std::setw(2)) << std::endl

constexpr string_view strview = "hello world...\n";
// Must be local static/global for conversion to string view to work in constexpr. Local non-static doesn't work.
constexpr string str(strview);
constexpr string_view sv = str;

int main()
{
    std::cout << strview << str << sv;

    DEBUG_STRING(string, "   ");
    DEBUG_STRING(u8string, " ");
    DEBUG_STRING(u16string, "");
    DEBUG_STRING(u32string, "");
    DEBUG_STRING(wstring, "  ");

    return 0;
}
