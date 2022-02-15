#ifndef TALE_ACTOR_H
#define TALE_ACTOR_H

#include <string>
#include <map>
#include <memory>
#include "tale/setting.hpp"
#include "tale/globals/random.hpp"
#include "tale/globals/interactionstore.hpp"
#include "tale/kernels/goal.hpp"
#include "tale/kernels/emotion.hpp"
#include "tale/kernels/relationship.hpp"
#include "tale/kernels/trait.hpp"
#include "tale/kernels/resource.hpp"

namespace tale
{
    class School;
    /**
     * @brief Represents one person in the simulation.
     * An actor has to act accordingly to their internal state. So this class needs to contain everything neccessary.
     */
    class Actor : public std::enable_shared_from_this<Actor>
    {
    public:
        const size_t id_;
        std::string name_;
        std::shared_ptr<Goal> goal_;
        std::vector<std::shared_ptr<Trait>> traits_;
        std::map<EmotionType, std::shared_ptr<Emotion>> emotions_;
        std::map<size_t, std::map<RelationshipType, std::shared_ptr<Relationship>>> relationships_;
        std::shared_ptr<Resource> resource_;

        Actor(School &school, size_t id);
        bool IsEnrolledInCourse(size_t course_id) const;
        void EnrollInCourse(size_t course_id, uint32_t slot);
        size_t GetFilledSlotsCount() const;
        bool AllSlotsFilled() const;
        bool SlotsEmpty(const std::vector<uint32_t> &slots) const;
        std::string ChooseInteraction(const std::vector<std::weak_ptr<Actor>> &course_group, std::vector<std::weak_ptr<Kernel>> &out_reasons, std::vector<std::weak_ptr<Actor>> &out_participants);
        void ApplyResourceChange(std::vector<std::weak_ptr<Kernel>> reasons, size_t tick, float value);
        void ApplyEmotionChange(std::vector<std::weak_ptr<Kernel>> reasons, size_t tick, EmotionType type, float value);
        void ApplyRelationshipChange(std::vector<std::weak_ptr<Kernel>> reasons, size_t tick, size_t actor_id, RelationshipType type, float value);

    private:
        Random &random_;
        School &school_;
        InteractionStore &interaction_store_;
        const Setting &setting_;
        size_t filled_slots_count_ = 0;
        std::vector<int> enrolled_courses_id_;
    };

} // namespace tale
#endif // TALE_ACTOR_H