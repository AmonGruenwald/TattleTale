#include "tattle/curations/raritycuration.hpp"
#include <algorithm>

namespace tattletale
{

    RarityCuration::RarityCuration(size_t max_chain_size) : Curation("Rarity", max_chain_size) {}

    float RarityCuration::CalculateScore(const std::vector<Kernel *> &chain) const
    {
        float score = 0.0f;
        for (auto &kernel : chain)
        {
            score += (1 - kernel->GetChance());
        }
        score /= static_cast<float>(max_chain_size_);
        return score;
    }
    Kernel *RarityCuration::GetFirstNoteworthyEvent(const std::vector<Kernel *> &chain) const
    {
        float highest_chance = 0.0f;
        Kernel *highest_kernel = nullptr;
        for (auto &kernel : chain)
        {
            float chance = kernel->GetChance();
            // skipping likely kernels
            if (chance >= 0.5f)
            {
                continue;
            }
            if (chance >= highest_chance)
            {
                highest_chance = chance;
                highest_kernel = kernel;
            }
        }
        return highest_kernel;
    }
    Kernel *RarityCuration::GetSecondNoteworthyEvent(const std::vector<Kernel *> &chain) const
    {
        float lowest_chance = 1.0f;
        Kernel *lowest_kernel = nullptr;
        for (auto &kernel : chain)
        {
            float chance = kernel->GetChance();
            // skipping likely kernels
            if (chance >= 0.5f)
            {
                continue;
            }
            if (chance <= lowest_chance)
            {
                lowest_chance = chance;
                lowest_kernel = kernel;
            }
        }
        return lowest_kernel;
    }

} // namespace tattletale