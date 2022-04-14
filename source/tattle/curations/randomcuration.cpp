
#include "tattle/curations/randomcuration.hpp"
#include <algorithm>

namespace tattletale
{

    RandomCuration::RandomCuration(size_t max_chain_size, Random &random) : Curation("Random", max_chain_size), random_(random) {}
    float RandomCuration::CalculateScore(const std::vector<Kernel *> &chain) const
    {
        return random_.GetFloat(0.0f, 1.0f);
    }
    Kernel *RandomCuration::GetFirstNoteworthyEvent(const std::vector<Kernel *> &chain) const
    {
        return chain[random_.GetUInt(0, chain.size() - 1)];
    }
    Kernel *RandomCuration::GetSecondNoteworthyEvent(const std::vector<Kernel *> &chain) const
    {
        return chain[random_.GetUInt(0, chain.size() - 1)];
    }

} // namespace tattletale