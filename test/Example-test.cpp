#include <gtest/gtest.h>
#include "Example.hpp"

// Demonstrate some basic assertions.
TEST(ExampleTests, BasicAssertions)
{
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

TEST(ExampleTests, Add)
{
    int a = 14;
    int b = 45;
    EXPECT_EQ(a + b, add(a, b));
}