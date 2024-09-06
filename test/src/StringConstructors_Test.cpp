#include "common.hpp"

TEST(StringConstructors, NoArgumentConstructed) {
    string s1;

    AssertEmpty(s1);
}

TEST(StringConstructors, CopyConstructedWith_SmallString) {
    string s1(Small1);
    string s2 = s1;

    AssertSmall(s1, Small1);
    AssertSmall(s2, Small1);
}

TEST(StringConstructors, CopyConstructedWith_LargeString) {
    string s1(Large1);
    string s2 = s1;

    AssertLarge(s1, Large1);
    AssertLarge(s2, Large1);
}

TEST(StringConstructors, MoveConstructedWith_SmallString) {
    string s1(Small1);
    string s2 = std::move(s1);

    AssertEmpty(s1);
    AssertSmall(s2, Small1);
}

TEST(StringConstructors, MoveConstructedWith_LargeString) {
    string s1(Large1);
    string s2 = std::move(s1);

    AssertEmpty(s1);
    AssertLarge(s2, Large1);
}

TEST(StringConstructors, ConstructedWith_SmallCountOfSameElement) {
    string s1(5, 'Q');

    AssertSmall(s1, Small1, false);
    ASSERT_STREQ(s1.data(), "QQQQQ");
}

TEST(StringConstructors, ConstructedWith_LargeCountOfSameElement) {
    string s1(22, 'R');

    AssertLarge(s1, Large1, false);
    ASSERT_STREQ(s1.data(), "RRRRRRRRRRRRRRRRRRRRRR");
}

TEST(StringConstructors, ConstructedWith_SmallStringView) {
    string_view sv1(Small1);
    string s1(sv1);

    AssertSmall(s1, Small1);
}

TEST(StringConstructors, ConstructedWith_LargeStringView) {
    string_view sv1(Large1);
    string s1(sv1);

    AssertLarge(s1, Large1);
}

TEST(StringConstructors, ConstructedWith_SmallConstPointer) {
    string s1(Small1);

    AssertSmall(s1, Small1);
}

TEST(StringConstructors, ConstructedWith_LargeConstPointer) {
    string s1(Large1);

    AssertLarge(s1, Large1);
}
