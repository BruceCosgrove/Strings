#include "common.hpp"

TEST(StringConstructors, NoArgumentConstructed) {
    {
        string s1;

        AssertEmpty(s1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringConstructors, CopyConstructedWith_EmptyString) {
    {
        string s1;
        string s2 = s1;

        AssertEmpty(s1);
        AssertEmpty(s2);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringConstructors, CopyConstructedWith_SmallString) {
    {
        string s1(Small1);
        string s2 = s1;

        AssertSmall(s1, Small1);
        AssertSmall(s2, Small1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringConstructors, CopyConstructedWith_LargeString) {
    {
        string s1(Large1);
        string s2 = s1;

        AssertLarge(s1, Large1);
        AssertLarge(s2, Large1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringConstructors, MoveConstructedWith_EmptyString) {
    {
        string s1;
        string s2 = std::move(s1);

        AssertEmpty(s1);
        AssertEmpty(s2);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringConstructors, MoveConstructedWith_SmallString) {
    {
        string s1(Small1);
        string s2 = std::move(s1);

        AssertEmpty(s1);
        AssertSmall(s2, Small1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringConstructors, MoveConstructedWith_LargeString) {
    {
        string s1(Large1);
        string s2 = std::move(s1);

        AssertEmpty(s1);
        AssertLarge(s2, Large1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringConstructors, ConstructedWith_EmptyCountOfSameElement) {
    {
        string s1(0, 'Q');

        AssertEmpty(s1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringConstructors, ConstructedWith_SmallCountOfSameElement) {
    {
        string s1(5, 'Q');

        ASSERT_TRUE(s1.small());
        ASSERT_FALSE(s1.empty());
        ASSERT_EQ(s1.size(), 5ul);
        ASSERT_EQ(s1.ssize(), 5l);
        ASSERT_EQ(s1.length(), 5ul);
        ASSERT_EQ(s1.capacity(), string::sboc);
        ASSERT_EQ(s1.begin() + 5, s1.end());
        ASSERT_EQ(s1.cbegin() + 5, s1.cend());
        ASSERT_EQ(s1.rbegin() + 5, s1.rend());
        ASSERT_EQ(s1.crbegin() + 5, s1.crend());
        ASSERT_STREQ(s1.data(), "QQQQQ");
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringConstructors, ConstructedWith_LargeCountOfSameElement) {
    {
        string s1(22, 'R');

        ASSERT_FALSE(s1.small());
        ASSERT_FALSE(s1.empty());
        ASSERT_EQ(s1.size(), 22ul);
        ASSERT_EQ(s1.ssize(), 22l);
        ASSERT_EQ(s1.length(), 22ul);
        ASSERT_GE(s1.capacity(), 22ul);
        ASSERT_EQ(s1.begin() + 22, s1.end());
        ASSERT_EQ(s1.cbegin() + 22, s1.cend());
        ASSERT_EQ(s1.rbegin() + 22, s1.rend());
        ASSERT_EQ(s1.crbegin() + 22, s1.crend());
        ASSERT_STREQ(s1.data(), "RRRRRRRRRRRRRRRRRRRRRR");
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringConstructors, ConstructedWith_EmptyStringView) {
    {
        string_view sv1;
        string s1(sv1);

        AssertEmpty(s1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringConstructors, ConstructedWith_SmallStringView) {
    {
        string_view sv1(Small1);
        string s1(sv1);

        AssertSmall(s1, Small1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringConstructors, ConstructedWith_LargeStringView) {
    {
        string_view sv1(Large1);
        string s1(sv1);

        AssertLarge(s1, Large1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringConstructors, ConstructedWith_EmptyConstPointer) {
    {
        string s1("");

        AssertEmpty(s1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringConstructors, ConstructedWith_SmallConstPointer) {
    {
        string s1(Small1.data());

        AssertSmall(s1, Small1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringConstructors, ConstructedWith_LargeConstPointer) {
    {
        string s1(Large1.data());

        AssertLarge(s1, Large1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}
