#ifndef TALE_ACTOR_H
#define TALE_ACTOR_H

#include <string>
#include <map>
#include <memory>
#include "tale/setting.hpp"
#include "tale/globals/random.hpp"
#include "tale/globals/interactionstore.hpp"
#include "tale/kernels/goal.hpp"
#include "tale/kernels/resourcekernels/resource.hpp"
#include "tale/kernels/resourcekernels/emotion.hpp"
#include "tale/kernels/resourcekernels/relationship.hpp"
#include "tale/kernels/trait.hpp"
#include "tale/kernels/interactions/interactionrequirement.hpp"

namespace tale
{
    class School;
    /**
     * @brief Represents one person in the simulation.
     * An Actor decides how to act each tick of the simulation. To do this it needs to hold all information necessary to come to a decision.
     */
    class Actor : public std::enable_shared_from_this<Actor>
    {
    public:
        /**
         * @brief The id of the actor. Corresponds to its index in the actors_ vector in School.
         */
        const size_t id_;
        /**
         * @brief The first name of the Actor
         */
        const std::string first_name_;
        /**
         * @brief The last name of the Actor
         */
        const std::string last_name_;
        /**
         * @brief The abstract state of the  \link Actor Actor's \endlink wealth.
         */
        std::weak_ptr<Resource> wealth_;
        /**
         * @brief The \link Actor Actor's \endlink current \link Emotion Emotional \endlink state is. Maps one Emotion to each EmotionType.
         */
        std::map<EmotionType, std::weak_ptr<Emotion>> emotions_;
        /**
         * @brief Holds the  \link Actor Actor's \endlink \link Relationship Relationships \endlink with other \link Actor Actors \endlink.
         * Maps the other \link Actor Actor's \endlink id to another map of RelationshipType to Relationship.
         */
        std::map<size_t, std::map<RelationshipType, std::weak_ptr<Relationship>>> relationships_;
        /**
         * @brief The \link Trait Traits \endlink  the Actor posses.
         */
        std::vector<std::weak_ptr<Trait>> traits_;
        /**
         * @brief The Goal the Actor is trying to reach during the simulation.
         */
        std::weak_ptr<Goal> goal_;

