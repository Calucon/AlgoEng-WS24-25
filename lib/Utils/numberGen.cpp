#include "numberGen.h"

AEPKSS::Util::NumberGenerator::NumberGenerator(uint64_t seed)
{
    this->index = this->permutate(permutate(seed) + 0x12C640B5F2829DEUL);
    this->index = this->permutate(permutate(seed) + 0x135B61464343UL);
}

uint64_t AEPKSS::Util::NumberGenerator::next()
{
    return this->permutate((this->permutate(this->index++) + this->intermediateOffset) ^ 0x6D2E37F602FUL);
}

uint64_t AEPKSS::Util::NumberGenerator::permutate(uint64_t x)
{
    if (x >= prime)
        return x; // Out of range - mapped to themselves

    // TODO: what happens on overflow?
    uint64_t residue = (x * x) % prime;
    return (x <= prime / 2) ? residue : prime - residue;
}