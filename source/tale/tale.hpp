/// @file tale.hpp Access point for the simulation side of the project.
#ifndef TALE_TALE_H
#define TALE_TALE_H
#include "shared/kernels/kernel.hpp"
#include "shared/kernels/goal.hpp"
#include "shared/kernels/interactions/interaction.hpp"
#include "shared/kernels/interactions/interactionprototype.hpp"
#include "shared/kernels/interactions/interactionrequirement.hpp"
#include "shared/kernels/interactions/interactioncontexttype.hpp"
#include "shared/kernels/resourcekernels/resource.hpp"
#include "shared/kernels/resourcekernels/emotion.hpp"
#include "shared/kernels/resourcekernels/relationship.hpp"
#include "shared/random.hpp"
#include "shared/chronicle.hpp"
#include "shared/setting.hpp"
#include "tale/interactionstore.hpp"
#include "shared/actor.hpp"
#include "tale/school.hpp"
#include "tale/course.hpp"
/**
 * @brief Namespace for the TattleTale project.
 */
namespace tattletale
{
    /**
     * @brief Run the whole simulation.
     *
     * @param chronicle Chronicle object where Kernels get written to. Allows sharing of events with the curation part.
     * @param random Random object that is to be used by the simulation. This allows repeatability.
     * @param setting Contains all parameters used by the simulation.
     */
    void Tale(Chronicle &chronicle, Random &random, const Setting &setting);
} // namespace tattletale
#endif // TALE_TALE_H