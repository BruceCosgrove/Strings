#include <gtest/gtest.h>
#include "string.hpp"

static constexpr char Small[] = "hello";
static constexpr std::size_t SmallLength = sizeof(Small) / sizeof(char) - 1;
static constexpr char Large[] = "hello from large string";
static constexpr std::size_t LargeLength = sizeof(Large) / sizeof(char) - 1;

static void TestEmpty(string& s)
{
    ASSERT_TRUE(s.small());
    ASSERT_TRUE(s.empty());
    ASSERT_EQ(s.size(), 0);
    ASSERT_EQ(s.ssize(), 0);
    ASSERT_EQ(s.length(), 0);
    ASSERT_EQ(s.capacity(), string::sboc);
    ASSERT_EQ(s.begin(), s.end());
    ASSERT_EQ(s.cbegin(), s.cend());
    ASSERT_EQ(s.rbegin(), s.rend());
    ASSERT_EQ(s.crbegin(), s.crend());
    ASSERT_STREQ(s.data(), "");
}

TEST(StringConstructors, NoArgumentConstructor) {
    string s;
    TestEmpty(s);
}

TEST(StringConstructors, CopyConstructor_Small) {
    string s0 = Small;
    string s = s0;

    ASSERT_TRUE(s.small());
    ASSERT_FALSE(s.empty());
    ASSERT_EQ(s.size(), SmallLength);
    ASSERT_EQ(s.ssize(), SmallLength);
    ASSERT_EQ(s.length(), SmallLength);
    ASSERT_EQ(s.capacity(), string::sboc);
    ASSERT_EQ(s.begin() + SmallLength, s.end());
    ASSERT_EQ(s.cbegin() + SmallLength, s.cend());
    ASSERT_EQ(s.rbegin() + SmallLength, s.rend());
    ASSERT_EQ(s.crbegin() + SmallLength, s.crend());
    ASSERT_STREQ(s.data(), Small);
}

TEST(StringConstructors, CopyConstructor_Large) {
    string s0 = Large;
    string s = s0;

    ASSERT_FALSE(s.small());
    ASSERT_FALSE(s.empty());
    ASSERT_EQ(s.size(), LargeLength);
    ASSERT_EQ(s.ssize(), LargeLength);
    ASSERT_EQ(s.length(), LargeLength);
    ASSERT_GE(s.capacity(), LargeLength);
    ASSERT_EQ(s.begin() + LargeLength, s.end());
    ASSERT_EQ(s.cbegin() + LargeLength, s.cend());
    ASSERT_EQ(s.rbegin() + LargeLength, s.rend());
    ASSERT_EQ(s.crbegin() + LargeLength, s.crend());
    ASSERT_STREQ(s.data(), Large);
}

TEST(StringConstructors, MoveConstructor_Small) {
    string s0 = Small;
    string s = std::move(s0);

    ASSERT_TRUE(s.small());
    ASSERT_FALSE(s.empty());
    ASSERT_EQ(s.size(), SmallLength);
    ASSERT_EQ(s.ssize(), SmallLength);
    ASSERT_EQ(s.length(), SmallLength);
    ASSERT_EQ(s.capacity(), string::sboc);
    ASSERT_EQ(s.begin() + SmallLength, s.end());
    ASSERT_EQ(s.cbegin() + SmallLength, s.cend());
    ASSERT_EQ(s.rbegin() + SmallLength, s.rend());
    ASSERT_EQ(s.crbegin() + SmallLength, s.crend());
    ASSERT_STREQ(s.data(), Small);

    TestEmpty(s0);
}

TEST(StringConstructors, MoveConstructor_Large) {
    string s0 = Large;
    string s = std::move(s0);

    ASSERT_FALSE(s.small());
    ASSERT_FALSE(s.empty());
    ASSERT_EQ(s.size(), LargeLength);
    ASSERT_EQ(s.ssize(), LargeLength);
    ASSERT_EQ(s.length(), LargeLength);
    ASSERT_GE(s.capacity(), LargeLength);
    ASSERT_EQ(s.begin() + LargeLength, s.end());
    ASSERT_EQ(s.cbegin() + LargeLength, s.cend());
    ASSERT_EQ(s.rbegin() + LargeLength, s.rend());
    ASSERT_EQ(s.crbegin() + LargeLength, s.crend());
    ASSERT_STREQ(s.data(), Large);

    TestEmpty(s0);
}

