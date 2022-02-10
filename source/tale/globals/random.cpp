#include "tale/globals/random.hpp"
namespace tale
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
    uint32_t Random::GetInt(uint32_t min, uint32_t max)
    {
        std::uniform_int_distribution<uint32_t> distribution(min, max);
        return distribution(rng_);
    }
    float Random::GetFloat(float min, float max)
    {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(rng_);
    }
} // namespace tale