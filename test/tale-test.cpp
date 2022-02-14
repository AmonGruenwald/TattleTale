#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <memory>
#include "tale/tale.hpp"

TEST(TaleTests, CreateDefaultActor)
{
    tale::Random random;
    tale::Setting setting;
    tale::InteractionStore interaction_store;
    tale::School school(random, setting, interaction_store);
    tale::Actor actor(random, setting, school, interaction_store, 0);
    EXPECT_EQ("John Doe", actor.name_);
}

TEST(TaleTests, IncreasingKernelNumber)
{
    tale::Kernel::current_number_ = 0;
    std::vector<std::weak_ptr<tale::Kernel>> default_reasons;
    size_t tick = 0;
    tale::Action action("action", tick, default_reasons);
    tale::Emotion emotion("emotion", tick, default_reasons, 1);
    tale::Goal goal("goal", tick, default_reasons);
    tale::Relationship relationship("relationship", tick, default_reasons, 1);
    tale::Resource resource("resource", tick, default_reasons, 1);
    tale::Trait trait("trait", tick, default_reasons);

    EXPECT_EQ(0, action.number_);
    EXPECT_EQ(1, emotion.number_);
    EXPECT_EQ(2, goal.number_);
    EXPECT_EQ(3, relationship.number_);
    EXPECT_EQ(4, resource.number_);
    EXPECT_EQ(5, trait.number_);
}

TEST(TaleTests, CreateRandomInteractionFromStore)
{
    tale::InteractionStore interaction_store;
    std::string interaction_name = interaction_store.GetRandomInteractionName();
    size_t tick = 0;
    std::vector<std::weak_ptr<tale::Kernel>> default_reasons;
    tale::Random random;
    tale::Setting setting;
    tale::School school(random, setting, interaction_store);
    size_t participant_count = interaction_store.GetParticipantCount(interaction_name);
    std::vector<std::weak_ptr<tale::Actor>> participants;
    std::vector<std::shared_ptr<tale::Actor>> actors;
    for (size_t i = 0; i < participant_count; ++i)
    {
        std::shared_ptr<tale::Actor> actor = std::shared_ptr<tale::Actor>(new tale::Actor(random, setting, school, interaction_store, i));
        actors.push_back(actor);
        participants.push_back(actor);
    }
    std::shared_ptr<tale::Interaction> interaction = interaction_store.CreateInteraction(interaction_name, tick, default_reasons, participants);
    EXPECT_EQ(interaction->name_, interaction_name);
    EXPECT_EQ(interaction->tick_, tick);
    EXPECT_EQ(interaction->participant_count_, participant_count);
    for (size_t i = 0; i < participant_count; ++i)
    {
        EXPECT_EQ(interaction->participants_[i].lock(), actors[i]);
    }
    for (size_t i = 0; i < interaction->resource_effects_.size(); ++i)
    {
        EXPECT_EQ(interaction->resource_effects_[i], interaction_store.GetResourceEffects(interaction_name)[i]);
    }
    for (size_t i = 0; i < interaction->emotion_effects_.size(); ++i)
    {
        for (auto &[emotion_type, value] : interaction->emotion_effects_[i])
        {
            EXPECT_EQ(value, interaction_store.GetEmotionEffects(interaction_name)[i].at(emotion_type));
        }
    }
    for (size_t i = 0; i < interaction->relationship_effects_.size(); ++i)
    {
        for (auto &[participant, map] : interaction->relationship_effects_[i])
        {
            for (auto &[relationship_type, value] : map)
            {
                EXPECT_EQ(value, interaction_store.GetRelationshipEffects(interaction_name)[i].at(participant).at(relationship_type));
            }
        }
    }
}
// TODO: Add more Kernel Tests: correct storing of other Kernels, special tests for each type etc.