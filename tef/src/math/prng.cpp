#include "prng.h"

namespace tef::math
{

    prng_t::prng_t()
        : device(),
        engine(device()),
        dist_i32(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()),
        dist_u32(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max()),
        dist_i64(std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max()),
        dist_u64(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max()),
        dist_float(0.f, 1.f),
        dist_double(0., 1.)
    {}

    int32_t prng_t::next_i32()
    {
        return dist_i32(engine);
    }

    int32_t prng_t::next_i32(int32_t min, int32_t max)
    {
        int32_t count = (max - min) + 1;
        return min + (((next_i32() % count) + count) % count);
    }

    uint32_t prng_t::next_u32()
    {
        return dist_u32(engine);
    }

    uint32_t prng_t::next_u32(uint32_t min, uint32_t max)
    {
        uint32_t count = (max - min) + 1;
        return min + (((next_u32() % count) + count) % count);
    }

    int64_t prng_t::next_i64()
    {
        return dist_i64(engine);
    }

    int64_t prng_t::next_i64(int64_t min, int64_t max)
    {
        int64_t count = (max - min) + 1;
        return min + (((next_i64() % count) + count) % count);
    }

    uint64_t prng_t::next_u64()
    {
        return dist_u64(engine);
    }

    uint64_t prng_t::next_u64(uint64_t min, uint64_t max)
    {
        uint64_t count = (max - min) + 1;
        return min + (((next_u64() % count) + count) % count);
    }

    float prng_t::next_float()
    {
        return dist_float(engine);
    }

    float prng_t::next_float(float min, float max)
    {
        return min + (next_float() * (max - min));
    }

    double prng_t::next_double()
    {
        return dist_double(engine);
    }

    double prng_t::next_double(double min, double max)
    {
        return min + (next_double() * (max - min));
    }

}
