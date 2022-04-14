#ifndef TATTLE_CURATIONS_RANDOMCURATION_H
#define TATTLE_CURATIONS_RANDOMCURATION_H

#include "tattle/curations/curation.hpp"
#include "shared/random.hpp"

namespace tattletale
{
    class RandomCuration : public Curation
    {
    public:
        RandomCuration(size_t max_chain_size, Random &random);
        float CalculateScore(const std::vector<Kernel *> &chain) const override;
        Kernel *GetFirstNoteworthyEvent(const std::vector<Kernel *> &chain) const override;
        Kernel *GetSecondNoteworthyEvent(const std::vector<Kernel *> &chain) const override;

    private:
        Random &random_;
    };
} // namespace tattletale
#endif // TATTLE_CURATIONS_RANDOMCURATION_H