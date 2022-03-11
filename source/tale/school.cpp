#include "tale/talecore.hpp"
#include "tale/school.hpp"
#include "tale/kernels/interactions/interactionrequirement.hpp"
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <fstream>

namespace tale
{
    School::School(const Setting &setting) : setting_(setting), random_(setting.seed), chronicle_(random_, setting.actor_count), interaction_store_(random_, chronicle_)
    {
        size_t actor_count = setting_.actor_count;
        size_t tick = 0;

        std::vector<std::string> firstnames = GetRandomFirstnames(actor_count);
        std::vector<std::string> surnames = GetRandomSurames(actor_count);
        std::string actor_creation_description = "CREATED ALL ACTORS:";
        for (size_t i = 0; i < actor_count; ++i)
        {
            std::string name = firstnames[i] + " " + surnames[i];
            actor_creation_description += ("\n" + std::to_string(i) + ": " + name);
            std::shared_ptr<Actor> actor(new Actor(*this, i, name));

            actors_.push_back(actor);
            // TODO: creating actors should probably be handled by the Chronicle class, similar to Kernels
            chronicle_.actors_.push_back(actor);
            freetime_group_.push_back(actor);
        }
        // needs to happen after every actor is created bc of relationships
        for (auto &actor : actors_)
        {
            actor->SetupRandomValues(tick);
        }
        TALE_DEBUG_PRINT(actor_creation_description);
        std::string detailed_actor_description = "DETAILED ACTOR DESCRIPTION:";
        for (size_t i = 0; i < actor_count; ++i)
        {
            detailed_actor_description += ("\n" + actors_[i]->name_);
            detailed_actor_description += ("\n\t" + actors_[i]->GetWealthDescriptionString());
            detailed_actor_description += ("\n\t" + actors_[i]->GetEmotionsDescriptionString());
            detailed_actor_description += ("\n\t" + actors_[i]->GetRelationshipsDescriptionString());
            detailed_actor_description += ("\n\t" + actors_[i]->GetTraitsDescriptionString());
            detailed_actor_description += ("\n\t" + actors_[i]->GetGoalDescriptionString());
        }
        TALE_VERBOSE_PRINT(detailed_actor_description);

        size_t course_count = setting_.course_count();
        size_t slot_count_per_week = setting_.slot_count_per_week();
        std::vector<uint32_t> random_slot_order;
        for (size_t i = 0; i < course_count; ++i)
        {
            // TODO: create better names
            courses_.push_back(Course(random_, setting_, i, "Course " + std::to_string(i)));

            // Filling courses:
            std::string course_creation_description = ("CREATED COURSE " + std::to_string(courses_[i].id_));
            random_slot_order.clear();
            // randomly order all slots of the course
            // TODO: optimize this by shuffling an already filled vector everytime instead of this
            while (random_slot_order.size() < slot_count_per_week)
            {
                uint32_t random_slot = random_.GetUInt(0, slot_count_per_week - 1);
                while (std::count(random_slot_order.begin(), random_slot_order.end(), random_slot))
                {
                    random_slot += 1;
                    random_slot %= slot_count_per_week;
                }
                random_slot_order.push_back(random_slot);
            }

            size_t slot_index = 0;
            // Go over every slot
            while (slot_index < random_slot_order.size() && setting_.same_course_per_week > 0)
            {
                std::vector<uint32_t> slots;
                // Because Actor have the same course a few times per week use the same group a few times
                for (size_t j = 0; j < setting_.same_course_per_week && slot_index < random_slot_order.size(); ++j)
                {
                    slots.push_back(random_slot_order[slot_index]);
                    ++slot_index;
                }
                std::vector<std::weak_ptr<Actor>> course_group = FindRandomCourseGroup(courses_[i].id_, slots);

                course_creation_description += "\n\t";
                for (size_t j = 0; j < slots.size(); ++j)
                {
                    course_creation_description += "[";
                    if (slots[j] < 10)
                    {
                        course_creation_description += "0";
                    }
                    course_creation_description += std::to_string(slots[j]);
                    course_creation_description += "]";
                    courses_[i].AddToSlot(course_group, slots[j]);
                }
                course_creation_description += (" <- " + std::to_string(course_group.size()) + " ACTORS.");
            }
            TALE_DEBUG_PRINT(course_creation_description);
        }
    }

