#include <gtest/gtest.h>
#include "tattle/tattle.hpp"

// Demonstrate some basic assertions.
TEST(TattleTests, BasicAssertions)
{
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

TEST(TattleTests, TattleIntExample)
{
    int expected = 0;
    EXPECT_EQ(expected, tattle::TattleIntExample());
}