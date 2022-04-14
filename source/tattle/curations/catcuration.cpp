
#include "tattle/curations/catcuration.hpp"
#include <algorithm>

namespace tattletale
{

    CatCuration::CatCuration(size_t max_chain_size) : Curation("Cat", max_chain_size) {}
    float CatCuration::CalculateScore(const std::vector<Kernel *> &chain) const
    {
        return 1;
    }
    Kernel *CatCuration::GetFirstNoteworthyEvent(const std::vector<Kernel *> &chain) const
    {
        return chain[0];
    }
    Kernel *CatCuration::GetSecondNoteworthyEvent(const std::vector<Kernel *> &chain) const
    {
        return chain[chain.size() - 1];
    }

} // namespace tattletale