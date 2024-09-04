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

constexpr string_view strview = "hello world...";
// Must be local static/global for conversion to string view to work in constexpr. Local non-static doesn't work.
constexpr string str(strview);
constexpr string_view sv = str;

int main()
{
    std::cout << "\nprinting\n";
    std::cout << strview << '\n' << str << '\n' << sv << '\n';

    std::cout << "\ncapacity\n";
    DEBUG_STRING(string, "   ");
    DEBUG_STRING(u8string, " ");
    DEBUG_STRING(u16string, "");
    DEBUG_STRING(u32string, "");
    DEBUG_STRING(wstring, "  ");

    // std::cout << "\ncontructors and operator=\n";
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

    std::cout << "\niterators\n";
    for (auto c : g)
        std::cout << c;
    std::cout << '\n';
    for (auto it = g.crbegin(); it != g.crend(); ++it)
        std::cout << *it;
    std::cout << '\n';

    std::cout << "\nequality\n";
    auto bsv = static_cast<string_view>(b);
    auto gsv = static_cast<string_view>(g);

    std::cout << '"' << bsv << "\" " << (bsv == gsv ? "==" : "!=") << " \"" << gsv << "\"\n";
    std::cout << '"' << bsv << "\" " << (bsv == bsv ? "==" : "!=") << " \"" << bsv << "\"\n";

    std::cout << "\ncopy\n";
    char buffer[12] = "where world";
    std::cout << buffer << '\n';
    auto count = strview.copy(buffer, 4, 12);
    std::cout << buffer << '\n';
    std::cout << count << " chars copied\n";

    std::cout << "\ncompare\n";
    std::cout << bsv.compare(gsv) << ' ';
    std::cout << bsv.compare("") << '\n';

    std::cout << "\nreserve and shrink_to_fit\n";
    string s_rstf;
    std::cout << s_rstf.capacity() << ' ';
    s_rstf.reserve(1);
    std::cout << s_rstf.capacity() << ' ';
    s_rstf.reserve(42);
    std::cout << s_rstf.capacity() << ' ';
    s_rstf.shrink_to_fit();
    std::cout << s_rstf.capacity() << ' ';
    s_rstf = "oblong egg time, wow";
    std::cout << s_rstf.capacity() << ' ';
    s_rstf.reserve(42);
    std::cout << s_rstf.capacity() << ' ';
    s_rstf.shrink_to_fit();
    std::cout << s_rstf.capacity() << '\n';

    return 0;
}
