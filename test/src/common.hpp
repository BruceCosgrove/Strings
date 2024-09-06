#pragma once

#include <gtest/gtest.h>
#include "string.hpp"

static constexpr char Small[] = "small";
static constexpr auto SmallLength = sizeof(Small) / sizeof(char) - 1;
static constexpr char Large[] = "this is a large string";
static constexpr auto LargeLength = sizeof(Large) / sizeof(char) - 1;

static void AssertEmpty(const string& s)
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
