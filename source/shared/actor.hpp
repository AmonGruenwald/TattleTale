#ifndef TALE_ACTOR_H
#define TALE_ACTOR_H

#include <string>
#include <list>
#include <memory>
#include <robin_hood.h>
#include "shared/setting.hpp"
#include "shared/random.hpp"
#include "tale/interactionstore.hpp"
#include "shared/kernels/goal.hpp"
#include "shared/kernels/resourcekernels/resource.hpp"
#include "shared/kernels/resourcekernels/emotion.hpp"
#include "shared/kernels/resourcekernels/relationship.hpp"
#include "shared/kernels/interactions/interactionrequirement.hpp"

namespace tattletale
{
    class Chronicle;
    class School;
    /**
     * @brief Represents one person in the simulation.
     * An Actor decides how to act each tick of the simulation. To do this it needs to hold all information necessary to come to a decision.
     */
    class Actor
    {
    public:
        /**
         * @brief The id of the actor. Corresponds to its index in the actors_ vector in School.
         */
        const size_t id_;
        /**
         * @brief The full name of the Actor
         */
        const std::string name_;
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
        Resource *wealth_;
        /**
         * @brief The \link Actor Actor's \endlink current \link Emotion Emotional \endlink state is. Maps one Emotion to each EmotionType.
         */
        std::vector<Emotion *> emotions_;
        /**
         * @brief Holds the  \link Actor Actor's \endlink \link Relationship Relationships \endlink with other \link Actor Actors \endlink.
         * Maps the other \link Actor Actor's \endlink id to another map of RelationshipType to Relationship.
         */
        robin_hood::unordered_map<size_t, std::vector<Relationship *>> relationships_;
        /**
         * @brief The Goal the Actor is trying to reach during the simulation.
         */
        Goal *goal_;

        /**
         * @brief Randomizes the internal state of the Actor.
         *
         * @param tick During which tick this process happens.
         */
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
        /**
         * @brief Removes the Actor from the passed course during the passed slot.
         *
         * This can crash if the slot is already empty.
         *
         * @param course_id The id of the Course we want the Actor to be removed from.
         * @param slot The slot from which we want the Actor to be removed.
         */
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
         * @brief Let's the actor decide on an Interaction for the currrent situation depending on their internal state.
         *
         * This first weeds out Interaction that are not possible due to their InteractionRequirements. Then calculates chances for each
         * possible one based on their respective InteractionTendency and the \link Actor Actor's \endlink internal state and then chooses
         * one at semi randomly based on the chances. Afterwards a similar process is employed to pick necessary participants for the Interaction.
         *
         * @param[in] actor_group The course group the Actor is currently interacting in.
         * @param[in] context The ContextType this Interaction will be in.
         * @param[out] out_reasons Vector the Actor will write it's reason for the decision to.
         * @param[out] out_participants Vector the Actor will to with which other \link Actor Actors \endlink he wants to do the Interaction.
         * @param[out] out_chance How likely it was that this interaction was chosen.
         * @return The index of the InteractionPrototype the Actor chose.
         */
        int ChooseInteraction(const std::list<Actor *> &actor_group, ContextType context, std::vector<Kernel *> &out_reasons, std::vector<Actor *> &out_participants, float &out_chance);
        /**
         * @brief Checks wether the passed slot is still unused for the Actor.
         *
         * @param slot The slot that will be checked.
         * @return The result of the check.
         */
        bool SlotEmpty(size_t slot) const;
        /**
         * @brief Calculates the chance of an Interaction based on its InteractionTendency and the \link Actor Actor's \endlink current state.
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
        float CalculateInteractionChance(const InteractionTendency &tendency, const ContextType &context, Kernel *&out_reason);
        /**
         * @brief Modifies the chance for an Interaction based on the Goal of the Actor.
         *
         * @param original_chance The chance of the Interaction before this modification.
         * @param interaction_index The index of the Interaction in the InteractionStore.
         * @param out_reason If the goal had an effect, this will hold the Goal Kernel otherwise it will be a nullptr.
         * @return The modified chance.
         */
        float ApplyGoalChanceModification(float original_chance, size_t interaction_index, Kernel *&out_reason);
        /**
         * @brief Checks wether the requirements of an Interaction are met based on the passed InteractionRequirement and the state of the Actor.
         *
         * @param requirement The InteractionRequirement object containing the necessary requirements to be met.
         * @param actor_group The group of \link Actor Actors \endlink that this Interaction will take place in.
         * @param context The ContextType this Interaction will take place in.
         * @return The result of the check.
         */
        bool CheckRequirements(const InteractionRequirement &requirement, const std::list<Actor *> &actor_group, ContextType context) const;
        /**
         * @brief Calculates the chance an Actor to be picked for a participant slot.
         *
         * @param participant The participant for which we want to calculate the chance.
         * @param participant_id The participant id for the slot we are currently picking for.
         * @param requirement The relevant InteractionRequirement for this picking process.
         * @param tendency The relevant InteractionTendency for this picking process.
         * @param out_reason The potential Kernel that led to the calculated chance.
         * @return The chance of the Actor being picked.
         */
        float CalculateParticipantChance(const Actor *participant, size_t participant_id, const std::shared_ptr<InteractionRequirement> &requirement, const std::shared_ptr<InteractionTendency> &tendency, Kernel *&out_reason);

