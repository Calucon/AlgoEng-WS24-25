#include <cstdlib>
#include <chrono>
#include <random>

using namespace std;

namespace AEPKSS::Util
{
    /**
     * Random number generator based on prime permutations
     * @see https://preshing.com/20121224/how-to-generate-a-sequence-of-unique-random-integers/
     */
    class NumberGenerator
    {
    private:
        uint64_t index;
        uint64_t intermediateOffset;
        /**
         * Largest Prime that fits into a uint64_t (18446744073709551557UL);
         */
        const uint64_t prime = 0xFFFFFFFFFFFFFFC5UL;
        /**
         * Perform a permutation to get to the next number
         */
        uint64_t permutate(uint64_t x);

    public:
        NumberGenerator(uint64_t seed);
        /**
         * Returns the next random number
         */
        uint64_t next();
    };
}