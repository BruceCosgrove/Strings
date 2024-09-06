#include "common.hpp"

TEST(StringConstructors, NoArgumentConstructor) {
    string s1;
    AssertEmpty(s1);
}

TEST(StringConstructors, CopyConstructor_Small) {
    string s1 = Small;
    string s2 = s1;

    ASSERT_TRUE(s2.small());
    ASSERT_FALSE(s2.empty());
    ASSERT_EQ(s2.size(), SmallLength);
    ASSERT_EQ(s2.ssize(), static_cast<decltype(s2.ssize())>(SmallLength));
    ASSERT_EQ(s2.length(), SmallLength);
    ASSERT_EQ(s2.capacity(), string::sboc);
    ASSERT_EQ(s2.begin() + SmallLength, s2.end());
    ASSERT_EQ(s2.cbegin() + SmallLength, s2.cend());
    ASSERT_EQ(s2.rbegin() + SmallLength, s2.rend());
    ASSERT_EQ(s2.crbegin() + SmallLength, s2.crend());
    ASSERT_STREQ(s2.data(), Small);
}

TEST(StringConstructors, CopyConstructor_Large) {
    string s1 = Large;
    string s2 = s1;

    ASSERT_FALSE(s2.small());
    ASSERT_FALSE(s2.empty());
    ASSERT_EQ(s2.size(), LargeLength);
    ASSERT_EQ(s2.ssize(), static_cast<decltype(s2.ssize())>(LargeLength));
    ASSERT_EQ(s2.length(), LargeLength);
    ASSERT_GE(s2.capacity(), LargeLength);
    ASSERT_EQ(s2.begin() + LargeLength, s2.end());
    ASSERT_EQ(s2.cbegin() + LargeLength, s2.cend());
    ASSERT_EQ(s2.rbegin() + LargeLength, s2.rend());
    ASSERT_EQ(s2.crbegin() + LargeLength, s2.crend());
    ASSERT_STREQ(s2.data(), Large);
}

TEST(StringConstructors, MoveConstructor_Small) {
    string s1 = Small;
    string s2 = std::move(s1);

    ASSERT_TRUE(s2.small());
    ASSERT_FALSE(s2.empty());
    ASSERT_EQ(s2.size(), SmallLength);
    ASSERT_EQ(s2.ssize(), static_cast<decltype(s2.ssize())>(SmallLength));
    ASSERT_EQ(s2.length(), SmallLength);
    ASSERT_EQ(s2.capacity(), string::sboc);
    ASSERT_EQ(s2.begin() + SmallLength, s2.end());
    ASSERT_EQ(s2.cbegin() + SmallLength, s2.cend());
    ASSERT_EQ(s2.rbegin() + SmallLength, s2.rend());
    ASSERT_EQ(s2.crbegin() + SmallLength, s2.crend());
    ASSERT_STREQ(s2.data(), Small);

    AssertEmpty(s1);
}

TEST(StringConstructors, MoveConstructor_Large) {
    string s1 = Large;
    string s2 = std::move(s1);

    ASSERT_FALSE(s2.small());
    ASSERT_FALSE(s2.empty());
    ASSERT_EQ(s2.size(), LargeLength);
    ASSERT_EQ(s2.ssize(), static_cast<decltype(s2.ssize())>(LargeLength));
    ASSERT_EQ(s2.length(), LargeLength);
    ASSERT_GE(s2.capacity(), LargeLength);
    ASSERT_EQ(s2.begin() + LargeLength, s2.end());
    ASSERT_EQ(s2.cbegin() + LargeLength, s2.cend());
    ASSERT_EQ(s2.rbegin() + LargeLength, s2.rend());
    ASSERT_EQ(s2.crbegin() + LargeLength, s2.crend());
    ASSERT_STREQ(s2.data(), Large);

    AssertEmpty(s1);
}

TEST(StringConstructors, size_type_value_type_Constructor_Small) {
    string s1(SmallLength, 'Q');

    ASSERT_TRUE(s1.small());
    ASSERT_FALSE(s1.empty());
    ASSERT_EQ(s1.size(), SmallLength);
    ASSERT_EQ(s1.ssize(), static_cast<decltype(s1.ssize())>(SmallLength));
    ASSERT_EQ(s1.length(), SmallLength);
    ASSERT_EQ(s1.capacity(), string::sboc);
    ASSERT_EQ(s1.begin() + SmallLength, s1.end());
    ASSERT_EQ(s1.cbegin() + SmallLength, s1.cend());
    ASSERT_EQ(s1.rbegin() + SmallLength, s1.rend());
    ASSERT_EQ(s1.crbegin() + SmallLength, s1.crend());
    ASSERT_STREQ(s1.data(), "QQQQQ");
}

