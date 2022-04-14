
#include "tattle/curations/tagcuration.hpp"
#include <algorithm>

namespace tattletale
{

    TagCuration::TagCuration(size_t max_chain_size) : Curation("Tag", max_chain_size) {}
    float TagCuration::CalculateScore(const std::vector<Kernel *> &chain) const
    {
        return 1;
    }
    Kernel *TagCuration::GetFirstNoteworthyEvent(const std::vector<Kernel *> &chain) const
    {
        return chain[0];
    }
    Kernel *TagCuration::GetSecondNoteworthyEvent(const std::vector<Kernel *> &chain) const
    {
        return chain[chain.size() - 1];
    }

} // namespace tattletale