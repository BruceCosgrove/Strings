#pragma once

#include <gtest/gtest.h>
#include "string.hpp"

constexpr char Small[] = "small";
constexpr auto SmallLength = sizeof(Small) / sizeof(char) - 1;
constexpr char Large[] = "this is a large string";
constexpr auto LargeLength = sizeof(Large) / sizeof(char) - 1;

void AssertEmpty(const string& s);
void AssertSmall(const string& s, bool alsoAssertEquality = true);
void AssertLarge(const string& s, bool alsoAssertEquality = true);
