#include "common.hpp"

TEST(StringCopyAssignmentOperators, EmptyString_CopyAssignedWith_EmptyString) {
    string s1;
    string s2;
    s1 = s2;

    AssertEmpty(s1);
    AssertEmpty(s2);
}

TEST(StringCopyAssignmentOperators, EmptyString_CopyAssignedWith_SmallString) {
    string s1;
    string s2(Small1);
    s1 = s2;

    AssertSmall(s1, Small1);
    AssertSmall(s2, Small1);
}

TEST(StringCopyAssignmentOperators, EmptyString_CopyAssignedWith_LargeString) {
    string s1;
    string s2(Large1);
    s1 = s2;

    AssertLarge(s1, Large1);
    AssertLarge(s2, Large1);
}

TEST(StringCopyAssignmentOperators, SmallString_CopyAssignedWith_EmptyString) {
    string s1(Small1);
    string s2;
    s1 = s2;

    AssertEmpty(s1);
    AssertEmpty(s2);
}

TEST(StringCopyAssignmentOperators, SmallString_CopyAssignedWith_SmallString) {
    string s1(Small1);
    string s2(Small2);
    s1 = s2;

    AssertSmall(s1, Small2);
    AssertSmall(s2, Small2);
}

TEST(StringCopyAssignmentOperators, SmallString_CopyAssignedWith_LargeString) {
    string s1(Small1);
    string s2(Large1);
    s1 = s2;

    AssertLarge(s1, Large1);
    AssertLarge(s2, Large1);
}

TEST(StringCopyAssignmentOperators, LargeString_CopyAssignedWith_EmptyString) {
    string s1(Large1);
    string s2;
    s1 = s2;

    AssertEmptyLarge(s1, Large1);
    AssertEmpty(s2);
}

TEST(StringCopyAssignmentOperators, LargeString_CopyAssignedWith_SmallString) {
    string s1(Large1);
    string s2(Small1);
    s1 = s2;

    AssertLarge(s1, Small1);
    AssertSmall(s2, Small1);
}

TEST(StringCopyAssignmentOperators, LargeString_CopyAssignedWith_SmallerLargeString) {
    string s1(Large1);
    string s2(Large2);
    s1 = s2;

    AssertLarge(s1, Large2);
    AssertLarge(s2, Large2);
}

TEST(StringCopyAssignmentOperators, LargeString_CopyAssignedWith_LargerLargeString) {
    string s1(Large2);
    string s2(Large1);
    s1 = s2;

    AssertLarge(s1, Large1);
    AssertLarge(s2, Large1);
}
