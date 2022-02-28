#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include "tale/tale.hpp"
#include <time.h>

#define GTEST_INFO std::cout << "[   INFO   ] "
TEST(TaleKernels, IncreasingKernelId)
{
    tale::Random random;
    tale::Chronicle chronicle(random);
    std::vector<std::weak_ptr<tale::Kernel>> no_reasons;
    size_t tick = 0;
    tale::Setting setting;
    setting.actor_count = 0;
    setting.days_to_simulate = 0;
    tale::School school(setting);
    std::shared_ptr<tale::Actor> actor(new tale::Actor(school, 0, "John Doe"));
    std::weak_ptr<tale::Emotion> emotion = chronicle.CreateEmotion(tale::EmotionType::kHappy, tick, actor, no_reasons, 1);
    std::weak_ptr<tale::Goal> goal = chronicle.CreateGoal("goal", tick, no_reasons);
    std::weak_ptr<tale::Relationship> relationship = chronicle.CreateRelationship(tale::RelationshipType::kLove, tick, actor, no_reasons, 1);
    std::weak_ptr<tale::Resource> wealth = chronicle.CreateResource("wealth", tick, actor, no_reasons, 1);
    std::weak_ptr<tale::Trait> trait = chronicle.CreateTrait("trait", tick, no_reasons);

    EXPECT_EQ(0, emotion.lock()->id_);
    EXPECT_EQ(1, goal.lock()->id_);
    EXPECT_EQ(2, relationship.lock()->id_);
    EXPECT_EQ(3, wealth.lock()->id_);
    EXPECT_EQ(4, trait.lock()->id_);
}

