#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include "tale/tale.hpp"
#include <time.h>

#define GTEST_INFO std::cout << "[   INFO   ] "

using namespace tattletale;

TEST(TaleKernels, IncreasingKernelId)
{
    Random random;
    Chronicle chronicle(random);
    std::vector<Kernel *> no_reasons;
    size_t tick = 0;
    Setting setting;
    setting.actor_count = 0;
    setting.days_to_simulate = 0;
    School school(chronicle, random, setting);
    chronicle.Reset();
    Actor *actor = chronicle.CreateActor(school, "John", "Doe");
    Emotion *emotion = chronicle.CreateEmotion(EmotionType::kHappy, tick, actor, no_reasons, 1);
    Goal *goal = chronicle.CreateGoal(Goal::GetRandomGoalType(random), tick, actor, no_reasons);
    Relationship *relationship = chronicle.CreateRelationship(RelationshipType::kLove, tick, actor, actor, no_reasons, 1);
    Resource *wealth = chronicle.CreateResource("wealth", "wealthy", "poor", tick, actor, no_reasons, 1);
    Trait *trait = chronicle.CreateTrait("trait", tick, actor, no_reasons);

    EXPECT_EQ(0, emotion->id_);
    EXPECT_EQ(1, goal->id_);
    EXPECT_EQ(2, relationship->id_);
    EXPECT_EQ(3, wealth->id_);
    EXPECT_EQ(4, trait->id_);
}

