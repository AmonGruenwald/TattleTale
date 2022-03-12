#include "shared/random.hpp"
namespace tattletale
{
    Random::Random()
    {
        uint32_t default_seed = 123456789;
        Seed(123456789);
    }
    Random::Random(uint32_t seed)
    {
        Seed(seed);
    }
    void Random::Seed(uint32_t seed)
    {
        rng_.seed(seed);
    }
    int Random::GetInt(int min, int max)
    {
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(rng_);
    }
    uint32_t Random::GetUInt(uint32_t min, uint32_t max)
    {
        std::uniform_int_distribution<uint32_t> distribution(min, max);
        return distribution(rng_);
    }
    float Random::GetFloat(float min, float max)
    {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(rng_);
    }
    size_t Random::PickIndex(const std::vector<float> &probability_distribution, bool support_all_zeroes)
    {
        if (support_all_zeroes)
        {
            bool all_zeros = true;
            for (auto value : probability_distribution)
            {
                if (value != 0)
                {
                    all_zeros = false;
                }
            }
            if (all_zeros)
            {
                std::vector<float> new_probability_distribution(probability_distribution.size(), 1.0f);
                std::discrete_distribution<size_t> new_distribution(new_probability_distribution.begin(), new_probability_distribution.end());
                return new_distribution(rng_);
            }
        }
        std::discrete_distribution<size_t> distribution(probability_distribution.begin(), probability_distribution.end());
        return distribution(rng_);
    }
} // namespace tattletale