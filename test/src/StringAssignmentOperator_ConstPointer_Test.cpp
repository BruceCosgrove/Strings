#include "common.hpp"

TEST(StringAssignmentOperator_ConstPointer, EmptyString_AssignedWith_EmptyStringView) {
    {
        string s1;
        s1 = Empty.data();

        AssertEmpty(s1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringAssignmentOperator_ConstPointer, EmptyString_AssignedWith_SmallStringView) {
    {
        string s1;
        s1 = Small1.data();

        AssertSmall(s1, Small1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringAssignmentOperator_ConstPointer, EmptyString_AssignedWith_LargeStringView) {
    {
        string s1;
        s1 = Large1.data();

        AssertLarge(s1, Large1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringAssignmentOperator_ConstPointer, SmallString_AssignedWith_EmptyStringView) {
    {
        string s1(Small1);
        s1 = Empty.data();

        AssertEmpty(s1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringAssignmentOperator_ConstPointer, SmallString_AssignedWith_SmallStringView) {
    {
        string s1(Small1);
        s1 = Small2.data();

        AssertSmall(s1, Small2);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringAssignmentOperator_ConstPointer, SmallString_AssignedWith_LargeStringView) {
    {
        string s1(Small1);
        s1 = Large1.data();

        AssertLarge(s1, Large1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringAssignmentOperator_ConstPointer, LargeString_AssignedWith_EmptyStringView) {
    {
        string s1(Large1);
        s1 = Empty.data();

        AssertEmptyLarge(s1, Large1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringAssignmentOperator_ConstPointer, LargeString_AssignedWith_SmallStringView) {
    {
        string s1(Large1);
        s1 = Small1.data();

        AssertLarge(s1, Small1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringAssignmentOperator_ConstPointer, LargeString_AssignedWith_SmallerLargeStringView) {
    {
        string s1(Large1);
        s1 = Large2.data();

        AssertLarge(s1, Large2);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}

TEST(StringAssignmentOperator_ConstPointer, LargeString_AssignedWith_LargerLargeStringView) {
    {
        string s1(Large2);
        s1 = Large1.data();

        AssertLarge(s1, Large1);
    }
    ASSERT_TRUE(allocation_tracker::allocations_match_deallocations());
}
