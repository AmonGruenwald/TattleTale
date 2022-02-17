#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include "tale/tale.hpp"
#include <time.h>

#define GTEST_INFO std::cerr << "[   INFO   ] "
TEST(Tale_Kernels, IncreasingKernelNumber)
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

class Tale_School : public ::testing::Test
{
protected:
    Tale_School() {}
    virtual ~Tale_School() {}
    void SetUp(const tale::Setting &setting)
    {
        tale::School school(setting);
        for (size_t i = 0; i < setting.actor_count; ++i)
        {
            EXPECT_EQ(school.GetActor(i).lock()->id_, i);
        }
        for (size_t i = 0; i < setting.course_count(); ++i)
        {
            EXPECT_EQ(school.GetCourse(i).id_, i);
        }
        school.SimulateDays(setting.days_to_simulate);
    }
    virtual void TearDown() {}
};

TEST_F(Tale_School, CreateAndRunDefaultSchool)
{
    tale::Setting setting;
    SetUp(setting);
}

TEST_F(Tale_School, CreateAndRunSchoolWithZeroActors)
{
    tale::Setting setting;
    setting.actor_count = 0;
    SetUp(setting);
}

TEST_F(Tale_School, CreateAndRunSchoolWithZeroActorsPerCourse)
{
    tale::Setting setting;
    setting.actors_per_course = 0;
    SetUp(setting);
}

TEST_F(Tale_School, CreateAndRunSchoolWithZeroCoursesPerDay)
{
    tale::Setting setting;
    setting.courses_per_day = 0;
    SetUp(setting);
}

TEST_F(Tale_School, CreateAndRunSchoolWithZeroSameCoursesPerWeek)
{
    tale::Setting setting;
    setting.same_course_per_week = 0;
    SetUp(setting);
}

TEST_F(Tale_School, CreateAndRunSchoolWithZeroInAllSettings)
{
    tale::Setting setting;
    setting.actor_count = 0;
    setting.actors_per_course = 0;
    setting.courses_per_day = 0;
    setting.same_course_per_week = 0;
    SetUp(setting);
}

TEST_F(Tale_School, CreateAndRunSchoolWithOneActor)
{
    tale::Setting setting;
    setting.actor_count = 1;
    SetUp(setting);
}

TEST_F(Tale_School, CreateAndRunSchoolWithOneActorPerCourse)
{
    tale::Setting setting;
    setting.actors_per_course = 1;
    SetUp(setting);
}

TEST_F(Tale_School, CreateAndRunSchoolWithOneCoursePerDay)
{
    tale::Setting setting;
    setting.courses_per_day = 1;
    SetUp(setting);
}

TEST_F(Tale_School, CreateAndRunSchoolWithOneSameCoursePerWeek)
{
    tale::Setting setting;
    setting.same_course_per_week = 1;
    SetUp(setting);
}

TEST_F(Tale_School, CreateAndRunSchoolWithOneInAllSettings)
{
    tale::Setting setting;
    setting.actor_count = 1;
    setting.actors_per_course = 1;
    setting.courses_per_day = 1;
    setting.same_course_per_week = 1;
    SetUp(setting);
}

TEST_F(Tale_School, CreateAndRunSchoolWithRandomValuesInAllSettings)
{
    time_t seconds = time(NULL);
    tale::Random random(seconds);
    tale::Setting setting;
    setting.actor_count = random.GetUInt(0, 1000);
    setting.actors_per_course = random.GetUInt(0, 50);
    setting.courses_per_day = random.GetUInt(0, 10);
    setting.same_course_per_week = random.GetUInt(0, 10);
    setting.days_to_simulate = random.GetUInt(0, 50);
    GTEST_INFO << "Actor Count = " << setting.actor_count << std::endl;
    GTEST_INFO << "Actors per Course = " << setting.actors_per_course << std::endl;
    GTEST_INFO << "Courses per Day = " << setting.courses_per_day << std::endl;
    GTEST_INFO << "Same Course per Week = " << setting.same_course_per_week << std::endl;
    GTEST_INFO << "Days to Simulate = " << setting.days_to_simulate << std::endl;
    SetUp(setting);
}

