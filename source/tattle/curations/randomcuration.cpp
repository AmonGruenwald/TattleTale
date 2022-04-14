
#include "tattle/curations/randomcuration.hpp"
#include <algorithm>

namespace tattletale
{

    RandomCuration::RandomCuration(size_t max_chain_size) : Curation("Random", max_chain_size) {}
    float RandomCuration::CalculateScore(const std::vector<Kernel *> &chain) const
    {
        return 1;
    }
    std::string RandomCuration::GenerateScoreDescription(float score) const
    {
        return "";
    }
    Kernel *RandomCuration::GetFirstNoteworthyEvent(const std::vector<Kernel *> &chain) const
    {
        return chain[0];
    }
    Kernel *RandomCuration::GetSecondNoteworthyEvent(const std::vector<Kernel *> &chain) const
    {
        return chain[chain.size() - 1];
    }

} // namespace tattletale