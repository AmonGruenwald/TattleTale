#ifndef TALE_GLOBALS_RANDOM_H
#define TALE_GLOBALS_RANDOM_H

#include <random>
#include <vector>
namespace tattletale
{
    /**
     * @brief Represents one person in the simulation.
     * An Actor decides how to act each tick of the simulation. To do this it needs to hold all information necessary to come to a decision.
     */
    class Random
    {
    public:
        /**
         * @brief Constructor seeding with a default value of 123456789.
         *
         */
        Random();
        /**
         * @brief Constructor seeding with the passed value.
         *
         * @param seed The seed that is to be used.
         */
        Random(uint32_t seed);

        /**
         * @brief Function to reseed the object.
         *
         * This brings the object in the same state it would have been in if it was constructed using the passed seed.
         * @param seed The seed that is to be used.
         */
        void Seed(uint32_t seed);

        /**
         * @brief Getter for a random integer.
         *
         * Gets a random integer between the passed bounds (inclusive).
         * @param min The lower bound for the returned value (inclusive).
         * @param max The uppper bound for the returned value (inclusive).
         * @return The random integer
         */
        int GetInt(int min, int max);
        /**
         * @brief Getter for a random unsigned integer.
         *
         * Gets a random unsigned integer between the passed bounds (inclusive).
         * @param min The lower bound for the returned value (inclusive).
         * @param max The uppper bound for the returned value (inclusive).
         * @return The random unsigned integer
         */
        uint32_t GetUInt(uint32_t min, uint32_t max);
        /**
         * @brief Getter for a random float.
         *
         * Gets a random float between the passed bounds (inclusive).
         * @param min The lower bound for the returned value (inclusive).
         * @param max The uppper bound for the returned value (inclusive).
         * @return The random float
         */
        float GetFloat(float min, float max);

        /**
         * @brief Getter for a random index according to a probability distribution.
         *
         * Returns a random index for the passed vector, picked according to the chances stored in said vector (between 0.0f and 1.0f).
         *If all passed values are zero a division through zero happens, because of that we have to check for that case. As this is quite
         * a costly operation though, the user has to turn that check on manually if they know it could happen with the bool parameter.
         * If it is turned on, all zeroes would mean every index has the same chance to be picked.
         * @param probability_distribution The vector in which the chances for each index are stored.
         * @param support_all_zeroes Flag wether the function has to check if every chance is zero.
         * @return The random index
         */
        size_t PickIndex(const std::vector<float> &probability_distribution, bool support_all_zeroes = false);
        /**
         * @brief Shuffles the elements of the passed vector.
         *
         * @param [out] out_vector The vector that is to be shuffled
         */
        void Shuffle(std::vector<uint32_t> &out_vector);

    private:
        /**
         * @brief Random number generator engine. Using Mersenne Twister.
         */
        std::mt19937 rng_;
    };
} // namespace tattletale
#endif // TALE_GLOBALS_RANDOM_H