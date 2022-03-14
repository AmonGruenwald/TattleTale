/// @file tattle.hpp For now just provides two basic functions for testing purposes.
#ifndef TATTLE_TATTLE_H
#define TATTLE_TATTLE_H
#include "shared/chronicle.hpp"
#include "shared/setting.hpp"
/**
#include "shared/chronicle.hpp"
#include "shared/setting.hpp"
 * @brief Responsible for the curation of the TattleTale project.
 * For now it just contains two basic functions for testing purposes.
 */
namespace tattletale
{
    std::string Tattle(const Chronicle &chronicle, const Setting &setting);
} // namespace tattletale
#endif // TATTLE_TATTLE_H