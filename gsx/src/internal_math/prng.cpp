#include "prng.h"

#include <random>

#include "utils.h"

namespace gsx::math
{

    prng_t::prng_t()
    {
        std::random_device rd;
        std::uniform_int_distribution<u64> dist(0, UINT64_MAX);
        init(dist(rd));
    }

}