TEST(Tale_ExtraSchoolTests, CorrectCurrentDay)
{
    tale::Setting setting;
    setting.actor_count = 10;
    setting.days_to_simulate = 5;
    tale::School school(setting);
    school.SimulateDays(setting.days_to_simulate);
    EXPECT_EQ(school.GetCurrentDay(), 5);
    EXPECT_EQ(school.GetCurrentWeekday(), tale::Weekday::Saturday);
}

TEST(Tale_Interactions, CreateRandomInteractionFromStore)
{
    tale::InteractionStore interaction_store;
    std::string interaction_name = interaction_store.GetRandomInteractionName();
    size_t tick = 0;
    std::vector<std::weak_ptr<tale::Kernel>> default_reasons;
    tale::Setting setting;
    size_t participant_count = interaction_store.GetParticipantCount(interaction_name);
    setting.actor_count = participant_count;
    tale::School school(setting);
    std::vector<std::weak_ptr<tale::Actor>> participants;
    for (size_t i = 0; i < participant_count; ++i)
    {
        participants.push_back(school.GetActor(i));
    }
    std::shared_ptr<tale::Interaction> interaction = interaction_store.CreateInteraction(interaction_name, tick, default_reasons, participants);
    EXPECT_EQ(interaction->name_, interaction_name);
    EXPECT_EQ(interaction->tick_, tick);
    EXPECT_EQ(interaction->participant_count_, participant_count);
    for (size_t i = 0; i < participant_count; ++i)
    {
        EXPECT_EQ(interaction->participants_[i].lock(), school.GetActor(i).lock());
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

TEST(Tale_Interactions, ApplyInteraction)
{
    size_t tick = 0;
    std::vector<std::weak_ptr<tale::Kernel>> default_reasons;
    tale::Setting setting;
    size_t participant_count = 2;
    setting.actor_count = participant_count;
    tale::School school(setting);
    std::vector<std::weak_ptr<tale::Actor>> participants;
    participants.push_back(school.GetActor(0));
    participants.push_back(school.GetActor(1));
    std::vector<float> resource_effects;
    std::vector<std::map<tale::EmotionType, float>> emotion_effects;
    std::vector<std::map<size_t, std::map<tale::RelationshipType, float>>> relationship_effects;
    std::vector<float> expected_resource_values;
    std::vector<std::map<tale::EmotionType, float>> expected_emotion_values;
    std::vector<std::map<size_t, std::map<tale::RelationshipType, float>>> expected_relationship_values;
    std::vector<float> signs = {1.0f, -1.0f};
    for (size_t participant_index = 0; participant_index < 2; ++participant_index)
    {
        float sign = signs[participant_index];
        resource_effects.push_back(0.5f * sign);
        expected_resource_values.push_back(0.5f * sign + school.GetActor(participant_index).lock()->resource_->GetValue());
        std::map<tale::EmotionType, float> emotion_map;
        std::map<tale::EmotionType, float> expected_emotion_values_map;
        for (int emotion_type_int = (int)tale::EmotionType::kNone + 1; emotion_type_int != (int)tale::EmotionType::kLast; ++emotion_type_int)
        {
            tale::EmotionType type = static_cast<tale::EmotionType>(emotion_type_int);
            emotion_map.insert({type, emotion_type_int * 0.1f * sign});
            expected_emotion_values_map.insert({type, emotion_type_int * 0.1f * sign + school.GetActor(participant_index).lock()->emotions_[type]->GetValue()});
        }
        emotion_effects.push_back(emotion_map);
        expected_emotion_values.push_back(expected_emotion_values_map);

        std::map<tale::RelationshipType, float> relationship_map;
        std::map<tale::RelationshipType, float> expected_relationship_values_map;
        size_t other_participant = (participant_index == 0 ? 1 : 0);
        for (int relationship_type_int = (int)tale::RelationshipType::kNone + 1; relationship_type_int != (int)tale::RelationshipType::kLast; ++relationship_type_int)
        {
            tale::RelationshipType type = static_cast<tale::RelationshipType>(relationship_type_int);
            relationship_map.insert({type, relationship_type_int * 0.1f * sign});
            // No relationship value at start
            // TODO: (this might change if we have random start relationships)
            expected_relationship_values_map.insert({type, relationship_type_int * 0.1f * sign});
        }
        std::map<size_t, std::map<tale::RelationshipType, float>> participant_relationship_map = {{other_participant, relationship_map}};
        std::map<size_t, std::map<tale::RelationshipType, float>> expected_participant_relationship_map = {{other_participant, expected_relationship_values_map}};
        relationship_effects.push_back(participant_relationship_map);
        expected_relationship_values.push_back(expected_participant_relationship_map);
    }
    std::shared_ptr<tale::Interaction> interaction(new tale::Interaction("Test", tick, default_reasons, participant_count, participants, resource_effects, emotion_effects, relationship_effects));
    interaction->Apply();

    for (size_t participant_index = 0; participant_index < participant_count; ++participant_index)
    {
        EXPECT_EQ(school.GetActor(participant_index).lock()->resource_->GetValue(), expected_resource_values[participant_index]);
        for (auto &[type, value] : expected_emotion_values[participant_index])
        {
            EXPECT_EQ(school.GetActor(participant_index).lock()->emotions_[type]->GetValue(), value);
        }
        for (auto &[other_participant_index, map] : expected_relationship_values[participant_index])
        {
            for (auto &[type, value] : map)
            {
                EXPECT_EQ(school.GetActor(participant_index).lock()->relationships_[other_participant_index][type]->GetValue(), value);
            }
        }
    }
}

TEST(Tale_Interactions, InteractionBecomesReason)
{
    size_t tick = 0;
    std::vector<std::weak_ptr<tale::Kernel>> default_reasons;
    tale::Setting setting;
    size_t participant_count = 2;
    setting.actor_count = participant_count;
    tale::School school(setting);
    std::vector<std::weak_ptr<tale::Actor>> participants;
    participants.push_back(school.GetActor(0));
    participants.push_back(school.GetActor(1));
    std::vector<float> resource_effects;
    std::vector<std::map<tale::EmotionType, float>> emotion_effects;
    std::vector<std::map<size_t, std::map<tale::RelationshipType, float>>> relationship_effects;
    for (size_t participant_index = 0; participant_index < 2; ++participant_index)
    {
        resource_effects.push_back(0.1f);
        std::map<tale::EmotionType, float> emotion_map;
        for (int emotion_type_int = (int)tale::EmotionType::kNone + 1; emotion_type_int != (int)tale::EmotionType::kLast; ++emotion_type_int)
        {
            tale::EmotionType type = static_cast<tale::EmotionType>(emotion_type_int);
            emotion_map.insert({type, 0.1f});
        }
        emotion_effects.push_back(emotion_map);

        std::map<tale::RelationshipType, float> relationship_map;
        size_t other_participant = (participant_index == 0 ? 1 : 0);
        for (int relationship_type_int = (int)tale::RelationshipType::kNone + 1; relationship_type_int != (int)tale::RelationshipType::kLast; ++relationship_type_int)
        {
            tale::RelationshipType type = static_cast<tale::RelationshipType>(relationship_type_int);
            relationship_map.insert({type, 0.1f});
        }
        std::map<size_t, std::map<tale::RelationshipType, float>> participant_relationship_map = {{other_participant, relationship_map}};
        relationship_effects.push_back(participant_relationship_map);
    }
    std::string name = "InteractionBecomesReason";
    std::shared_ptr<tale::Interaction> interaction(new tale::Interaction(name, tick, default_reasons, participant_count, participants, resource_effects, emotion_effects, relationship_effects));
    interaction->Apply();
    for (size_t participant_index = 0; participant_index < participant_count; ++participant_index)
    {
        EXPECT_EQ(school.GetActor(participant_index).lock()->resource_->reasons_[0].lock()->name_, name);
        EXPECT_EQ(school.GetActor(participant_index).lock()->resource_->reasons_[0].lock()->number_, interaction->number_);
        for (auto &[type, emotion] : school.GetActor(participant_index).lock()->emotions_)
        {
            EXPECT_EQ(emotion->reasons_[0].lock()->name_, name);
            EXPECT_EQ(emotion->reasons_[0].lock()->number_, interaction->number_);
        }
        for (auto &[other_participant, map] : school.GetActor(participant_index).lock()->relationships_)
        {
            // TODO: this will break if actors get random relationships
            for (auto &[type, relationship] : map)
            {
                EXPECT_EQ(relationship->reasons_[0].lock()->name_, name);
                EXPECT_EQ(relationship->reasons_[0].lock()->number_, interaction->number_);
            }
        }
    }
}

TEST(Tale_Course, CreateCourse)
{
    tale::Setting setting;
    tale::Random random;
    tale::Course course(random, setting, 0, "Test");
    EXPECT_EQ(course.GetSlotCount(), setting.slot_count_per_week());
}

TEST(Tale_Course, AddGroupsToSlot)
{
    tale::Setting setting;
    tale::Random random;
    tale::Course course(random, setting, 0, "Test");
    setting.actor_count = 0;
    tale::School school(setting);
    std::vector<std::shared_ptr<tale::Actor>> actors;
    for (size_t slot = 0; slot < course.GetSlotCount(); ++slot)
    {
        std::vector<std::weak_ptr<tale::Actor>> course_group;
        std::shared_ptr<tale::Actor> actor(new tale::Actor(school, slot));
        actors.push_back(actor);
        course_group.push_back(actor);
        course.AddToSlot(course_group, slot);
        EXPECT_EQ(course.GetCourseGroupForSlot(slot)[0].lock()->id_, slot);
    }
    for (size_t slot = 0; slot < course.GetSlotCount(); ++slot)
    {
        EXPECT_EQ(course.GetCourseGroupForSlot(slot)[0].lock()->id_, slot);
    }
}

TEST(Tale_Course, AreAllSlotsFilled)
{
    tale::Setting setting;
    tale::Random random;
    tale::Course course(random, setting, 0, "Test");
    setting.actor_count = 0;
    tale::School school(setting);
    std::vector<std::shared_ptr<tale::Actor>> actors;
    EXPECT_FALSE(course.AllSlotsFilled());
    for (size_t slot = 0; slot < course.GetSlotCount() - 1; ++slot)
    {
        std::vector<std::weak_ptr<tale::Actor>> course_group;
        std::shared_ptr<tale::Actor> actor(new tale::Actor(school, slot));
        actors.push_back(actor);
        course_group.push_back(actor);
        course.AddToSlot(course_group, slot);
        EXPECT_FALSE(course.AllSlotsFilled());
    }
    std::vector<std::weak_ptr<tale::Actor>> course_group;
    std::shared_ptr<tale::Actor> actor(new tale::Actor(school, course.GetSlotCount() - 1));
    actors.push_back(actor);
    course_group.push_back(actor);
    course.AddToSlot(course_group, course.GetSlotCount() - 1);
    EXPECT_TRUE(course.AllSlotsFilled());
}

TEST(Tale_Course, GetRandomCourseSlot)
{
    tale::Setting setting;
    time_t seconds = time(NULL);
    tale::Random random(seconds);
    setting.actor_count = 300;
    setting.actors_per_course = 30;
    setting.courses_per_day = 6;
    setting.same_course_per_week = 4;
    tale::Course course(random, setting, 0, "Test");
    setting.actor_count = 0;
    tale::School school(setting);
    std::vector<std::shared_ptr<tale::Actor>> actors;

    std::vector<uint32_t> random_filled_slots;

    for (size_t i = 0; i < setting.slot_count_per_week() / 2; ++i)
    {
        uint32_t random_slot = random.GetUInt(0, setting.slot_count_per_week() - 1);
        while (std::count(random_filled_slots.begin(), random_filled_slots.end(), random_slot))
        {
            ++random_slot;
            random_slot %= setting.slot_count_per_week();
        }
        random_filled_slots.push_back(random_slot);
    }
    for (size_t i = 0; i < random_filled_slots.size(); ++i)
    {

        std::vector<std::weak_ptr<tale::Actor>> course_group;
        std::shared_ptr<tale::Actor> actor(new tale::Actor(school, random_filled_slots[i]));
        actors.push_back(actor);
        course_group.push_back(actor);
        course.AddToSlot(course_group, random_filled_slots[i]);
    }

    size_t tries = 10000;
    for (size_t i = 0; i < tries; ++i)
    {
        uint32_t random_empty_slot = course.GetRandomEmptySlot();
        for (size_t j = 0; j < random_filled_slots.size(); ++j)
        {
            EXPECT_NE(random_empty_slot, random_filled_slots[j]);
        }
    }
}
// TODO: Add more Kernel Tests: correct storing of other Kernels, special tests for each type etc.