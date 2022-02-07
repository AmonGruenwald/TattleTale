#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <memory>
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

TEST(TaleTests, IncreasingKernelNumber)
{
    std::vector<std::weak_ptr<tale::Kernel>> default_reasons;
    tale::Action action(default_reasons);
    tale::Emotion emotion(default_reasons);
    tale::Goal goal(default_reasons);
    tale::Interaction interaction(default_reasons);
    tale::Relationship relationship(default_reasons);
    tale::Resource resource(default_reasons);
    tale::Trait trait(default_reasons);

    EXPECT_EQ(0, action.number_);
    EXPECT_EQ(1, emotion.number_);
    EXPECT_EQ(2, goal.number_);
    EXPECT_EQ(3, interaction.number_);
    EXPECT_EQ(4, relationship.number_);
    EXPECT_EQ(5, resource.number_);
    EXPECT_EQ(6, trait.number_);
}
// TODO: Add more Kernel Tests: correct storing of other Kernels, special tests for each type etc.