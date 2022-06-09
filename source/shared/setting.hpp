#ifndef TALE_SETTING_H
#define TALE_SETTING_H

#include <math.h>
#include <assert.h>
#include <algorithm>
#include <shared/tattletalecore.hpp>
#include <fmt/core.h>
#include <fmt/format.h>
namespace tattletale
{
    /**
     * @brief Stores all settings necessary for the simulation
     *
     * Only one Setting object should exist per simulation, so its run can be reproduced using the same Setting.
     */
    struct Setting
    {
        /**
         * @brief Seed used for the Random object used in the Simulation.
         */
        uint32_t seed = 123456789;
        /**
         * @brief How many days will be simulated.
         */
        size_t days_to_simulate = 5;
        /**
         * @brief How many \link Actor Actors \endlink will be instanced.
         */
        size_t actor_count = 100;
        /**
         * @brief How may \link Course Courses \endlink an Actor can visit per day.
         */
        size_t courses_per_day = 6;
        /**
         * @brief How many \link Actor Actors \endlink a Course can hold at the same time.
         */
        size_t actors_per_course = 30;
        /**
         * @brief How many times an Actor can visit the same Course per week.
         */
        size_t same_course_per_week = 4;
        /**
         * @brief How many \link Relationship Relationships \endlink an Actor should at least have when he gets initialized
         */
        uint32_t desired_min_start_relationships_count = 0;
        /**
         * @brief How many \link Relationship Relationships \endlink an Actor can have when he gets initialized
         */
        uint32_t desired_max_start_relationships_count = 4;
        /**
         * @brief To how many \link Actor Actors \endlink another Actor would reach out to during freetime.
         */
        size_t freetime_actor_count = 6;
        /**
         * @brief How many Kernel objects are contained in a chain that could potentially be curated.
         */
        size_t max_chain_size = 5;
        /**
         * @brief Calculates how many slots are there in total in a week.
         *
         * This is just the amount of courses per day times the length of a workweek (which is hardcoded as 5).
         *
         * @return The amount of slots
         */
        size_t slot_count_per_week() const
        {
            // 5 day workweek
            return courses_per_day * 5;
        }
        /**
         * @brief Calculates the minimum necessary amount of courses so every actor can theoretically completely feel their schedule
         *
         * @return The amount of courses
         */
        size_t minimum_necessary_course_count() const
        {
            if (same_course_per_week == 0)
            {
                return 0;
            }
            return (size_t)ceil((double)slot_count_per_week() / (double)same_course_per_week);
        }

        /**
         * @brief Calculates the amount of courses that the simulation will have.
         *
         * This is either the minimum_necessary_course_count() or actor_count/actor_per_course, which ever is bigger.
         *
         * @return The amount of courses
         */
        size_t course_count() const
        {
            if (actors_per_course == 0)
            {
                return 0;
            }
            return std::max((size_t)minimum_necessary_course_count(), (size_t)ceil((double)actor_count / (double)actors_per_course));
        }
        /**
         * @brief Calculates the amount of minimum start \link Relationship Relationships \endlink an Actor should have.
         *
         * This is either whatever desired_min_start_relationships_count is or equal to actor_count - 1 as an Actor can't have more
         * \link Relationship Relationships \endlink than there are \link Actor Actors \endlink
         *
         * @return The max amount of start \link Relationship Relationships \endlink.
         */
        uint32_t min_start_relationships_count() const
        {
            TATTLETALE_ERROR_PRINT(desired_min_start_relationships_count <= desired_max_start_relationships_count, "desired_min_start_relationships_count must be smaller or equal to desired_max_start_relationships_count");
            return std::min((uint32_t)(std::max((int)(actor_count - 1), 0)), desired_min_start_relationships_count);
        }
        /**
         * @brief Calculates the amount of maximum start \link Relationship Relationships \endlink an Actor can potentially have.
         *
         * This is either whatever desired_max_start_relationships_count is or equal to actor_count - 1 as an Actor can't have more
         * \link Relationship Relationships \endlink than there are \link Actor Actors \endlink
         *
         * @return The max amount of start \link Relationship Relationships \endlink.
         */
        uint32_t max_start_relationships_count() const
        {
            TATTLETALE_ERROR_PRINT((desired_min_start_relationships_count <= desired_max_start_relationships_count), "desired_min_start_relationships_count must be smaller or equal to desired_max_start_relationships_count");
            return std::min((uint32_t)(std::max((int)(actor_count - 1), 0)), desired_max_start_relationships_count);
        }

        std::string ToString() const
        {
            std::string string = "";
            string += fmt::format("The seed was {}.\n", seed);
            string += fmt::format("Simulated {} days with {} actors that had \nbetween {} and {} established relationship at the \nstart.\n", days_to_simulate, actor_count, desired_min_start_relationships_count, desired_max_start_relationships_count);
            string += fmt::format("They had {} courses per day with {} actors per \ncourse and each course was run {} per week.\n", courses_per_day, actors_per_course, same_course_per_week);
            string += fmt::format("During freetime the actors could choose to \ninteract from a group of {} other actors.\n", freetime_actor_count);
            string += fmt::format("For the curation kernel chains of maximum size \n{} were considered.\n", max_chain_size);
            return string;
        }
    };

} // namespace tattletale

template <>
struct fmt::formatter<tattletale::Setting>
{
    // d - default
    char presentation = 'd';

    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && (*it == 'd'))
            presentation = *it++;

        if (it != end && *it != '}')
            throw format_error("invalid format");

        return it;
    }

    template <typename FormatContext>
    auto format(const tattletale::Setting &setting, FormatContext &ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "{}", setting.ToString());
    }
};
#endif // TALE_SETTING_H