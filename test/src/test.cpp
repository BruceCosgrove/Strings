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

    std::cout << "\ncontructors and operator=\n";
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
    string k(10, 'k');
    std::cout << k << '\n';
    string l(20, 'l');
    std::cout << l << '\n';

    std::cout << "\niterators\n";
    int i2 = 0;
    for (auto c : g)
        { std::cout << c; ++i2; }
    std::cout << ' ' << i2 << '\n';
    int i3 = 0;
    for (auto it = g.crbegin(); it != g.crend(); ++it)
        { std::cout << *it; ++i3; }
    std::cout << ' ' << i3 << '\n';

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

    std::cout << "\nswap\n";
    #define CHECK_SWAP(a, b) \
        std::cout << '"' << a << "\" at " << (void*)a.data() << " and \"" << b << "\" at " << (void*)b.data() << " to\n"; \
        a.swap(b); \
        std::cout << '"' << a << "\" at " << (void*)a.data() << " and \"" << b << "\" at " << (void*)b.data() << "\n\n"
    CHECK_SWAP(s_rstf, s_rstf);
    CHECK_SWAP(s_rstf, b);
    CHECK_SWAP(a, b);
    CHECK_SWAP(s_rstf, h);
    CHECK_SWAP(s_rstf, s_rstf);

    std::cout << "\nassign\n";
    #define CHECK_ASSIGN(a, method) \
        std::cout << '"' << a << "\" to "; \
        method; \
        std::cout << '"' << a << "\"\n"
    CHECK_ASSIGN(h, h.assign(3, 'T'));
    CHECK_ASSIGN(a, a.assign(6, 'W'));

    return 0;
}