class TaleCreateAndRunSchool : public ::testing::Test
{
protected:
    TaleCreateAndRunSchool() {}
    virtual ~TaleCreateAndRunSchool() {}
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

TEST_F(TaleCreateAndRunSchool, CreateAndRunDefaultSchool)
{
    tale::Setting setting;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithZeroActors)
{
    tale::Setting setting;
    setting.actor_count = 0;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithZeroActorsPerCourse)
{
    tale::Setting setting;
    setting.actors_per_course = 0;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithZeroCoursesPerDay)
{
    tale::Setting setting;
    setting.courses_per_day = 0;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithZeroSameCoursesPerWeek)
{
    tale::Setting setting;
    setting.same_course_per_week = 0;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithZeroInAllSettings)
{
    tale::Setting setting;
    setting.actor_count = 0;
    setting.actors_per_course = 0;
    setting.courses_per_day = 0;
    setting.same_course_per_week = 0;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithOneActor)
{
    tale::Setting setting;
    setting.actor_count = 1;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithOneActorPerCourse)
{
    tale::Setting setting;
    setting.actors_per_course = 1;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithOneCoursePerDay)
{
    tale::Setting setting;
    setting.courses_per_day = 1;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithOneSameCoursePerWeek)
{
    tale::Setting setting;
    setting.same_course_per_week = 1;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithOneInAllSettings)
{
    tale::Setting setting;
    setting.actor_count = 1;
    setting.actors_per_course = 1;
    setting.courses_per_day = 1;
    setting.same_course_per_week = 1;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithRandomValuesInAllSettings)
{
    uint32_t seconds = static_cast<uint32_t>(time(NULL));
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

TEST(TaleExtraSchoolTests, CorrectCurrentDayAfterSimulation)
{
    tale::Setting setting;
    setting.actor_count = 10;
    setting.days_to_simulate = 5;
    tale::School school(setting);
    school.SimulateDays(setting.days_to_simulate);
    EXPECT_EQ(school.GetCurrentDay(), 5);
    EXPECT_EQ(school.GetCurrentWeekday(), tale::Weekday::Saturday);
}

TEST(TaleInteractions, CreateRandomInteractionFromStore)
{
    tale::Random random;
    tale::Chronicle chronicle(random);
    tale::InteractionStore interaction_store(random, chronicle);
    size_t interaction_index = interaction_store.GetRandomInteractionPrototypeIndex();
    size_t tick = 0;
    std::vector<std::weak_ptr<tale::Kernel>> default_reasons;
    size_t participant_count = interaction_store.GetParticipantCount(interaction_index);
    tale::Setting setting;
    setting.actor_count = participant_count;
    tale::School school(setting);
    std::vector<std::weak_ptr<tale::Actor>> participants;
    for (size_t i = 0; i < participant_count; ++i)
    {
        participants.push_back(school.GetActor(i));
    }
    std::shared_ptr<tale::Interaction> interaction = interaction_store.CreateInteraction(interaction_index, tick, default_reasons, participants).lock();
    // EXPECT_EQ(interaction->name_, interaction_store.GetInteractionName(interaction_index));
    EXPECT_EQ(interaction->tick_, tick);
    EXPECT_EQ(interaction->GetParticipants().size(), participant_count);
    for (size_t i = 0; i < participant_count; ++i)
    {
        EXPECT_EQ(interaction->GetParticipants()[i].lock(), school.GetActor(i).lock());
    }
    for (size_t i = 0; i < interaction->GetPrototype().wealth_effects.size(); ++i)
    {
        EXPECT_EQ(interaction->GetPrototype().wealth_effects[i], interaction_store.GetWealthEffects(interaction_index)[i]);
    }
    for (size_t i = 0; i < interaction->GetPrototype().emotion_effects.size(); ++i)
    {
        for (auto &[emotion_type, value] : interaction->GetPrototype().emotion_effects[i])
        {
            EXPECT_EQ(value, interaction_store.GetEmotionEffects(interaction_index)[i].at(emotion_type));
        }
    }
    for (size_t i = 0; i < interaction->GetPrototype().relationship_effects.size(); ++i)
    {
        for (auto &[participant, map] : interaction->GetPrototype().relationship_effects[i])
        {
            for (auto &[relationship_type, value] : map)
            {
                EXPECT_EQ(value, interaction_store.GetRelationshipEffects(interaction_index)[i].at(participant).at(relationship_type));
            }
        }
    }
}

TEST(TaleInteractions, ApplyInteraction)
{
    size_t tick = 0;
    std::vector<std::weak_ptr<tale::Kernel>> no_reasons;
    tale::Random random;
    tale::Chronicle chronicle(random);
    tale::Setting setting;
    size_t participant_count = 2;
    setting.actor_count = participant_count;
    tale::School school(setting);
    std::vector<std::weak_ptr<tale::Actor>> participants;
    participants.push_back(school.GetActor(0));
    participants.push_back(school.GetActor(1));
    std::vector<float> wealth_effects;
    std::vector<std::map<tale::EmotionType, float>> emotion_effects;
    std::vector<std::map<size_t, std::map<tale::RelationshipType, float>>> relationship_effects;
    std::vector<float> expected_wealth_values;
    std::vector<std::map<tale::EmotionType, float>> expected_emotion_values;
    std::vector<std::map<size_t, std::map<tale::RelationshipType, float>>> expected_relationship_values;
    std::vector<float> signs = {1.0f, -1.0f};
    for (size_t participant_index = 0; participant_index < 2; ++participant_index)
    {
        float sign = signs[participant_index];
        wealth_effects.push_back(0.5f * sign);
        expected_wealth_values.push_back(0.5f * sign + school.GetActor(participant_index).lock()->wealth_.lock()->GetValue());
        std::map<tale::EmotionType, float> emotion_map;
        std::map<tale::EmotionType, float> expected_emotion_values_map;
        for (int emotion_type_int = (int)tale::EmotionType::kNone + 1; emotion_type_int != (int)tale::EmotionType::kLast; ++emotion_type_int)
        {
            tale::EmotionType type = static_cast<tale::EmotionType>(emotion_type_int);
            emotion_map.insert({type, emotion_type_int * 0.1f * sign});
            expected_emotion_values_map.insert({type, emotion_type_int * 0.1f * sign + school.GetActor(participant_index).lock()->emotions_[type].lock()->GetValue()});
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

            float existing_value = 0;
            std::shared_ptr<tale::Actor> actor = school.GetActor(participant_index).lock();
            if (actor->relationships_.count(other_participant))
            {
                std::map<tale::RelationshipType, std::weak_ptr<tale::Relationship>> existing_map = actor->relationships_.at(other_participant);
                if (existing_map.count(type))
                {
                    existing_value = existing_map.at(type).lock()->GetValue();
                }
            }

            expected_relationship_values_map.insert({type, existing_value + relationship_type_int * 0.1f * sign});
        }
        std::map<size_t, std::map<tale::RelationshipType, float>> participant_relationship_map = {{other_participant, relationship_map}};
        std::map<size_t, std::map<tale::RelationshipType, float>> expected_participant_relationship_map = {{other_participant, expected_relationship_values_map}};
        relationship_effects.push_back(participant_relationship_map);
        expected_relationship_values.push_back(expected_participant_relationship_map);
    }
    tale::InteractionPrototype prototype;
    prototype.name = "Test";
    prototype.wealth_effects = wealth_effects;
    prototype.emotion_effects = emotion_effects;
    prototype.relationship_effects = relationship_effects;

    std::shared_ptr<tale::Interaction> interaction = chronicle.CreateInteraction(prototype, tick, no_reasons, participants).lock();
    interaction->Apply();

    for (size_t participant_index = 0; participant_index < participant_count; ++participant_index)
    {
        EXPECT_EQ(school.GetActor(participant_index).lock()->wealth_.lock()->GetValue(), std::clamp(expected_wealth_values[participant_index], -1.0f, 1.0f));
        for (auto &[type, value] : expected_emotion_values[participant_index])
        {
            EXPECT_EQ(school.GetActor(participant_index).lock()->emotions_[type].lock()->GetValue(), std::clamp(value, -1.0f, 1.0f));
        }
        for (auto &[other_participant_index, map] : expected_relationship_values[participant_index])
        {
            for (auto &[type, value] : map)
            {
                EXPECT_EQ(school.GetActor(participant_index).lock()->relationships_[other_participant_index][type].lock()->GetValue(), std::clamp(value, -1.0f, 1.0f));
            }
        }
    }
}

TEST(TaleInteractions, InteractionBecomesReason)
{
    size_t tick = 0;
    std::vector<std::weak_ptr<tale::Kernel>> no_reasons;
    tale::Random random;
    tale::Chronicle chronicle(random);
    tale::Setting setting;
    size_t participant_count = 2;
    setting.actor_count = participant_count;
    tale::School school(setting);
    std::vector<std::weak_ptr<tale::Actor>> participants;
    participants.push_back(school.GetActor(0));
    participants.push_back(school.GetActor(1));
    std::vector<float> wealth_effects;
    std::vector<std::map<tale::EmotionType, float>> emotion_effects;
    std::vector<std::map<size_t, std::map<tale::RelationshipType, float>>> relationship_effects;
    for (size_t participant_index = 0; participant_index < 2; ++participant_index)
    {
        wealth_effects.push_back(0.1f);
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

    tale::InteractionPrototype prototype;
    prototype.name = "InteractionBecomesReason";
    prototype.wealth_effects = wealth_effects;
    prototype.emotion_effects = emotion_effects;
    prototype.relationship_effects = relationship_effects;
    std::shared_ptr<tale::Interaction> interaction = chronicle.CreateInteraction(prototype, tick, no_reasons, participants).lock();
    interaction->Apply();
    for (size_t participant_index = 0; participant_index < participant_count; ++participant_index)
    {
        EXPECT_EQ(school.GetActor(participant_index).lock()->wealth_.lock()->reasons_[0].lock()->name_, prototype.name);
        EXPECT_EQ(school.GetActor(participant_index).lock()->wealth_.lock()->reasons_[0].lock()->id_, interaction->id_);
        for (auto &[type, emotion] : school.GetActor(participant_index).lock()->emotions_)
        {
            EXPECT_EQ(emotion.lock()->reasons_[0].lock()->name_, prototype.name);
            EXPECT_EQ(emotion.lock()->reasons_[0].lock()->id_, interaction->id_);
        }
        for (auto &[other_participant, map] : school.GetActor(participant_index).lock()->relationships_)
        {
            // TODO: this will break if actors get random relationships
            for (auto &[type, relationship] : map)
            {
                EXPECT_EQ(relationship.lock()->reasons_[0].lock()->name_, prototype.name);
                EXPECT_EQ(relationship.lock()->reasons_[0].lock()->id_, interaction->id_);
            }
        }
    }
}

TEST(TaleCourse, CreateCourse)
{
    tale::Setting setting;
    tale::Random random;
    tale::Course course(random, setting, 0, "Test");
    EXPECT_EQ(course.GetSlotCount(), setting.slot_count_per_week());
}

TEST(TaleCourse, AddGroupsToSlot)
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
        std::shared_ptr<tale::Actor> actor(new tale::Actor(school, slot, "John Doe"));
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

TEST(TaleCourse, AreAllSlotsFilled)
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
        std::shared_ptr<tale::Actor> actor(new tale::Actor(school, slot, "John Doe"));
        actors.push_back(actor);
        course_group.push_back(actor);
        course.AddToSlot(course_group, slot);
        EXPECT_FALSE(course.AllSlotsFilled());
    }
    std::vector<std::weak_ptr<tale::Actor>> course_group;
    std::shared_ptr<tale::Actor> actor(new tale::Actor(school, course.GetSlotCount() - 1, "John Doe"));
    actors.push_back(actor);
    course_group.push_back(actor);
    course.AddToSlot(course_group, course.GetSlotCount() - 1);
    EXPECT_TRUE(course.AllSlotsFilled());
}

TEST(TaleCourse, GetRandomCourseSlot)
{
    tale::Setting setting;
    uint32_t seconds = static_cast<uint32_t>(time(NULL));
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
        uint32_t random_slot = random.GetUInt(0, static_cast<uint32_t>(setting.slot_count_per_week() - 1));
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
        std::shared_ptr<tale::Actor> actor(new tale::Actor(school, random_filled_slots[i], "John Doe"));
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

class TaleActor : public ::testing::Test
{
protected:
    std::string actor_name_ = "John Doe";
    uint32_t desired_min_start_relationships_count_ = 3;
    uint32_t desired_max_start_relationships_count_ = 5;
    size_t actor_id_ = 9;
    std::shared_ptr<tale::Actor> actor_;
    tale::Setting setting_;
    std::shared_ptr<tale::School> school_;
    TaleActor()
    {
        setting_.actor_count = 10;
        setting_.desired_min_start_relationships_count = desired_min_start_relationships_count_;
        setting_.desired_max_start_relationships_count = desired_max_start_relationships_count_;
        school_ = std::shared_ptr<tale::School>(new tale::School(setting_));
        actor_ = std::shared_ptr<tale::Actor>(new tale::Actor(*school_, actor_id_, actor_name_));
        actor_->SetupRandomValues(0);
    }
    virtual ~TaleActor() {}
    void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(TaleActor, CreateActor)
{
    EXPECT_EQ(actor_->name_, actor_name_);
    EXPECT_EQ(actor_->id_, actor_id_);
}

TEST_F(TaleActor, ActorHasInitializedStartingValues)
{
    EXPECT_TRUE(actor_->wealth_.lock());
    EXPECT_NE(actor_->emotions_.size(), 0);
    GTEST_INFO << "Relationship Size: " << actor_->relationships_.size() << "\n";
    EXPECT_GE(actor_->relationships_.size(), desired_min_start_relationships_count_);
    EXPECT_LE(actor_->relationships_.size(), desired_max_start_relationships_count_);
}

TEST_F(TaleActor, AddActorToCourse)
{
    size_t course_id = 5;
    tale::Course course(school_->GetRandom(), setting_, course_id, "Test");
    std::vector<std::weak_ptr<tale::Actor>> course_group;
    std::vector<uint32_t> slots_to_check;
    course_group.push_back(actor_);
    EXPECT_FALSE(actor_->IsEnrolledInCourse(course_id));
    EXPECT_EQ(actor_->GetFilledSlotsCount(), 0);
    for (uint32_t i = 0; i < setting_.slot_count_per_week(); ++i)
    {
        slots_to_check.push_back(i);
        EXPECT_TRUE(actor_->SlotsEmpty(slots_to_check));
    }
    for (size_t i = 0; i < setting_.slot_count_per_week(); ++i)
    {
        EXPECT_FALSE(actor_->AllSlotsFilled());
        course.AddToSlot(course_group, i);
        EXPECT_EQ(actor_->GetFilledSlotsCount(), i + 1);
        EXPECT_TRUE(actor_->IsEnrolledInCourse(course_id));
        EXPECT_FALSE(actor_->SlotsEmpty(slots_to_check));
    }
    EXPECT_TRUE(actor_->AllSlotsFilled());
}

TEST_F(TaleActor, DefaultTendencyChanceCalculation)
{
    tale::Tendency tendency;
    tale::ContextType context = tale::ContextType::kNone;
    float group_size_ratio = 0.0f;
    std::weak_ptr<tale::Kernel> reason;
    float chance = actor_->CalculateTendencyChance(tendency, context, group_size_ratio, reason);
    EXPECT_FLOAT_EQ(chance, 0.5f);
}

TEST_F(TaleActor, MaxTendencyChanceCalculation)
{
    tale::Tendency tendency;
    tendency.group_size = 1.0f;
    tendency.contexts[tale::ContextType::kCourse] = 1.0f;
    tendency.contexts[tale::ContextType::kFreetime] = -1.0f;
    tendency.wealth = 1.0f;
    for (auto &[type, value] : tendency.emotions)
    {
        tendency.emotions[type] = 1.0f;
    }
    std::vector<std::weak_ptr<tale::Kernel>> no_reasons;
    tale::Random random;
    tale::Chronicle chronicle(random);
    actor_->wealth_ = chronicle.CreateResource("wealth", 0, actor_, no_reasons, 1.0f);
    for (auto &[type, value] : tendency.emotions)
    {
        actor_->emotions_[type] = chronicle.CreateEmotion(type, 0, actor_, no_reasons, 1.0f);
    }
    tale::ContextType context = tale::ContextType::kCourse;
    float group_size_ratio = 1.0f;
    std::weak_ptr<tale::Kernel> reason;
    float chance = actor_->CalculateTendencyChance(tendency, context, group_size_ratio, reason);
    EXPECT_FLOAT_EQ(chance, 1.0f);
}

TEST_F(TaleActor, RandomTendencyChanceCalculation)
{
    uint32_t seconds = static_cast<uint32_t>(time(NULL));
    tale::Random random(seconds);
    tale::Tendency tendency;
    uint32_t tries = 1000;
    for (uint32_t i = 0; i < tries; ++i)
    {
        tendency.group_size = random.GetFloat(-1.0f, 1.0f);
        tendency.contexts[tale::ContextType::kCourse] = random.GetFloat(-1.0f, 1.0f);
        tendency.contexts[tale::ContextType::kFreetime] = random.GetFloat(-1.0f, 1.0f);
        tendency.wealth = random.GetFloat(-1.0f, 1.0f);
        for (auto &[type, value] : tendency.emotions)
        {
            tendency.emotions[type] = random.GetFloat(-1.0f, 1.0f);
        }
        std::vector<std::weak_ptr<tale::Kernel>> no_reasons;
        tale::Chronicle chronicle(random);
        actor_->wealth_ = chronicle.CreateResource("wealth", 0, actor_, no_reasons, random.GetFloat(-1.0f, 1.0f));
        for (auto &[type, value] : tendency.emotions)
        {
            actor_->emotions_[type] = chronicle.CreateEmotion(type, 0, actor_, no_reasons, random.GetFloat(-1.0f, 1.0f));
        }
        tale::ContextType context = (random.GetFloat(-1.0f, 1.0f) <= 0 ? tale::ContextType::kCourse : tale::ContextType::kFreetime);
        float group_size_ratio = random.GetFloat(-1.0f, 1.0f);
        std::weak_ptr<tale::Kernel> reason;
        float chance = actor_->CalculateTendencyChance(tendency, context, group_size_ratio, reason);
        EXPECT_GE(chance, 0.0f);
        EXPECT_LE(chance, 1.0f);
    }
}
TEST(TaleRandom, PickIndexWithHundredPercentChance)
{
    uint32_t seconds = static_cast<uint32_t>(time(NULL));
    tale::Random random(seconds);
    uint32_t tries = 1000;
    for (uint32_t i = 0; i < tries; ++i)
    {
        uint32_t random_index = random.GetUInt(0, tries - 1);
        std::vector<float> distribution;
        distribution.reserve(tries);
        for (size_t j = 0; j < tries; ++j)
        {
            distribution.push_back((j == random_index ? 1.0f : 0.0f));
        }
        EXPECT_EQ(random.PickIndex(distribution), random_index);
    }
}
TEST(TaleRandom, PickBetweenFullRangeWithHundredPercentChance)
{
    uint32_t seconds = static_cast<uint32_t>(time(NULL));
    tale::Random random(seconds);
    uint32_t tries = 1000;
    for (uint32_t i = 0; i < tries; ++i)
    {
        uint32_t random_index_bottom = random.GetUInt(0, (tries - 1) / 2);
        uint32_t random_index_top = random.GetUInt(random_index_bottom, tries - 1);
        std::vector<float> distribution(tries, 1.0f);
        EXPECT_GE(random.PickIndex(distribution), 0);
        EXPECT_LT(random.PickIndex(distribution), tries);
    }
}
TEST(TaleRandom, PickBetweenFullRangeWithZeroPercentChance)
{
    uint32_t seconds = static_cast<uint32_t>(time(NULL));
    tale::Random random(seconds);
    uint32_t tries = 1000;
    for (uint32_t i = 0; i < tries; ++i)
    {
        uint32_t random_index_bottom = random.GetUInt(0, (tries - 1) / 2);
        uint32_t random_index_top = random.GetUInt(random_index_bottom, tries - 1);
        std::vector<float> distribution(tries, 0.0f);
        EXPECT_GE(random.PickIndex(distribution, true), 0);
        EXPECT_LT(random.PickIndex(distribution, true), tries);
    }
}
TEST(TaleRandom, PickBetweenRandomRange)
{
    uint32_t seconds = static_cast<uint32_t>(time(NULL));
    tale::Random random(seconds);
    uint32_t tries = 1000;
    for (uint32_t i = 0; i < tries; ++i)
    {
        uint32_t random_index_bottom = random.GetUInt(0, (tries - 1) / 2);
        uint32_t random_index_top = random.GetUInt(random_index_bottom, tries - 1);
        std::vector<float> distribution;
        distribution.reserve(tries);
        for (uint32_t j = 0; j < tries; ++j)
        {
            distribution.push_back(((j >= random_index_bottom && j <= random_index_top) ? 1.0f : 0.0f));
        }
        EXPECT_GE(random.PickIndex(distribution, true), random_index_bottom);
        EXPECT_LE(random.PickIndex(distribution, true), random_index_top);
    }
}