class TaleCreateAndRunSchool : public ::testing::Test
{
protected:
    TaleCreateAndRunSchool() {}
    virtual ~TaleCreateAndRunSchool() {}
    void SetUp(const Setting &setting)
    {
        Random random;
        Chronicle chronicle(random);
        School school(chronicle, random, setting);
        for (size_t i = 0; i < setting.actor_count; ++i)
        {
            EXPECT_EQ(school.GetActor(i)->id_, i);
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
    Setting setting;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithZeroActors)
{
    Setting setting;
    setting.actor_count = 0;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithZeroActorsPerCourse)
{
    Setting setting;
    setting.actors_per_course = 0;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithZeroCoursesPerDay)
{
    Setting setting;
    setting.courses_per_day = 0;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithZeroSameCoursesPerWeek)
{
    Setting setting;
    setting.same_course_per_week = 0;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithZeroFreetimeActorCount)
{
    Setting setting;
    setting.freetime_actor_count = 0;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithZeroInAllSettings)
{
    Setting setting;
    setting.actor_count = 0;
    setting.actors_per_course = 0;
    setting.courses_per_day = 0;
    setting.same_course_per_week = 0;
    setting.freetime_actor_count = 0;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithOneActor)
{
    Setting setting;
    setting.actor_count = 1;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithOneActorPerCourse)
{
    Setting setting;
    setting.actors_per_course = 1;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithOneCoursePerDay)
{
    Setting setting;
    setting.courses_per_day = 1;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithOneFreetimeActorCount)
{
    Setting setting;
    setting.freetime_actor_count = 1;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithOneSameCoursePerWeek)
{
    Setting setting;
    setting.same_course_per_week = 1;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithOneInAllSettings)
{
    Setting setting;
    setting.actor_count = 1;
    setting.actors_per_course = 1;
    setting.courses_per_day = 1;
    setting.same_course_per_week = 1;
    setting.freetime_actor_count = 1;
    SetUp(setting);
}

TEST_F(TaleCreateAndRunSchool, CreateAndRunSchoolWithRandomValuesInAllSettings)
{
    uint32_t seconds = static_cast<uint32_t>(time(NULL));
    Random random(seconds);
    Setting setting;
    setting.actor_count = random.GetUInt(0, 1000);
    setting.actors_per_course = random.GetUInt(0, 50);
    setting.courses_per_day = random.GetUInt(0, 10);
    setting.same_course_per_week = random.GetUInt(0, 10);
    setting.days_to_simulate = random.GetUInt(0, 50);
    setting.freetime_actor_count = random.GetUInt(0, 30);
    GTEST_INFO << "Actor Count = " << setting.actor_count << std::endl;
    GTEST_INFO << "Actors per Course = " << setting.actors_per_course << std::endl;
    GTEST_INFO << "Courses per Day = " << setting.courses_per_day << std::endl;
    GTEST_INFO << "Same Course per Week = " << setting.same_course_per_week << std::endl;
    GTEST_INFO << "Days to Simulate = " << setting.days_to_simulate << std::endl;
    SetUp(setting);
}

TEST(TaleExtraSchoolTests, CorrectCurrentDayAfterSimulation)
{
    Setting setting;
    setting.actor_count = 10;
    setting.days_to_simulate = 5;
    Random random;
    Chronicle chronicle(random);
    chronicle.Reset();
    School school(chronicle, random, setting);
    school.SimulateDays(setting.days_to_simulate);
    EXPECT_EQ(school.GetCurrentDay(), 5);
    EXPECT_EQ(school.GetCurrentWeekday(), Weekday::Saturday);
}

TEST(TaleExtraSchoolTests, InitializedRelationshipsAtStart)
{
    Setting setting;
    setting.actor_count = 100;
    setting.desired_min_start_relationships_count = 4;
    setting.desired_max_start_relationships_count = 12;
    Random random;
    Chronicle chronicle(random);
    chronicle.Reset();
    School school(chronicle, random, setting);
    for (size_t i = 0; i < setting.actor_count; ++i)
    {
        auto a = school.GetActor(i);
        for (auto &b : a->GetAllKnownActors())
        {
            EXPECT_NE(a->CalculateRelationshipStrength(b->id_), 0);
            EXPECT_NE(b->CalculateRelationshipStrength(a->id_), 0);
        }
    }
}

TEST(TaleInteractions, CreateRandomInteractionFromStore)
{
    Random random;
    Setting setting;
    Chronicle chronicle(random);
    InteractionStore interaction_store(random);
    size_t interaction_index = interaction_store.GetRandomInteractionPrototypeIndex();
    size_t participant_count = interaction_store.GetParticipantCount(interaction_index);
    chronicle.Reset();
    setting.actor_count = participant_count;
    School school(chronicle, random, setting);
    std::vector<Actor *> participants;
    for (size_t i = 0; i < participant_count; ++i)
    {
        participants.push_back(school.GetActor(i));
    }
    size_t tick = 0;
    std::vector<Kernel *> no_reasons;
    Interaction *interaction = interaction_store.CreateInteraction(chronicle, interaction_index, 1.0f, tick, no_reasons, participants);
    EXPECT_EQ(interaction->name_, interaction_store.GetInteractionName(interaction_index));
    EXPECT_EQ(interaction->tick_, tick);
    EXPECT_EQ(interaction->GetParticipants().size(), participant_count);
    for (size_t i = 0; i < participant_count; ++i)
    {
        EXPECT_EQ(interaction->GetParticipants()[i], school.GetActor(i));
    }
    for (size_t i = 0; i < interaction->GetPrototype()->wealth_effects.size(); ++i)
    {
        EXPECT_EQ(interaction->GetPrototype()->wealth_effects[i], interaction_store.GetWealthEffects(interaction_index)[i]);
    }
    for (size_t i = 0; i < interaction->GetPrototype()->emotion_effects.size(); ++i)
    {
        for (size_t j = 0; j < interaction->GetPrototype()->emotion_effects[i].size(); ++j)
        {
            EXPECT_EQ(interaction->GetPrototype()->emotion_effects[i][j], interaction_store.GetEmotionEffects(interaction_index)[i][j]);
        }
    }
    for (size_t i = 0; i < interaction->GetPrototype()->relationship_effects.size(); ++i)
    {
        for (auto &[participant, vector] : interaction->GetPrototype()->relationship_effects[i])
        {
            for (size_t j = 0; j < vector.size(); ++j)
            {
                EXPECT_EQ(vector[j], interaction_store.GetRelationshipEffects(interaction_index)[i].at(participant)[j]);
            }
        }
    }
}

TEST(TaleInteractions, ApplyInteraction)
{
    size_t tick = 0;
    std::vector<Kernel *> no_reasons;
    Random random;
    size_t participant_count = 2;
    Setting setting;
    setting.actor_count = participant_count;
    Chronicle chronicle(random);
    School school(chronicle, random, setting);
    std::vector<Actor *> participants;
    participants.push_back(school.GetActor(0));
    participants.push_back(school.GetActor(1));
    std::vector<float> wealth_effects;
    std::vector<std::vector<float>> emotion_effects;
    std::vector<robin_hood::unordered_map<size_t, std::vector<float>>> relationship_effects;
    std::vector<float> expected_wealth_values;
    std::vector<std::vector<float>> expected_emotion_values;
    std::vector<robin_hood::unordered_map<size_t, std::vector<float>>> expected_relationship_values;
    std::vector<float> signs = {1.0f, -1.0f};
    for (size_t participant_index = 0; participant_index < 2; ++participant_index)
    {
        float sign = signs[participant_index];
        wealth_effects.push_back(0.5f * sign);
        expected_wealth_values.push_back(0.5f * sign + school.GetActor(participant_index)->wealth_->GetValue());
        std::vector<float> emotion_vector(static_cast<int>(EmotionType::kLast), 0.0f);
        std::vector<float> expected_emotion_values_vector(static_cast<int>(EmotionType::kLast), 0.0f);
        for (int type_index = 0; type_index < emotion_vector.size(); ++type_index)
        {
            emotion_vector[type_index] = type_index * 0.1f * sign;
            expected_emotion_values_vector[type_index] = type_index * 0.1f * sign + school.GetActor(participant_index)->emotions_[type_index]->GetValue();
        }
        emotion_effects.push_back(emotion_vector);
        expected_emotion_values.push_back(expected_emotion_values_vector);

        std::vector<float> relationship_vector(static_cast<int>(RelationshipType::kLast), 0.0f);
        std::vector<float> expected_relationship_values_vector(static_cast<int>(RelationshipType::kLast), 0.0f);
        size_t other_participant = (participant_index == 0 ? 1 : 0);
        for (int type_index = 0; type_index < relationship_vector.size(); ++type_index)
        {
            relationship_vector[type_index] = type_index * 0.1f * sign;

            float existing_value = 0;
            Actor *actor = school.GetActor(participant_index);
            if (actor->relationships_.count(other_participant))
            {
                std::vector<Relationship *> existing_map = actor->relationships_.at(other_participant);
                existing_value = existing_map[type_index]->GetValue();
            }

            expected_relationship_values_vector[type_index] = existing_value + type_index * 0.1f * sign;
        }
        robin_hood::unordered_map<size_t, std::vector<float>> participant_relationship_map = {{other_participant, relationship_vector}};
        robin_hood::unordered_map<size_t, std::vector<float>> expected_participant_relationship_map = {{other_participant, expected_relationship_values_vector}};
        relationship_effects.push_back(participant_relationship_map);
        expected_relationship_values.push_back(expected_participant_relationship_map);
    }
    std::shared_ptr<InteractionPrototype> prototype(new InteractionPrototype());
    prototype->name = "Test";
    prototype->wealth_effects = wealth_effects;
    prototype->emotion_effects = emotion_effects;
    prototype->relationship_effects = relationship_effects;
    std::shared_ptr<InteractionRequirement> requirement(new InteractionRequirement());
    std::shared_ptr<InteractionTendency> tendency(new InteractionTendency());
    Interaction *interaction = chronicle.CreateInteraction(prototype, requirement, tendency, 1.0f, tick, no_reasons, participants);
    interaction->Apply();

    for (size_t participant_index = 0; participant_index < participant_count; ++participant_index)
    {
        EXPECT_EQ(school.GetActor(participant_index)->wealth_->GetValue(), std::clamp(expected_wealth_values[participant_index], -1.0f, 1.0f));
        for (size_t type_index = 0; type_index < expected_emotion_values[participant_index].size(); ++type_index)
        {
            EXPECT_EQ(school.GetActor(participant_index)->emotions_[type_index]->GetValue(), std::clamp(expected_emotion_values[participant_index][type_index], -1.0f, 1.0f));
        }
        for (auto &[other_participant_index, vector] : expected_relationship_values[participant_index])
        {
            for (size_t type_index = 0; type_index < vector.size(); ++type_index)
            {
                EXPECT_EQ(school.GetActor(participant_index)->relationships_[other_participant_index][type_index]->GetValue(), std::clamp(vector[type_index], -1.0f, 1.0f));
            }
        }
    }
}

TEST(TaleInteractions, InteractionBecomesReason)
{
    size_t tick = 0;
    Random random;
    size_t participant_count = 2;
    Setting setting;
    setting.desired_min_start_relationships_count = 0;
    setting.desired_max_start_relationships_count = 0;
    setting.actor_count = participant_count;
    Chronicle chronicle(random);
    School school(chronicle, random, setting);
    std::vector<Actor *> participants;
    participants.push_back(school.GetActor(0));
    participants.push_back(school.GetActor(1));
    std::vector<float> wealth_effects;
    std::vector<std::vector<float>> emotion_effects;
    std::vector<robin_hood::unordered_map<size_t, std::vector<float>>> relationship_effects;
    for (size_t participant_index = 0; participant_index < participant_count; ++participant_index)
    {
        wealth_effects.push_back(0.1f);
        std::vector<float> emotion_vector(static_cast<int>(EmotionType::kLast), 0.1f);
        emotion_effects.push_back(emotion_vector);

        std::vector<float> relationship_vector(static_cast<int>(RelationshipType::kLast), 0.1f);
        size_t other_participant = (participant_index == 0 ? 1 : 0);
        robin_hood::unordered_map<size_t, std::vector<float>> participant_relationship_map = {{other_participant, relationship_vector}};
        relationship_effects.push_back(participant_relationship_map);
    }
    std::shared_ptr<InteractionPrototype> prototype(new InteractionPrototype());
    prototype->name = "InteractionBecomesReason";
    prototype->wealth_effects = wealth_effects;
    prototype->emotion_effects = emotion_effects;
    prototype->relationship_effects = relationship_effects;
    prototype->description = "{} did test interaction with {}";
    std::shared_ptr<InteractionRequirement> requirement(new InteractionRequirement());
    requirement->participant_count = participant_count;
    std::shared_ptr<InteractionTendency> tendency(new InteractionTendency());

    std::vector<Kernel *> no_reasons;
    Interaction *interaction = chronicle.CreateInteraction(prototype, requirement, tendency, 1.0f, tick, no_reasons, participants);
    interaction->Apply();
    for (size_t participant_index = 0; participant_index < participant_count; ++participant_index)
    {
        EXPECT_EQ(school.GetActor(participant_index)->wealth_->GetReasons()[0]->name_, prototype->name);
        EXPECT_EQ(school.GetActor(participant_index)->wealth_->GetReasons()[0]->id_, interaction->id_);

        for (int type_index = 0; type_index < static_cast<int>(EmotionType::kLast); ++type_index)
        {
            EXPECT_EQ(school.GetActor(participant_index)->emotions_[type_index]->GetReasons()[0]->name_, prototype->name);
            EXPECT_EQ(school.GetActor(participant_index)->emotions_[type_index]->GetReasons()[0]->id_, interaction->id_);
        }
        for (auto &[other_participant, vector] : school.GetActor(participant_index)->relationships_)
        {
            for (int type_index = 0; type_index < static_cast<int>(EmotionType::kLast); ++type_index)
            {
                EXPECT_EQ(vector[type_index]->GetReasons()[0]->name_, prototype->name);
                EXPECT_EQ(vector[type_index]->GetReasons()[0]->id_, interaction->id_);
            }
        }
    }
}

TEST(TaleCourse, CreateCourse)
{
    Setting setting;
    Random random;
    Course course(random, setting, 0, "Test");
    EXPECT_EQ(course.GetSlotCount(), setting.slot_count_per_week());
}

TEST(TaleCourse, AddGroupsToSlot)
{
    Setting setting;
    Random random;
    Course course(random, setting, 0, "Test");
    setting.actor_count = 0;
    Chronicle chronicle(random);
    School school(chronicle, random, setting);
    std::vector<Actor *> actors;
    for (size_t slot = 0; slot < course.GetSlotCount(); ++slot)
    {
        std::list<Actor *> course_group;
        Actor *actor = chronicle.CreateActor(school, "John", "Doe");
        actors.push_back(actor);
        course_group.push_back(actor);
        course.AddToSlot(course_group, slot);
        EXPECT_EQ((*course.GetCourseGroupForSlot(slot).begin())->id_, slot);
    }
    for (size_t slot = 0; slot < course.GetSlotCount(); ++slot)
    {
        EXPECT_EQ((*course.GetCourseGroupForSlot(slot).begin())->id_, slot);
    }
}

TEST(TaleCourse, AreAllSlotsFilled)
{
    Setting setting;
    Random random;
    Course course(random, setting, 0, "Test");
    setting.actor_count = 0;
    Chronicle chronicle(random);
    School school(chronicle, random, setting);
    EXPECT_FALSE(course.AllSlotsFilled());
    for (size_t slot = 0; slot < setting.slot_count_per_week() - 1; ++slot)
    {
        std::list<Actor *> course_group;
        for (size_t actor_index = 0; actor_index < setting.actors_per_course; ++actor_index)
        {
            Actor *actor = chronicle.CreateActor(school, "John", "Doe");
            course_group.push_back(actor);
        }
        course.AddToSlot(course_group, slot);
        EXPECT_FALSE(course.AllSlotsFilled());
    }
    std::list<Actor *> course_group;
    for (size_t actor_index = 0; actor_index < setting.actors_per_course; ++actor_index)
    {
        Actor *actor = chronicle.CreateActor(school, "John", "Doe");
        course_group.push_back(actor);
    }
    course.AddToSlot(course_group, setting.slot_count_per_week() - 1);
    EXPECT_TRUE(course.AllSlotsFilled());
}

TEST(TaleCourse, GetRandomCourseSlot)
{
    Setting setting;
    uint32_t seconds = static_cast<uint32_t>(time(NULL));
    Random random(seconds);
    setting.actor_count = 300;
    setting.actors_per_course = 30;
    setting.courses_per_day = 6;
    setting.same_course_per_week = 4;
    Course course(random, setting, 0, "Test");
    setting.actor_count = 0;
    Chronicle chronicle(random);
    School school(chronicle, random, setting);

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

        std::list<Actor *> course_group;
        Actor *actor = chronicle.CreateActor(school, "John", "Doe");
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
    std::string actor_first_name_ = "John";
    std::string actor_last_name_ = "Doe";
    uint32_t desired_min_start_relationships_count_ = 1;
    uint32_t desired_max_start_relationships_count_ = 8;
    Actor *actor_;
    Setting setting_;
    School *school_;
    Random random_;
    Chronicle chronicle_;
    TaleActor() : random_(), chronicle_(random_)
    {
        setting_.actor_count = 10;
        setting_.desired_min_start_relationships_count = desired_min_start_relationships_count_;
        setting_.desired_max_start_relationships_count = desired_max_start_relationships_count_;
        school_ = new School(chronicle_, random_, setting_);
        actor_ = chronicle_.CreateActor(*school_, actor_first_name_, actor_last_name_);
        actor_->SetupRandomValues(0);
    }
    virtual ~TaleActor() {}
    void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(TaleActor, CreateActor)
{
    EXPECT_EQ(actor_->first_name_, actor_first_name_);
    EXPECT_EQ(actor_->last_name_, actor_last_name_);
    EXPECT_EQ(actor_->id_, setting_.actor_count);
}

TEST_F(TaleActor, ActorHasInitializedStartingValues)
{
    EXPECT_TRUE(actor_->wealth_);
    EXPECT_NE(actor_->emotions_.size(), 0);
    GTEST_INFO << "Relationship Size: " << actor_->relationships_.size() << "\n";
    EXPECT_LE(actor_->relationships_.size(), setting_.max_start_relationships_count());
}

TEST_F(TaleActor, AddActorToCourse)
{
    size_t course_id = 5;
    Course course(school_->GetRandom(), setting_, course_id, "Test");
    std::list<Actor *> course_group;
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
    InteractionTendency tendency;
    ContextType context = ContextType::kLast;
    Kernel *reason;
    float chance = actor_->CalculateTendencyChance(tendency, context, reason);
    EXPECT_FLOAT_EQ(chance, 0.5f);
}

TEST_F(TaleActor, MaxTendencyChanceCalculation)
{
    InteractionTendency tendency;
    tendency.contexts[static_cast<int>(ContextType::kCourse)] = 1.0f;
    tendency.contexts[static_cast<int>(ContextType::kFreetime)] = -1.0f;
    tendency.wealth = 1.0f;
    for (size_t type_index = 0; type_index < tendency.emotions.size(); ++type_index)
    {
        tendency.emotions[type_index] = 1.0f;
    }
    std::vector<Kernel *> no_reasons;
    actor_->wealth_ = chronicle_.CreateResource("wealth", "wealthy", "poor", 0, actor_, no_reasons, 1.0f);
    for (size_t type_index = 0; type_index < tendency.emotions.size(); ++type_index)
    {
        actor_->emotions_[type_index] = chronicle_.CreateEmotion(static_cast<EmotionType>(type_index), 0, actor_, no_reasons, 1.0f);
    }
    ContextType context = ContextType::kCourse;
    Kernel *reason;
    float chance = actor_->CalculateTendencyChance(tendency, context, reason);
    EXPECT_FLOAT_EQ(chance, 1.0f);
}

TEST_F(TaleActor, RandomTendencyChanceCalculation)
{
    uint32_t seconds = static_cast<uint32_t>(time(NULL));
    Random random(seconds);
    InteractionTendency tendency;
    uint32_t tries = 1000;
    for (uint32_t i = 0; i < tries; ++i)
    {
        tendency.contexts[static_cast<int>(ContextType::kCourse)] = random.GetFloat(-1.0f, 1.0f);
        tendency.contexts[static_cast<int>(ContextType::kFreetime)] = random.GetFloat(-1.0f, 1.0f);
        tendency.wealth = random.GetFloat(-1.0f, 1.0f);
        for (size_t type_index = 0; type_index < tendency.emotions.size(); ++type_index)
        {
            tendency.emotions[type_index] = random.GetFloat(-1.0f, 1.0f);
        }
        std::vector<Kernel *> no_reasons;
        actor_->wealth_ = chronicle_.CreateResource("wealth", "wealthy", "poor", 0, actor_, no_reasons, random.GetFloat(-1.0f, 1.0f));
        for (size_t type_index = 0; type_index < tendency.emotions.size(); ++type_index)
        {
            actor_->emotions_[type_index] = chronicle_.CreateEmotion(static_cast<EmotionType>(type_index), 0, actor_, no_reasons, random.GetFloat(-1.0f, 1.0f));
        }
        ContextType context = (random.GetFloat(-1.0f, 1.0f) <= 0 ? ContextType::kCourse : ContextType::kFreetime);
        Kernel *reason;
        float chance = actor_->CalculateTendencyChance(tendency, context, reason);
        EXPECT_GE(chance, 0.0f);
        EXPECT_LE(chance, 1.0f);
    }
}
TEST(TaleRandom, PickIndexWithHundredPercentChance)
{
    uint32_t seconds = static_cast<uint32_t>(time(NULL));
    Random random(seconds);
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
    Random random(seconds);
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
    Random random(seconds);
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
    Random random(seconds);
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