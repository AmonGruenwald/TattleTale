#include <gtest/gtest.h>
#include "tale/tale.hpp"

// Demonstrate some basic assertions.
TEST(TaleTests, BasicAssertions)
{
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

TEST(TaleTests, TaleIntExample)
{
    int expected = 1;
    EXPECT_EQ(expected, tale::TaleIntExample());
}

TEST(TaleTests, CreateDefaultActor)
{
    tale::Actor actor;
    EXPECT_EQ("John Doe", actor.name_);
}