        Actor(School &school, size_t id, std::string first_name, std::string last_name);
        std::string GetFullName() const;
        void SetupRandomValues(size_t tick);
        /**
         * @brief Checks if the Actor is enrolled in the passed Course.
         *
         * @param course_id The id of the Course we want to check.
         * @return The result of the check.
         */
        bool IsEnrolledInCourse(size_t course_id) const;
        /**
         * @brief Enrolls the Actor in the passed course during the passed slot.
         *
         * This can crash if the slot is already filled.
         *
         * @param course_id The id of the Course we want the Actor to enroll in.
         * @param slot The slot during which we want the Actor to enroll in the course.
         */
        void EnrollInCourse(size_t course_id, uint32_t slot);
        void EjectFromCourse(size_t course_id, uint32_t slot);
        /**
         * @brief Getter for the amount of filled slots.
         *
         * @return The amount of filled slots.
         */
        size_t GetFilledSlotsCount() const;
        /**
         * @brief Checks wether all slots are already filled.
         *
         * @return The result of the check.
         */
        bool AllSlotsFilled() const;
        /**
         * @brief Checks wether the passed slots are empty.
         *
         * @param  slots  Vector of slots we want to check for.
         * @return The result of the check.
         */
        bool SlotsEmpty(const std::vector<uint32_t> &slots) const;
        /**
         * @brief Let's the actor decide on an Interaction for the currrent situation.
         *
         * WIP: Currently just chooses an Interaction at random.
         *
         * @param[in] course_group The course group the Actor is currently interacting in.
         * @param[out] out_reasons Vector the Actor will write it's reason for the decision to.
         * @param[out] out_participants Vector the Actor will to with which other \link Actor Actors \endlink he wants to do the Interaction.
         * @param[out] out_chance How likely it was that this interaction was chosen.
         * @return The index of the InteractionPrototype the Actor chose.
         */
        bool Actor::SlotEmpty(size_t slot) const;
        int ChooseInteraction(const std::vector<std::weak_ptr<Actor>> &actor_group, ContextType context, std::vector<std::weak_ptr<Kernel>> &out_reasons, std::vector<std::weak_ptr<Actor>> &out_participants, float &out_chance);
        /**
         * @brief Calculates the chance of an Interaction based on its Tendency and the \link Actor Actor's \endlink current state.
         *
         * Every value used is based between -1.0 and 1.0 except the return value which is between 0.0 and 1.0.
         * Because the Actor uses this value for a distribution to pick the Interaction it wants to do, 1.0 does not
         * automatically mean an Interaction has a 100% chance of being picked. If we have for example three
         * \link Interaction Interactions \endlink, which prodcue the retun values 1.0, 1.0 and 0.0. Then the first
         * two Interactions have a chance of 50% each and the last one has a 0% chance.
         *
         * @param[in] tendency The tendency of the Interaction we want the chance for.
         * @param[in] context In which context the Interaction would happen.
         * @param[in] group_size_ratio How big the group is in which this Interaction is happening (between -1.0 and 1.0).
         * @param[out] out_reason Which part of the state of the Actor increased the chance the most.
         * @return A chance value between 0.0 and 1.0.
         */
        float CalculateTendencyChance(const Tendency &tendency, const ContextType &context, std::weak_ptr<Kernel> &out_reason);
        float ApplyGoalChanceModification(float original_chance, size_t interaction_index, bool &out_had_positive_effect);
        bool CheckRequirements(const Requirement &requirement, const std::vector<std::weak_ptr<Actor>> &actor_group, ContextType context) const;
        /**
         * @brief Applies a change to the \link Actor Actor's \endlink wealth.
         *
         * @param reasons Vector holding the reasons for this change.
         * @param tick The tick during which this change happened.
         * @param value By how much the wealth gets changed.
         */
        void ApplyWealthChange(const std::vector<std::weak_ptr<Kernel>> &reasons, size_t tick, float value);
        /**
         * @brief Applies a change to the \link Actor Actor's \endlink \link Emotion Emotional \endlink state.
         *
         *
         *
         * @param reasons Vector holding the reasons for this change.
         * @param tick The tick during which this change happened.
         * @param type The type of Emotion that gets changed.
         * @param value By how much the Emotion gets changed.
         */
        void ApplyEmotionChange(const std::vector<std::weak_ptr<Kernel>> &reasons, size_t tick, EmotionType type, float value);
        /**
         * @brief Applies a change to the \link Actor Actor's \endlink \link Relationship Relationships \endlink.
         *
         *
         *
         * @param reasons Vector holding the reasons for this change.
         * @param tick The tick during which this change happened.
         * @param actor_id The id of the Actor with which the Relationship gets changed..
         * @param type The type of Relationship that gets changed.
         * @param value By how much the Relationship gets changed.
         */
        void ApplyRelationshipChange(const std::vector<std::weak_ptr<Kernel>> &reasons, size_t tick, size_t actor_id, std::map<RelationshipType, float> change);
        /**
         * @brief Creates a string describing the current wealth status of the Actor.
         *
         * @return The description string.
         */
        std::string GetWealthDescriptionString();
        /**
         * @brief Creates a string describing the current Emotion status of the Actor.
         *
         * @return The description string.
         */
        std::string GetEmotionsDescriptionString();
        /**
         * @brief Creates a string describing the current \link Relationship Relationships \endlink  status of the Actor.
         *
         * @return The description string.
         */
        std::string GetRelationshipsDescriptionString();
        /**
         * @brief Creates a string describing the Goal of the Actor.
         *
         * @return The description string.
         */
        std::string GetGoalDescriptionString();
        /**
         * @brief Creates a string describing the \link Trait Traits \endlink  of the Actor.
         *
         * @return The description string.
         */
        std::string GetTraitsDescriptionString();
        const std::vector<std::weak_ptr<Actor>> &GetAllKnownActors() const;
        const std::vector<std::weak_ptr<Actor>> &GetFreetimeActorGroup() const;
        float CalculateRelationshipValue(size_t actor_id) const;
        bool HasRelationshipWith(size_t actor_id) const;
        void InsertNewRelationship(std::weak_ptr<Actor> other_actor, std::map<RelationshipType, std::weak_ptr<Relationship>> relationship);

    private:
        /**
         * @brief Holds a Reference to the Random object of the simulation.
         */
        Random &random_;
        /**
         * @brief Holds a Reference to the Chronicle object of the simulation.
         */
        Chronicle &chronicle_;
        /**
         * @brief Holds a Reference to the School object of the simulation.
         */
        School &school_;
        /**
         * @brief Holds a Reference to the InteractionStore object of the simulation.
         */
        InteractionStore &interaction_store_;
        /**
         * @brief Holds a reference to the instance of the Setting object of the simulation.
         */
        const Setting &setting_;
        /**
         * @brief Counts how many of the \link Actor Actor's \endlink time slots are already filled with \link Course Courses \endlink.
         */
        size_t filled_slots_count_ = 0;
        /**
         * @brief Holds the ids of the \link Course Courses \endlink the Actor is enrolled in, in the order of the slots he will visit said courses.
         */
        std::vector<int> enrolled_courses_id_;
        std::vector<std::weak_ptr<Actor>> known_actors_;
        /**
         * @brief Initializes the Wealth member with a random value.
         *
         *
         *
         * @param tick The tick during which this happens.
         */
        void
        InitializeRandomWealth(size_t tick);
        /**
         * @brief Initializes the Emotion member map with random values.
         *
         *
         *
         * @param tick The tick during which this happens.
         */
        void InitializeRandomEmotions(size_t tick);
        /**
         * @brief Initializes the Relationship member map with random values.
         *
         * WIP: this does not to anything yet
         *
         * @param tick The tick during which this happens.
         */
        void InitializeRandomRelationships(size_t tick);
        /**
         * @brief Initializes the the Goal member with a random value.
         *
         * WIP: this does not to anything yet
         *
         * @param tick The tick during which this happens.
         */
        void InitializeRandomGoal(size_t tick);
        /**
         * @brief Initializes the Trait member vector with a random \link Trait Traits \endlink.
         *
         * WIP: this does not to anything yet
         *
         * @param tick The tick during which this happens.
         */
        void InitializeRandomTraits(size_t tick);

        void UpdateKnownActors();
    };

} // namespace tale
#endif // TALE_ACTOR_H