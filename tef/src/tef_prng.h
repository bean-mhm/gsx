#pragma once

// STD
#include <random>
#include <cstdint>

// Internal
#include "tef_utils.h"

namespace tef
{

    // Pseudo-Random Number Generator
    class prng_t
    {
    public:
        prng_t();
        no_copy_move_constructor(prng_t);

        int32_t next_i32();
        int32_t next_i32(int32_t min, int32_t max);

        uint32_t next_u32();
        uint32_t next_u32(uint32_t min, uint32_t max);

        int64_t next_i64();
        int64_t next_i64(int64_t min, int64_t max);

        uint64_t next_u64();
        uint64_t next_u64(uint64_t min, uint64_t max);

        float next_float();
        float next_float(float min, float max);

        double next_double();
        double next_double(double min, double max);

    private:
        // Engine
        std::random_device device;
        std::mt19937 engine;

        // Distributors
        std::uniform_int_distribution<int32_t> dist_i32;
        std::uniform_int_distribution<uint32_t> dist_u32;
        std::uniform_int_distribution<int64_t> dist_i64;
        std::uniform_int_distribution<uint64_t> dist_u64;
        std::uniform_real_distribution<float> dist_float;
        std::uniform_real_distribution<double> dist_double;

    };

}
