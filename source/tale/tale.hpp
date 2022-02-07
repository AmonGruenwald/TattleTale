/// @file tale.hpp For now just provides two basic functions for testing purposes.
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
#include "tale/actor.hpp"
/**
 * @brief Responsible for the event generation of the TattleTale project.
 * For now it just contains two basic functions for testing purposes.
 */
namespace tale
{
    /**
     * @brief Prints "Tale".
     *
     * Literally just prints "Tale".
     */
    void TalePrintExample();

    /**
     * @brief Returns 1.
     *
     * Literally always just returns 1.
     * @return The integer 1.
     */
    int TaleIntExample();
} // namespace tale
#endif // TALE_TALE_H