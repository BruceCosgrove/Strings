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

void AssertSmall(const string& s, bool alsoAssertEquality)
{
    ASSERT_TRUE(s.small());
    ASSERT_FALSE(s.empty());
    ASSERT_EQ(s.size(), SmallLength);
    ASSERT_EQ(s.ssize(), static_cast<decltype(s.ssize())>(SmallLength));
    ASSERT_EQ(s.length(), SmallLength);
    ASSERT_EQ(s.capacity(), string::sboc);
    ASSERT_EQ(s.begin() + SmallLength, s.end());
    ASSERT_EQ(s.cbegin() + SmallLength, s.cend());
    ASSERT_EQ(s.rbegin() + SmallLength, s.rend());
    ASSERT_EQ(s.crbegin() + SmallLength, s.crend());
    if (alsoAssertEquality)
        { ASSERT_STREQ(s.data(), Small); }
}

void AssertLarge(const string& s, bool alsoAssertEquality)
{
    ASSERT_FALSE(s.small());
    ASSERT_FALSE(s.empty());
    ASSERT_EQ(s.size(), LargeLength);
    ASSERT_EQ(s.ssize(), static_cast<decltype(s.ssize())>(LargeLength));
    ASSERT_EQ(s.length(), LargeLength);
    ASSERT_GE(s.capacity(), LargeLength);
    ASSERT_EQ(s.begin() + LargeLength, s.end());
    ASSERT_EQ(s.cbegin() + LargeLength, s.cend());
    ASSERT_EQ(s.rbegin() + LargeLength, s.rend());
    ASSERT_EQ(s.crbegin() + LargeLength, s.crend());
    if (alsoAssertEquality)
        { ASSERT_STREQ(s.data(), Large); }
}

// Note: in the above two methods, there are extra curly braces.
// This is because, in my environment, GoogleTest's
// GTEST_AMBIGUOUS_ELSE_BLOCKER_ fails to do its one job.
