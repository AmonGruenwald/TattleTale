/// @file tattle.hpp Access point for the curation side of the project.
#ifndef TATTLE_TATTLE_H
#define TATTLE_TATTLE_H
#include "shared/chronicle.hpp"
#include "shared/setting.hpp"

namespace tattletale
{
    /**
     * @brief Run a curation on the \link Kernel Kernels \endlink stored in the passed Chronicle.
     *
     * @param chronicle Chronicle object where \link Kernel Kernels \endlink are stored.
     * @param setting Contains all parameters used by the curation.
     */
    std::string Tattle(const Chronicle &chronicle, const Setting &setting);
} // namespace tattletale
#endif // TATTLE_TATTLE_H