TEST(StringConstructors, size_type_value_type_Constructor_Small) {
    string s(SmallLength, 'Q');

    ASSERT_TRUE(s.small());
    ASSERT_FALSE(s.empty());
    ASSERT_EQ(s.size(), SmallLength);
    ASSERT_EQ(s.ssize(), SmallLength);
    ASSERT_EQ(s.length(), SmallLength);
    ASSERT_EQ(s.capacity(), string::sboc);
    ASSERT_EQ(s.begin() + SmallLength, s.end());
    ASSERT_EQ(s.cbegin() + SmallLength, s.cend());
    ASSERT_EQ(s.rbegin() + SmallLength, s.rend());
    ASSERT_EQ(s.crbegin() + SmallLength, s.crend());
    ASSERT_STREQ(s.data(), "QQQQQ");
}

TEST(StringConstructors, size_type_value_type_Constructor_Large) {
    string s(LargeLength, 'R');

    ASSERT_FALSE(s.small());
    ASSERT_FALSE(s.empty());
    ASSERT_EQ(s.size(), LargeLength);
    ASSERT_EQ(s.ssize(), LargeLength);
    ASSERT_EQ(s.length(), LargeLength);
    ASSERT_GE(s.capacity(), LargeLength);
    ASSERT_EQ(s.begin() + LargeLength, s.end());
    ASSERT_EQ(s.cbegin() + LargeLength, s.cend());
    ASSERT_EQ(s.rbegin() + LargeLength, s.rend());
    ASSERT_EQ(s.crbegin() + LargeLength, s.crend());
    ASSERT_STREQ(s.data(), "RRRRRRRRRRRRRRRRRRRRRRR");
}

TEST(StringConstructors, string_view_type_Constructor_Small) {
    string_view sv = Small;
    string s(sv);

    ASSERT_TRUE(s.small());
    ASSERT_FALSE(s.empty());
    ASSERT_EQ(s.size(), SmallLength);
    ASSERT_EQ(s.ssize(), SmallLength);
    ASSERT_EQ(s.length(), SmallLength);
    ASSERT_EQ(s.capacity(), string::sboc);
    ASSERT_EQ(s.begin() + SmallLength, s.end());
    ASSERT_EQ(s.cbegin() + SmallLength, s.cend());
    ASSERT_EQ(s.rbegin() + SmallLength, s.rend());
    ASSERT_EQ(s.crbegin() + SmallLength, s.crend());
    ASSERT_STREQ(s.data(), Small);
}

TEST(StringConstructors, string_view_type_Constructor_Large) {
    string_view sv = Large;
    string s(sv);

    ASSERT_FALSE(s.small());
    ASSERT_FALSE(s.empty());
    ASSERT_EQ(s.size(), LargeLength);
    ASSERT_EQ(s.ssize(), LargeLength);
    ASSERT_EQ(s.length(), LargeLength);
    ASSERT_GE(s.capacity(), LargeLength);
    ASSERT_EQ(s.begin() + LargeLength, s.end());
    ASSERT_EQ(s.cbegin() + LargeLength, s.cend());
    ASSERT_EQ(s.rbegin() + LargeLength, s.rend());
    ASSERT_EQ(s.crbegin() + LargeLength, s.crend());
    ASSERT_STREQ(s.data(), Large);
}

TEST(StringConstructors, const_pointer_Constructor_Small) {
    string s = Small;

    ASSERT_TRUE(s.small());
    ASSERT_FALSE(s.empty());
    ASSERT_EQ(s.size(), SmallLength);
    ASSERT_EQ(s.ssize(), SmallLength);
    ASSERT_EQ(s.length(), SmallLength);
    ASSERT_EQ(s.capacity(), string::sboc);
    ASSERT_EQ(s.begin() + SmallLength, s.end());
    ASSERT_EQ(s.cbegin() + SmallLength, s.cend());
    ASSERT_EQ(s.rbegin() + SmallLength, s.rend());
    ASSERT_EQ(s.crbegin() + SmallLength, s.crend());
    ASSERT_STREQ(s.data(), Small);
}

TEST(StringConstructors, const_pointer_Constructor_Large) {
    string s = Large;

    ASSERT_FALSE(s.small());
    ASSERT_FALSE(s.empty());
    ASSERT_EQ(s.size(), LargeLength);
    ASSERT_EQ(s.ssize(), LargeLength);
    ASSERT_EQ(s.length(), LargeLength);
    ASSERT_GE(s.capacity(), LargeLength);
    ASSERT_EQ(s.begin() + LargeLength, s.end());
    ASSERT_EQ(s.cbegin() + LargeLength, s.cend());
    ASSERT_EQ(s.rbegin() + LargeLength, s.rend());
    ASSERT_EQ(s.crbegin() + LargeLength, s.crend());
    ASSERT_STREQ(s.data(), Large);
}
