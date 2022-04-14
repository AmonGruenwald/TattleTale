#ifndef TATTLE_CURATIONS_ABSOLUTEINTERESTCURATION_H
#define TATTLE_CURATIONS_ABSOLUTEINTERESTCURATION_H

#include "tattle/curations/curation.hpp"

namespace tattletale
{
    class AbsoluteInterestCuration : public Curation
    {
    public:
        AbsoluteInterestCuration(size_t max_chain_size);
        float CalculateScore(const std::vector<Kernel *> &chain) const override;
        Kernel *GetFirstNoteworthyEvent(const std::vector<Kernel *> &chain) const override;
        Kernel *GetSecondNoteworthyEvent(const std::vector<Kernel *> &chain) const override;
    };
} // namespace tattletale
#endif // TATTLE_CURATIONS_ABSOLUTEINTERESTCURATION_H