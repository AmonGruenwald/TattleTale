#ifndef TALE_SCHOOL_H
#define TALE_SCHOOL_H

#include <memory>
#include <vector>
#include "tale/course.hpp"
#include "tale/setting.hpp"
#include "tale/globals/random.hpp"
#include "tale/globals/chronicle.hpp"
#include "tale/globals/interactionstore.hpp"
#include "tale/actor.hpp"

namespace tale
{
    /**
     * @brief Enum specifying the days of the week.
     */
    enum class Weekday
    {
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday
    };
    /**
     * @brief Array that allows easy transformation from Weekday enum to a string..
     */
    static const char *weekday_string[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    /**
     * @brief School class that is responsible for managing actors and courses during the simulation
     *
     * The School class uses a \link Setting Settings \endlink object in its constructor to setup the simulation, instancing the appropriate amount of \link Actor Actors \endlink and \link Course Courses \endlink and then filling said
     * \link Course Courses \endlink with \link Actor Actors \endlink. Afterwards it can be used to run the simulation.
     */
    class School
    {
    public:
        /**
         * @brief Constructor that sets up everything necessayr for the simulation.
         *
         * Stores references to a Random and an InteractionStore Object, to be used in the simulation. Uses the passed Setting object to instance actors and courses correctly.
         *
         * @param setting Reference to the Setting object for the Simulation
         */
        School(const Setting &setting);
        /**
         * @brief Runs the simulation for the passed amount of days.
         *
         * Calls the private SimulatedDay function and increases current_day_ and increments current_weekday_.
         *
         * @param days How many days we want to simulate
         */
        void SimulateDays(size_t days);
        /**
         * @brief Getter for an Actor
         *
         * @param  actor_id The id of the Actor we want to get. Corresponds to index in actors_ vector.
         * @return The Actor.
         */
        std::weak_ptr<Actor> GetActor(size_t actor_id);
        /**
         * @brief Getter for a Course
         *
         * @param  actor_id The id of the Course we want to get. Corresponds to index in courses_ vector.
         * @return The Course.
         */
        Course &GetCourse(size_t course_id);
        /**
         * @brief Getter for the current Weekday.
         *
         * @return The current Weekday.
         */
        Weekday GetCurrentWeekday() const;
        /**
         * @brief Getter for the current day
         *
         * @return The current day.
         */
        size_t GetCurrentDay() const;
        /**
         * @brief Getter for a Reference to the Setting object of the simulation
         *
         * @return Reference to the Setting object.
         */
        const Setting &GetSetting() const;
        /**
         * @brief Getter for a Reference to the InteractionStore object of the simulation
         *
         * @return Reference to the InteractionStore object.
         */
        InteractionStore &GetInteractionStore();
        /**
         * @brief Getter for a Reference to the Random object of the simulation
         *
         * @return Reference to the Random object.
         */
        Random &GetRandom();
        /**
         * @brief Getter for a Reference to the Chronicle object of the simulation
         *
         * @return Reference to the Chronicle object.
         */
        Chronicle &GetChronicle();

    private:
        /**
         * @brief Holds all instanced courses
         */
        std::vector<Course> courses_;
        /**
         * @brief Holds all instanced actors.
         */
        std::vector<std::shared_ptr<Actor>> actors_;
        /**
         * @brief Holds all instanced actors as weak_ptrs.
         */
        std::vector<std::weak_ptr<Actor>> freetime_group_;
        /**
         * @brief Holds a reference to an instance of the Setting object that was passed during construction.
         */
        const Setting &setting_;
        /**
         * @brief Holds the instance of the Random object of the simulation.
         */
        Random random_;
        /**
         * @brief Holds the instance of the InteractionStore object for the simulation.
         */
        InteractionStore interaction_store_;
        /**
         * @brief Holds the instance of the Chronicle object for the simulation.
         */
        Chronicle chronicle_;
        /**
         * @brief The current tick of the simulation (increases after each slot or freetime activity has commenced for every actor). This is always the tick that will be simulated next.
         */
        size_t current_tick_ = 0;
        /**
         * @brief The current day. This is always the day that will be simulated next.
         */
        size_t current_day_ = 0;
        /**
         * @brief The current Weekday. This is always the Weekday that will be simulated next.
         */
        Weekday current_weekday_ = Weekday::Monday;
        /**
         * @brief Simulates a single day.
         *
         * Depending on wether it is a workday or the weekend let's the actors go through different \link Interaction Interactions \endlink.
         *
         * @param day Number of the day we want to simulate.
         * @param weekday Which Weekday we want to simulate.
         */
        void SimulateDay(size_t day, Weekday weekday);
        /**
         * @brief Simulates a tick where the  \link Actor Actors \endlink have free time.
         */
        void FreeTimeTick();
        /**
         * @brief Checks wheter the passed Weeekday is a workday or not.
         *
         * @param weekday Which Weekday we want to check.
         * @return The result of the check.
         */
        void LetActorInteract(std::shared_ptr<Actor> &actor, const std::vector<std::weak_ptr<Actor>> &group, ContextType type, std::string context_description = "an unknown time");
        bool IsWorkday(Weekday weekday) const;
        /**
         * @brief Checks wheter the passed Actor is in the passed course group.
         *
         * @param actor Which Actor we want to check.
         * @param course_group Which course group we want to look for the actor in.
         * @return The result of the check.
         */
        bool ActorIsInCourseGroup(const std::shared_ptr<Actor> &actor, const std::vector<std::weak_ptr<Actor>> &course_group) const;
        /**
         * @brief Transforms a Weekday and a daily tick to a slot index.
         *
         * Slots increase each day. So if we assume there are 6 slots per day, the slot indices for monday are 0, 1, 2, 3, 4, 5 and the slots for tuesday 6, 7, 8, 9, 10, 11.
         * This transforms a Weekday and daily tick index (which in the example above would go from 0-5) to corresponding slot index.
         *
         * @param weekday The Weekday from which we want to transform the daily tick to a slot index.
         * @param daily_tick The daily tick we want to transform to a slot index.
         * @return The slot index.
         */
        size_t WeekdayAndDailyTickToSlot(Weekday weekday, size_t daily_tick) const;
        /**
         * @brief Finds a random group of \link Actor Actors \endlink that could take the Course during those slots.
         *
         * Picks random \link Actor Actors \endlink and checks if they do not already have the Course corresponding to the course_id. It furhter makes sure that the \link Actor Actors \endlink
         * also are free during the passed slots. Because of those conditions it is not guaranteed that a course_group will have the full amount of \link Actor Actors \endlink specified in setting_.
         *
         * @param course_id The id of the course we ant to find a a random group for. Corresponds to the courses_ vector.
         * @param slots The slots we want to find free \link Actor Actors \endlink for.
         * @return The found group of Actors.
         */
        std::vector<std::weak_ptr<Actor>> FindRandomCourseGroup(size_t course_id, const std::vector<uint32_t> &slots);

        /**
         * @brief Creates a vector of randomly picked firstnames.
         *
         * Reads from the file tale/resources/firstname.txt to get random names;
         *
         * @param count How many firstnames the vector should contain.
         * @return The vector of firstnames.
         */
        std::vector<std::string> GetRandomFirstnames(size_t count);
        /**
         * @brief Creates a vector of randomly picked surnames.
         *
         * Reads from the file tale/resources/surname.txt to get random names;
         *
         * @param count How many surnames the vector should contain.
         * @return he vector of surnames.
         */
        std::vector<std::string> GetRandomSurames(size_t count);
        /**
         * @brief Creates a vector of randomly picked names.
         *
         * Reads from the passed filepath. This function should probably only be used internally by GetRandomFirstnames and GetRandomSurnames
         *
         * @param count How many surnames the vector should contain.
         * @param path The path of the file containing the names to pick from.
         * @return he vector of names.
         */
        std::vector<std::string> GetRandomNames(size_t count, std::string path);
    };

} // namespace tale
#endif // TALE_SCHOOL_H