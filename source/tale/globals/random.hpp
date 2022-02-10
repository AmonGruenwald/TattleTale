#ifndef TALE_GLOBALS_RANDOM_H
#define TALE_GLOBALS_RANDOM_H

#include <random>
namespace tale
{
    class Random
    {
    public:
        Random();
        Random(uint32_t seed);
        void Seed(uint32_t seed);
        uint32_t GetInt(uint32_t min, uint32_t max);
        float GetFloat(float min, float max);

    private:
        std::mt19937 rng_;
    };
} // namespace tale
#endif // TALE_GLOBALS_RANDOM_H