TEST(StringConstructors, size_type_value_type_Constructor_Large) {
    string s1(LargeLength, 'R');

    ASSERT_FALSE(s1.small());
    ASSERT_FALSE(s1.empty());
    ASSERT_EQ(s1.size(), LargeLength);
    ASSERT_EQ(s1.ssize(), static_cast<decltype(s1.ssize())>(LargeLength));
    ASSERT_EQ(s1.length(), LargeLength);
    ASSERT_GE(s1.capacity(), LargeLength);
    ASSERT_EQ(s1.begin() + LargeLength, s1.end());
    ASSERT_EQ(s1.cbegin() + LargeLength, s1.cend());
    ASSERT_EQ(s1.rbegin() + LargeLength, s1.rend());
    ASSERT_EQ(s1.crbegin() + LargeLength, s1.crend());
    ASSERT_STREQ(s1.data(), "RRRRRRRRRRRRRRRRRRRRRR");
}

TEST(StringConstructors, string_view_type_Constructor_Small) {
    string_view sv1 = Small;
    string s1(sv1);

    ASSERT_TRUE(s1.small());
    ASSERT_FALSE(s1.empty());
    ASSERT_EQ(s1.size(), SmallLength);
    ASSERT_EQ(s1.ssize(), static_cast<decltype(s1.ssize())>(SmallLength));
    ASSERT_EQ(s1.length(), SmallLength);
    ASSERT_EQ(s1.capacity(), string::sboc);
    ASSERT_EQ(s1.begin() + SmallLength, s1.end());
    ASSERT_EQ(s1.cbegin() + SmallLength, s1.cend());
    ASSERT_EQ(s1.rbegin() + SmallLength, s1.rend());
    ASSERT_EQ(s1.crbegin() + SmallLength, s1.crend());
    ASSERT_STREQ(s1.data(), Small);
}

TEST(StringConstructors, string_view_type_Constructor_Large) {
    string_view sv1 = Large;
    string s1(sv1);

    ASSERT_FALSE(s1.small());
    ASSERT_FALSE(s1.empty());
    ASSERT_EQ(s1.size(), LargeLength);
    ASSERT_EQ(s1.ssize(), static_cast<decltype(s1.ssize())>(LargeLength));
    ASSERT_EQ(s1.length(), LargeLength);
    ASSERT_GE(s1.capacity(), LargeLength);
    ASSERT_EQ(s1.begin() + LargeLength, s1.end());
    ASSERT_EQ(s1.cbegin() + LargeLength, s1.cend());
    ASSERT_EQ(s1.rbegin() + LargeLength, s1.rend());
    ASSERT_EQ(s1.crbegin() + LargeLength, s1.crend());
    ASSERT_STREQ(s1.data(), Large);
}

TEST(StringConstructors, const_pointer_Constructor_Small) {
    string s1 = Small;

    ASSERT_TRUE(s1.small());
    ASSERT_FALSE(s1.empty());
    ASSERT_EQ(s1.size(), SmallLength);
    ASSERT_EQ(s1.ssize(), static_cast<decltype(s1.ssize())>(SmallLength));
    ASSERT_EQ(s1.length(), SmallLength);
    ASSERT_EQ(s1.capacity(), string::sboc);
    ASSERT_EQ(s1.begin() + SmallLength, s1.end());
    ASSERT_EQ(s1.cbegin() + SmallLength, s1.cend());
    ASSERT_EQ(s1.rbegin() + SmallLength, s1.rend());
    ASSERT_EQ(s1.crbegin() + SmallLength, s1.crend());
    ASSERT_STREQ(s1.data(), Small);
}

TEST(StringConstructors, const_pointer_Constructor_Large) {
    string s1 = Large;

    ASSERT_FALSE(s1.small());
    ASSERT_FALSE(s1.empty());
    ASSERT_EQ(s1.size(), LargeLength);
    ASSERT_EQ(s1.ssize(), static_cast<decltype(s1.ssize())>(LargeLength));
    ASSERT_EQ(s1.length(), LargeLength);
    ASSERT_GE(s1.capacity(), LargeLength);
    ASSERT_EQ(s1.begin() + LargeLength, s1.end());
    ASSERT_EQ(s1.cbegin() + LargeLength, s1.cend());
    ASSERT_EQ(s1.rbegin() + LargeLength, s1.rend());
    ASSERT_EQ(s1.crbegin() + LargeLength, s1.crend());
    ASSERT_STREQ(s1.data(), Large);
}
