#include "common.hpp"

using signed_string = basic_string<char, std::make_signed_t<std::size_t>>;

TEST(StringElementAccess, EmptyString_Subscripted) {
    {
        signed_string s1;

        ASSERT_EQ(&s1[-1], &signed_string::nult);
        ASSERT_EQ(&s1[Small1.size()], &signed_string::nult);
        ASSERT_EQ(&s1[signed_string::npos], &signed_string::nult);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, SmallString_Subscripted) {
    {
        signed_string s1(Small1.data());

        ASSERT_EQ(&s1[-1], &signed_string::nult);
        ASSERT_EQ(s1[0], Small1[0]);
        ASSERT_EQ(s1[Small1.size() - 1], Small1[Small1.size() - 1]);
        ASSERT_EQ(&s1[Small1.size()], &signed_string::nult);
        ASSERT_EQ(&s1[signed_string::npos], &signed_string::nult);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, LargeString_Subscripted) {
    {
        signed_string s1(Large1.data());

        ASSERT_EQ(&s1[-1], &signed_string::nult);
        ASSERT_EQ(s1[0], Large1[0]);
        ASSERT_EQ(s1[Large1.size() - 1], Large1[Large1.size() - 1]);
        ASSERT_EQ(&s1[Large1.size()], &signed_string::nult);
        ASSERT_EQ(&s1[signed_string::npos], &signed_string::nult);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, EmptyString_At) {
    {
        signed_string s1;

        ASSERT_EQ(&s1.at(-1), &signed_string::nult);
        ASSERT_EQ(&s1.at(Small1.size()), &signed_string::nult);
        ASSERT_EQ(&s1.at(signed_string::npos), &signed_string::nult);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, SmallString_At) {
    {
        signed_string s1(Small1.data());

        ASSERT_EQ(&s1.at(-1), &signed_string::nult);
        ASSERT_EQ(s1.at(0), Small1[0]);
        ASSERT_EQ(s1.at(Small1.size() - 1), Small1[Small1.size() - 1]);
        ASSERT_EQ(&s1.at(Small1.size()), &signed_string::nult);
        ASSERT_EQ(&s1.at(signed_string::npos), &signed_string::nult);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, LargeString_At) {
    {
        signed_string s1(Large1.data());

        ASSERT_EQ(&s1.at(-1), &signed_string::nult);
        ASSERT_EQ(s1.at(0), Large1[0]);
        ASSERT_EQ(s1.at(Large1.size() - 1), Large1[Large1.size() - 1]);
        ASSERT_EQ(&s1.at(Large1.size()), &signed_string::nult);
        ASSERT_EQ(&s1.at(signed_string::npos), &signed_string::nult);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, EmptyString_Front) {
    {
        string s1;

        ASSERT_EQ(&s1.front(), &string::nult);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, SmallString_Front) {
    {
        string s1(Small1.data());

        ASSERT_EQ(s1.front(), Small1[0]);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, LargeString_Front) {
    {
        string s1(Large1.data());

        ASSERT_EQ(s1.front(), Large1[0]);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, EmptyString_Back) {
    {
        string s1;

        ASSERT_EQ(&s1.back(), &string::nult);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, SmallString_Back) {
    {
        string s1(Small1.data());

        ASSERT_EQ(s1.back(), Small1[Small1.size() - 1]);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, LargeString_Back) {
    {
        string s1(Large1.data());

        ASSERT_EQ(s1.back(), Large1[Large1.size() - 1]);
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, EmptyString_Data) {
    {
        string s1;

        // The string's data is within its object storage, meaning it's in small mode.
        ASSERT_TRUE((void*)&s1 <= s1.data() && s1.data() < (void*)(&s1 + 1));
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, SmallString_Data) {
    {
        string s1(Small1.data());

        // The string's data is within its object storage, meaning it's in small mode.
        ASSERT_TRUE((void*)&s1 <= s1.data() && s1.data() < (void*)(&s1 + 1));
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, LargeString_Data) {
    {
        string s1(Large1.data());

        // The string's data is NOT within its object storage, meaning it's in large mode.
        ASSERT_FALSE((void*)&s1 <= s1.data() && s1.data() < (void*)(&s1 + 1));
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, EmptyString_CStr) {
    {
        string s1;

        // The string's data is within its object storage, meaning it's in small mode.
        ASSERT_TRUE((void*)&s1 <= s1.c_str() && s1.c_str() < (void*)(&s1 + 1));
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, SmallString_CStr) {
    {
        string s1(Small1.data());

        // The string's data is within its object storage, meaning it's in small mode.
        ASSERT_TRUE((void*)&s1 <= s1.c_str() && s1.c_str() < (void*)(&s1 + 1));
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, LargeString_CStr) {
    {
        string s1(Large1.data());

        // The string's data is NOT within its object storage, meaning it's in large mode.
        ASSERT_FALSE((void*)&s1 <= s1.c_str() && s1.c_str() < (void*)(&s1 + 1));
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, EmptyString_OperatorStringView) {
    {
        string s1;
        string_view sv1 = s1;

        ASSERT_STREQ(s1.data(), sv1.data());
        ASSERT_EQ(s1.size(), sv1.size());
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, SmallString_OperatorStringView) {
    {
        string s1(Small1);
        string_view sv1 = s1;

        ASSERT_STREQ(s1.data(), sv1.data());
        ASSERT_EQ(s1.size(), sv1.size());
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}

TEST(StringElementAccess, LargeString_OperatorStringView) {
    {
        string s1(Large1);
        string_view sv1 = s1;

        ASSERT_STREQ(s1.data(), sv1.data());
        ASSERT_EQ(s1.size(), sv1.size());
    }
    ASSERT_TRUE(allocation_tracker::query_and_reset());
}
