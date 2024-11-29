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
    class NumberGenerator32
    {
    private:
        uint32_t index;
        uint32_t intermediateOffset;
        /**
         * Largest Prime that fits into a uint32_t (18446744073709551557UL);
         */
        const uint32_t prime = 4294967291U;
        /**
         * Perform a permutation to get to the next number
         */
        uint32_t permutate(uint32_t x);

    public:
        NumberGenerator32(uint32_t seed);
        /**
         * Returns the next random number
         */
        uint32_t next();
    };
}