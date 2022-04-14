
#include "tattle/curations/absoluteinterestcuration.hpp"
#include <algorithm>

namespace tattletale
{

    AbsoluteInterestCuration::AbsoluteInterestCuration(size_t max_chain_size) : Curation("Absolute Interest", max_chain_size) {}
    float AbsoluteInterestCuration::CalculateScore(const std::vector<Kernel *> &chain) const
    {
        float max_interactions = ceil(static_cast<float>(max_chain_size_) / 2.0f);
        size_t score = 0.0f;
        for (const auto &kernel : chain)
        {
            score += kernel->GetAbsoluteInterestScore();
        }
        if (score == 7)
        {
            int aslkdf = 0;
        }
        // TODO: don't use 4 magic number (max possible score)
        return std::clamp(score / max_interactions / 4.0f, 0.0f, 1.0f);
    }

    Kernel *AbsoluteInterestCuration::GetFirstNoteworthyEvent(const std::vector<Kernel *> &chain) const
    {
        // TODO: define this globally
        size_t lowest_score = 5;
        Kernel *lowest_kernel = nullptr;
        for (auto &kernel : chain)
        {
            size_t score = kernel->GetAbsoluteInterestScore();
            if (score == 0)
            {
                continue;
            }
            if (score < lowest_score)
            {
                lowest_score = score;
                lowest_kernel = kernel;
            }
        }
        return lowest_kernel;
    }
    Kernel *AbsoluteInterestCuration::GetSecondNoteworthyEvent(const std::vector<Kernel *> &chain) const
    {
        size_t highest_score = 0;
        Kernel *highest_kernel = nullptr;
        for (auto &kernel : chain)
        {
            size_t score = kernel->GetAbsoluteInterestScore();
            if (score == 0)
            {
                continue;
            }
            if (score >= highest_score)
            {
                highest_score = score;
                highest_kernel = kernel;
            }
        }
        return highest_kernel;
    }

} // namespace tattletale