    void School::SimulateDays(size_t days)
    {
        for (size_t i = 0; i < days; ++i)
        {
            SimulateDay(current_day_, current_weekday_);
            ++current_day_;
            current_weekday_ = static_cast<Weekday>((static_cast<int>(current_weekday_) + 1) % 7);
        }
        TALE_DEBUG_PRINT(std::to_string(chronicle_.GetKernelAmount()) + " KERNELS CREATED.");
        TALE_DEBUG_PRINT("RANDOM KERNEL CHAIN 1:\n" + chronicle_.GetRandomCausalityChainDescription(3));
        TALE_DEBUG_PRINT("RANDOM KERNEL CHAIN 2:\n" + chronicle_.GetRandomCausalityChainDescription(3));
        TALE_DEBUG_PRINT("RANDOM KERNEL CHAIN 3:\n" + chronicle_.GetRandomCausalityChainDescription(3));
        TALE_DEBUG_PRINT("RANDOM KERNEL CHAIN 4:\n" + chronicle_.GetRandomCausalityChainDescription(3));
        TALE_DEBUG_PRINT("RANDOM KERNEL CHAIN 5:\n" + chronicle_.GetRandomCausalityChainDescription(3));
        TALE_DEBUG_PRINT("GOAl KERNEL CHAIN:\n" + chronicle_.GetGoalCausalityChainDescription(3));
        TALE_DEBUG_PRINT("ACTOR INTERACTIONS:\n" + chronicle_.GetActorInteractionsDescription(random_.GetUInt(0, setting_.actor_count - 1)));
    }
    std::weak_ptr<Actor> School::GetActor(size_t actor_id)
    {
        assert(actor_id < actors_.size());
        return actors_[actor_id];
    }
    Course &School::GetCourse(size_t course_id)
    {
        assert(course_id < courses_.size());
        return courses_[course_id];
    }

    size_t School::GetCurrentDay() const
    {
        return current_day_;
    }
    Weekday School::GetCurrentWeekday() const
    {
        return current_weekday_;
    }
    const Setting &School::GetSetting() const
    {
        return setting_;
    }
    InteractionStore &School::GetInteractionStore()
    {
        return interaction_store_;
    }
    Random &School::GetRandom()
    {
        return random_;
    }
    Chronicle &School::GetChronicle()
    {
        return chronicle_;
    }
    void School::SimulateDay(size_t day, Weekday weekday)
    {
        TALE_DEBUG_PRINT(std::to_string(day) + ". " + weekday_string[static_cast<int>(weekday)]);
        if (IsWorkday(weekday))
        {
            for (size_t i = 0; i < setting_.courses_per_day; ++i)
            {
                size_t slot = WeekdayAndDailyTickToSlot(weekday, i);
                for (auto &course : courses_)
                {
                    std::vector<std::weak_ptr<Actor>> course_group = course.GetCourseGroupForSlot(slot);
                    for (auto &actor : course_group)
                    {
                        std::vector<std::weak_ptr<Kernel>> reasons;
                        std::vector<std::weak_ptr<Actor>> participants;
                        int interaction_index = actor.lock()->ChooseInteraction(course_group, ContextType::kCourse, reasons, participants);

                        if (interaction_index != -1)
                        {
                            std::weak_ptr<Interaction> interaction = interaction_store_.CreateInteraction(interaction_index, current_tick_, reasons, participants);
                            interaction.lock()->Apply();
                            TALE_VERBOSE_PRINT("During Slot " + std::to_string(i) + " in " + course.name_ + " " + interaction.lock()->ToString());
                        }
                        else
                        {
                            TALE_VERBOSE_PRINT("During Slot " + std::to_string(i) + " in " + course.name_ + actor.lock()->name_ + " did nothing.");
                        }
                    }
                }
                ++current_tick_;
            }
            FreeTimeTick();
            ++current_tick_;
        }
        else
        {
            for (size_t i = 0; i < setting_.courses_per_day + 1; ++i)
            {
                FreeTimeTick();
                ++current_tick_;
            }
        }
    }

