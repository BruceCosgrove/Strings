#include "common.hpp"

void AssertEmpty(const string& s)
{
    ASSERT_TRUE(s.small());
    ASSERT_TRUE(s.empty());
    ASSERT_EQ(s.size(), 0ul);
    ASSERT_EQ(s.ssize(), 0l);
    ASSERT_EQ(s.length(), 0ul);
    ASSERT_EQ(s.capacity(), string::sboc);
    ASSERT_EQ(s.begin(), s.end());
    ASSERT_EQ(s.cbegin(), s.cend());
    ASSERT_EQ(s.rbegin(), s.rend());
    ASSERT_EQ(s.crbegin(), s.crend());
    ASSERT_STREQ(s.data(), "");
}

void AssertEmptyLarge(const string& s, string_view expected)
{
    ASSERT_FALSE(s.small());
    ASSERT_TRUE(s.empty());
    ASSERT_EQ(s.size(), 0ul);
    ASSERT_EQ(s.ssize(), 0l);
    ASSERT_EQ(s.length(), 0ul);
    ASSERT_GE(s.capacity(), expected.size());
    ASSERT_EQ(s.begin(), s.end());
    ASSERT_EQ(s.cbegin(), s.cend());
    ASSERT_EQ(s.rbegin(), s.rend());
    ASSERT_EQ(s.crbegin(), s.crend());
    ASSERT_STREQ(s.data(), "");
}

void AssertSmall(const string& s, string_view expected)
{
    ASSERT_TRUE(s.small());
    ASSERT_FALSE(s.empty());
    ASSERT_EQ(s.size(), expected.size());
    ASSERT_EQ(s.ssize(), expected.ssize());
    ASSERT_EQ(s.length(), expected.length());
    ASSERT_EQ(s.capacity(), string::sboc);
    ASSERT_EQ(s.begin() + expected.size(), s.end());
    ASSERT_EQ(s.cbegin() + expected.size(), s.cend());
    ASSERT_EQ(s.rbegin() + expected.size(), s.rend());
    ASSERT_EQ(s.crbegin() + expected.size(), s.crend());
    ASSERT_STREQ(s.data(), expected.data());
}

void AssertLarge(const string& s, string_view expected)
{
    ASSERT_FALSE(s.small());
    ASSERT_FALSE(s.empty());
    ASSERT_EQ(s.size(), expected.size());
    ASSERT_EQ(s.ssize(), expected.ssize());
    ASSERT_EQ(s.length(), expected.length());
    ASSERT_GE(s.capacity(), expected.size());
    ASSERT_EQ(s.begin() + expected.size(), s.end());
    ASSERT_EQ(s.cbegin() + expected.size(), s.cend());
    ASSERT_EQ(s.rbegin() + expected.size(), s.rend());
    ASSERT_EQ(s.crbegin() + expected.size(), s.crend());
    ASSERT_STREQ(s.data(), expected.data());
}
