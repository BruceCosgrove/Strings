#include "common.hpp"

TEST(StringAssignmentOperator_StringView, EmptyString_AssignedWith_EmptyStringView) {
    {
        string s1;
        string_view sv1;
        s1 = sv1;

        AssertEmpty(s1);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringAssignmentOperator_StringView, EmptyString_AssignedWith_SmallStringView) {
    {
        string s1;
        string_view sv1(Small1);
        s1 = sv1;

        AssertSmall(s1, Small1);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringAssignmentOperator_StringView, EmptyString_AssignedWith_LargeStringView) {
    {
        string s1;
        string_view sv1(Large1);
        s1 = sv1;

        AssertLarge(s1, Large1);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringAssignmentOperator_StringView, SmallString_AssignedWith_EmptyStringView) {
    {
        string s1(Small1);
        string_view sv1;
        s1 = sv1;

        AssertEmpty(s1);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringAssignmentOperator_StringView, SmallString_AssignedWith_SmallStringView) {
    {
        string s1(Small1);
        string_view sv1(Small2);
        s1 = sv1;

        AssertSmall(s1, Small2);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringAssignmentOperator_StringView, SmallString_AssignedWith_LargeStringView) {
    {
        string s1(Small1);
        string_view sv1(Large1);
        s1 = sv1;

        AssertLarge(s1, Large1);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringAssignmentOperator_StringView, LargeString_AssignedWith_EmptyStringView) {
    {
        string s1(Large1);
        string_view sv1;
        s1 = sv1;

        AssertEmptyLarge(s1, Large1);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringAssignmentOperator_StringView, LargeString_AssignedWith_SmallStringView) {
    {
        string s1(Large1);
        string_view sv1(Small1);
        s1 = sv1;

        AssertLarge(s1, Small1);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringAssignmentOperator_StringView, LargeString_AssignedWith_SmallerLargeStringView) {
    {
        string s1(Large1);
        string_view sv1(Large2);
        s1 = sv1;

        AssertLarge(s1, Large2);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringAssignmentOperator_StringView, LargeString_AssignedWith_LargerLargeStringView) {
    {
        string s1(Large2);
        string_view sv1(Large1);
        s1 = sv1;

        AssertLarge(s1, Large1);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}
