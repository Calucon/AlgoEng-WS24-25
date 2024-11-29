#include "numberGen32.h"

AEPKSS::Util::NumberGenerator32::NumberGenerator32(uint32_t seed)
{
    this->index = this->permutate(permutate(seed) + 0x682f0161);
    this->intermediateOffset = this->permutate(permutate(seed + 1) + 0x46790905);
}

uint32_t AEPKSS::Util::NumberGenerator32::next()
{
    return this->permutate((this->permutate(this->index++) + this->intermediateOffset) ^ 0x5bf03635);
}

uint32_t AEPKSS::Util::NumberGenerator32::permutate(uint32_t x)
{
    if (x >= prime)
        return x; // Out of range - mapped to themselves

    uint32_t residue = ((size_t)x * x) % prime;
    return (x <= prime / 2) ? residue : prime - residue;
}