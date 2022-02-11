/// @file tale.hpp Collects everything that is necessary so user only have to include this one file.
#ifndef TALE_TALE_H
#define TALE_TALE_H
#include "tale/kernels/kernel.hpp"
#include "tale/kernels/action.hpp"
#include "tale/kernels/emotion.hpp"
#include "tale/kernels/goal.hpp"
#include "tale/kernels/interaction.hpp"
#include "tale/kernels/relationship.hpp"
#include "tale/kernels/resource.hpp"
#include "tale/kernels/trait.hpp"
#include "tale/globals/interactionstore.hpp"
#include "tale/globals/random.hpp"
#include "tale/actor.hpp"
#include "tale/school.hpp"
#include "tale/course.hpp"
#include "tale/setting.hpp"
/**
 * @brief Responsible for the event generation of the TattleTale project.
 */
namespace tale
{
    // TODO: should return events and needs to be parameterizable
    void Run(const Setting &setting);
} // namespace tale
#endif // TALE_TALE_H