#ifndef TALE_ACTOR_H
#define TALE_ACTOR_H

#include <string>
#include <map>
#include <memory>
#include "tale/kernels/goal.hpp"
#include "tale/kernels/emotion.hpp"
#include "tale/kernels/relationship.hpp"
#include "tale/kernels/trait.hpp"
#include "tale/kernels/resource.hpp"

namespace tale
{
    /**
     * @brief Represents one person in the simulation.
     * An actor has to act accordingly to their internal state. So this class needs to contain everything neccessary.
     */
    class Actor
    {
    public:
        Actor();
        bool IsEnrolledInCourse(size_t course_id);
        void EnrollInCourse(size_t course_id);
        std::string name_;
        std::shared_ptr<Goal> goal_;
        std::vector<std::shared_ptr<Trait>> traits_;
        std::map<EmotionType, std::shared_ptr<Emotion>> emotions_;
        std::map<std::weak_ptr<Actor>, std::map<RelationshipType, std::shared_ptr<Relationship>>> relationships_;
        std::shared_ptr<Resource> resource_;

    private:
        std::vector<size_t> enrolled_courses_id_;
    };

} // namespace tale
#endif // TALE_ACTOR_H