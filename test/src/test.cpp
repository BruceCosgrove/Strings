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

    string a;
    string b = str;
    string c = std::move(b);
    string d(strview);
    string e = "Goodbye.";
    string f = "Alone again... naturallyyyyyyy..";
    // string z = nullptr;
    a = b;
    c = d;
    e = std::move(c);
    a = strview;
    b = "Sideways-|";
    c = "supercalifragilisticexpialadocious";
    string g = c;
    string h = std::move(c);
    string i = string(string_view(g));
    i = a;
    h = g;
    g = std::move(h);
    g = std::move(i);
    // a = nullptr;

    return 0;
}
