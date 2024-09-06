#include "common.hpp"

TEST(StringMoveAssignmentOperators, EmptyString_MoveAssignedWith_EmptyString) {
    string s1;
    string s2;
    s1 = std::move(s2);

    AssertEmpty(s1);
    AssertEmpty(s2);
}

TEST(StringMoveAssignmentOperators, EmptyString_MoveAssignedWith_SmallString) {
    string s1;
    string s2(Small1);
    s1 = std::move(s2);

    AssertSmall(s1, Small1);
    AssertEmpty(s2);
}

TEST(StringMoveAssignmentOperators, EmptyString_MoveAssignedWith_LargeString) {
    string s1;
    string s2(Large1);
    s1 = std::move(s2);

    AssertLarge(s1, Large1);
    AssertEmpty(s2);
}

TEST(StringMoveAssignmentOperators, SmallString_MoveAssignedWith_EmptyString) {
    string s1(Small1);
    string s2;
    s1 = std::move(s2);

    AssertEmpty(s1);
    AssertEmpty(s2);
}

TEST(StringMoveAssignmentOperators, SmallString_MoveAssignedWith_SmallString) {
    string s1(Small1);
    string s2(Small2);
    s1 = std::move(s2);

    AssertSmall(s1, Small2);
    AssertEmpty(s2);
}

TEST(StringMoveAssignmentOperators, SmallString_MoveAssignedWith_LargeString) {
    string s1(Small1);
    string s2(Large1);
    s1 = std::move(s2);

    AssertLarge(s1, Large1);
    AssertEmpty(s2);
}

TEST(StringMoveAssignmentOperators, LargeString_MoveAssignedWith_EmptyString) {
    string s1(Large1);
    string s2;
    s1 = std::move(s2);

    AssertEmptyLarge(s1, Large1);
    AssertEmpty(s2);
}

TEST(StringMoveAssignmentOperators, LargeString_MoveAssignedWith_SmallString) {
    string s1(Large1);
    string s2(Small1);
    s1 = std::move(s2);

    AssertLarge(s1, Small1);
    AssertEmpty(s2);
}

TEST(StringMoveAssignmentOperators, LargeString_MoveAssignedWith_SmallerLargeString) {
    string s1(Large2);
    string s2(Large1);
    s1 = std::move(s2);

    AssertLarge(s1, Large1);
    AssertEmpty(s2);
}

TEST(StringMoveAssignmentOperators, LargeString_MoveAssignedWith_LargerLargeString) {
    string s1(Large1);
    string s2(Large2);
    s1 = std::move(s2);

    AssertLarge(s1, Large2);
    AssertEmpty(s2);
}
