#ifndef TATTLE_CURATIONS_CATCURATION_H
#define TATTLE_CURATIONS_CATCURATION_H

#include "tattle/curations/curation.hpp"

namespace tattletale
{
    class CatCuration : public Curation
    {
    public:
        CatCuration(size_t max_chain_size);
        float CalculateScore(const std::vector<Kernel *> &chain) const override;
        std::string GenerateScoreDescription(float score) const override;
        Kernel *GetFirstNoteworthyEvent(const std::vector<Kernel *> &chain) const override;
        Kernel *GetSecondNoteworthyEvent(const std::vector<Kernel *> &chain) const override;
    };
} // namespace tattletale
#endif // TATTLE_CURATIONS_CATCURATION_H