    void School::FreeTimeTick()
    {
        for (auto &actor : actors_)
        {
            std::vector<std::weak_ptr<Kernel>> reasons;
            std::vector<std::weak_ptr<Actor>> participants;
            int interaction_index = actor->ChooseInteraction(actor->GetAllKnownActors(), ContextType::kFreetime, reasons, participants);
            // TODO: registers interaction to events
            if (interaction_index != -1)
            {
                std::weak_ptr<Interaction> interaction = interaction_store_.CreateInteraction(interaction_index, current_tick_, reasons, participants);
                interaction.lock()->Apply();
                TALE_VERBOSE_PRINT("During Freetime " + interaction.lock()->ToString());
            }
            else
            {
                TALE_VERBOSE_PRINT("During Freetime " + actor->name_ + " does nothing.");
            }
        }
    }

    bool School::IsWorkday(Weekday weekday) const
    {
        if (weekday == Weekday::Saturday || weekday == Weekday::Sunday)
        {
            return false;
        }
        return true;
    }

    bool School::ActorIsInCourseGroup(const std::shared_ptr<Actor> &actor, const std::vector<std::weak_ptr<Actor>> &course_group) const
    {
        for (auto &actor_in_course_group : course_group)
        {
            if (actor_in_course_group.lock() == actor)
            {
                return true;
            }
        }
        return false;
    }

    size_t School::WeekdayAndDailyTickToSlot(Weekday weekday, size_t daily_tick) const
    {
        return static_cast<size_t>(weekday) * setting_.courses_per_day + daily_tick;
    }

    std::vector<std::weak_ptr<Actor>> School::FindRandomCourseGroup(size_t course_id, const std::vector<uint32_t> &slots)
    {
        std::vector<std::weak_ptr<Actor>> course_group;
        if (actors_.size() > 0)
        {
            for (size_t i = 0; i < setting_.actors_per_course; ++i)
            {
                size_t random_actor_index = random_.GetUInt(0, actors_.size() - 1);

                size_t current_index_search_try = 0;
                while (
                    (actors_[random_actor_index]->AllSlotsFilled() || actors_[random_actor_index]->IsEnrolledInCourse(course_id) || ActorIsInCourseGroup(actors_[random_actor_index], course_group) || !actors_[random_actor_index]->SlotsEmpty(slots)) && current_index_search_try < setting_.actor_count)
                {
                    random_actor_index += -1;
                    random_actor_index %= actors_.size();
                    ++current_index_search_try;
                }
                if (current_index_search_try != actors_.size())
                {
                    course_group.push_back(actors_[random_actor_index]);
                }
            }
        }
        return course_group;
    }
    std::vector<std::string> School::GetRandomFirstnames(size_t count)
    {
        return GetRandomNames(count, "tale/resources/firstname.txt");
    }
    std::vector<std::string> School::GetRandomSurames(size_t count)
    {
        return GetRandomNames(count, "tale/resources/surname.txt");
    }

    std::vector<std::string> School::GetRandomNames(size_t count, std::string path)
    {
        std::vector<std::string> all_names;
        std::string firstname;
        std::ifstream firstname_file(path);
        assert(firstname_file.is_open()); // Couldn't open the file
        while (getline(firstname_file, firstname))
        {
            all_names.push_back(firstname);
        }
        std::vector<std::string> random_names;
        for (size_t i = 0; i < count; ++i)
        {
            random_names.push_back(all_names[random_.GetUInt(0, all_names.size() - 1)]);
        }
        return random_names;
    }

} // namespace tale