        /**
         * @brief Applies a change to the \link Actor Actor's \endlink wealth.
         *
         * @param reasons Vector holding the reasons for this change.
         * @param tick The tick during which this change happened.
         * @param value By how much the wealth gets changed.
         */
        void ApplyWealthChange(const std::vector<Kernel *> &reasons, size_t tick, float value);
        /**
         * @brief Applies a change to the \link Actor Actor's \endlink \link Emotion Emotional \endlink state.
         *
         * @param reasons Vector holding the reasons for this change.
         * @param tick The tick during which this change happened.
         * @param type_index The type index of the EmotionType of the Emotion that gets changed.
         * @param value By how much the Emotion gets changed.
         */
        void ApplyEmotionChange(const std::vector<Kernel *> &reasons, size_t tick, int type_index, float value);
        /**
         * @brief Applies changes to the \link Actor Actor's \endlink \link Relationship Relationships \endlink.
         *
         * @param reasons Vector holding the reasons for this change.
         * @param tick The tick during which this change happened.
         * @param actor_id The id of the Actor with which the Relationship gets changed..
         * @param change Vector with floats describing by how much each Relationship will be changed.
         */
        void ApplyRelationshipChange(const std::vector<Kernel *> &reasons, size_t tick, size_t actor_id, std::vector<float> change);
        /**
         * @brief Creates a string describing the current status of the Actor.
         *
         * @return The description string.
         */
        std::string GetDetailedDescriptionString() const;
        /**
         * @brief Returns a list of all other \link Actor Actors \endlink this Actor has some kind of Relationship with.
         *
         * @return The list of known \link Actor Actors \endlink.
         */
        const std::list<Actor *> &GetAllKnownActors() const;
        /**
         * @brief Returns a list of \link Actor Actors \endlink this Actor has the strongest Relationship with. The size of this list is determined by the Setting.
         *
         * This group of Actors will be used for Interaction that happen during the freetime of the Actor. This happens because it makes sense for the Actor
         * to interact with those Actors he has the strongest feelings for (be they negative or positive).
         * @return The list of \link Actor Actors \endlink.
         */
        std::list<Actor *> GetFreetimeActorGroup() const;
        /**
         * @brief Caluclate the strength of the Relationship for the passed Actor.
         *
         * @param actor_id The id of the Actor we want to determine the Relationship strength for.
         * @return The Relationship strength.
         */
        float CalculateRelationshipStrength(size_t actor_id) const;
        /**
         * @brief Checks wether the Actor has a Relationship with the passed Actor.
         *
         * @param actor_id The id of the Actor we want to check.
         * @return The result of the check.
         */
        bool HasRelationshipWith(size_t actor_id) const;

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
        /**
         * @brief Holds all other \link Actor Actors \endlink this Actor has some kind of Relationship with.
         */
        std::list<Actor *> known_actors_;
        /**
         * @brief Holds all the \link Actor Actors \endlink this Actor has the strongest Relationships with.
         * */
        std::list<Actor *> freetime_group;

        /**
         * @brief Private Constructor so only Chronicle can create Actors.
         *
         * @param school Reference to the School object the Actor is part of.
         * @param id The \link Actor Actor's \endlink id.
         * @param first_name The \link Actor Actor's \endlink first name.
         * @param last_name The \link Actor Actor's \endlink last name.
         */
        Actor(School &school, size_t id, std::string first_name, std::string last_name);
        /**
         * @brief Initializes the Wealth member with a random value.
         *
         * @param tick The tick during which this happens.
         */
        void InitializeRandomWealth(size_t tick);
        /**
         * @brief Initializes the Emotion member map with random values.
         *
         * @param tick The tick during which this happens.
         */
        void InitializeRandomEmotions(size_t tick);
        /**
         * @brief Initializes the Relationship member map with random values.
         *
         * @param tick The tick during which this happens.
         */
        void InitializeRandomRelationships(size_t tick);
        /**
         * @brief Initializes the the Goal member with a random value.
         *
         * @param tick The tick during which this happens.
         */
        void InitializeRandomGoal(size_t tick);

        /**
         * @brief Updates the Relationship of this Actor with another one.
         *
         * @param other_actor The Actor with which the Relationship was changed.
         * @param relationship The new Relationship values.
         * @param already_known If the other Actor was already known to this Actor.
         */
        void UpdateRelationship(Actor *other_actor, std::vector<Relationship *> relationship, bool already_known = false);
        /**
         * @brief Chronicle is a friend so private constructor can be accessed.
         */
        friend class Chronicle;
    };

} // namespace tattletale

template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of<tattletale::Actor, T>::value, char>> : fmt::formatter<std::string>
{
    // d - default = fullname, l - last name, s - second name, o - detail
    char presentation = 'd';

    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
    {
        auto it = ctx.begin(), end = ctx.end();
        if (it != end && (*it == 'f' || *it == 'l' || *it == 'd' || *it == 'o'))
            presentation = *it++;

        if (it != end && *it != '}')
            throw format_error("invalid format");

        return it;
    }

    template <typename FormatContext>
    auto format(tattletale::Actor &actor, FormatContext &ctx) -> decltype(ctx.out())
    {
        if (presentation == 'f')
        {
            return fmt::formatter<std::string>::format(actor.first_name_, ctx);
        }
        else if (presentation == 'l')
        {
            return fmt::formatter<std::string>::format(actor.last_name_, ctx);
        }
        else if (presentation == 'o')
        {
            return fmt::formatter<std::string>::format(actor.GetDetailedDescriptionString(), ctx);
        }
        return fmt::formatter<std::string>::format(actor.name_, ctx);
    }
};
#endif // TALE_ACTOR_H