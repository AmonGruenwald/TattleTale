#ifndef TATTLE_CURATIONS_RARITYCURATION_H
#define TATTLE_CURATIONS_RARITYCURATION_H

#include "tattle/curations/curation.hpp"
#include "shared/random.hpp"

namespace tattletale
{
    class RarityCuration : public Curation
    {
    public:
        RarityCuration(size_t max_chain_size);
        float CalculateScore(const std::vector<Kernel *> &chain) const override;
        Kernel *GetFirstNoteworthyEvent(const std::vector<Kernel *> &chain) const override;
        Kernel *GetSecondNoteworthyEvent(const std::vector<Kernel *> &chain) const override;
    };
} // namespace tattletale
#endif // TATTLE_CURATIONS_RARITYCURATION_H