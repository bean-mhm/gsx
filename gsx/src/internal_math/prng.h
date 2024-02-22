#pragma once

#include <cstdint>

#include "vec2.h"
#include "vec3.h"
#include "../internal_common/all.h"

namespace gsx::math
{

    // pseudo-random number generator
    class prng_t
    {
    public:
        prng_t();

        template<typename A>
            requires (sizeof(A) > 0 && sizeof(A) % sizeof(u32) == 0)
        prng_t(const A& seed)
        {
            init(seed);
        }

        template<typename A, typename B>
            requires (sizeof(A) > 0 && sizeof(A) % sizeof(u32) == 0
        && sizeof(B) > 0 && sizeof(B) % sizeof(u32) == 0)
            prng_t(const A& seed0, const B& seed1)
        {
            init(seed0, seed1);
        }

        no_copy_construct_no_assignment(prng_t);

        template<typename T,
            std::enable_if_t<std::is_same_v<T, i32>, bool> = true>
        T next()
        {
            return (i32)next<u32>();
        }

        template<typename T,
            std::enable_if_t<std::is_same_v<T, i32>, bool> = true>
        T next(T min, T max)
        {
            return min + (next<u32>() % ((u32)(max - min) + 1u));
        }

        template<typename T,
            std::enable_if_t<std::is_same_v<T, u32>, bool> = true>
        T next()
        {
            u32 s0 = state[0];
            u32 s1 = state[1];
            u32 result = rot(s0 * 0x9e3779bbu, 5u) * 5u;
            s1 ^= s0;
            state[0] = rot(s0, 26u) ^ s1 ^ (s1 << 9u);
            state[1] = rot(s1, 13u);
            return result;
        }

        template<typename T,
            std::enable_if_t<std::is_same_v<T, u32>, bool> = true>
        T next(T min, T max)
        {
            return min + (next<u32>() % (max - min + 1u));
        }

        template<typename T,
            std::enable_if_t<std::is_same_v<T, f32>, bool> = true>
        T next()
        {
            return (f32)next<u32>() / (f32)(0xffffffffu);
        }

        template<typename T,
            std::enable_if_t<std::is_same_v<T, f32>, bool> = true>
        T next(T min, T max)
        {
            return min + (next<f32>() * (max - min));
        }

        template<typename T,
            std::enable_if_t<std::is_same_v<T, i64>, bool> = true>
        T next()
        {
            return (i64)next<u64>();
        }

        template<typename T,
            std::enable_if_t<std::is_same_v<T, i64>, bool> = true>
        T next(T min, T max)
        {
            return min + (next<u64>() % ((u64)(max - min) + 1ull));
        }

        template<typename T,
            std::enable_if_t<std::is_same_v<T, u64>, bool> = true>
        T next()
        {
            return (u64)next<u32>() | ((u64)next<u32>() << 32ull);
        }

        template<typename T,
            std::enable_if_t<std::is_same_v<T, u64>, bool> = true>
        T next(T min, T max)
        {
            return min + (next<u64>() % (max - min + 1ull));
        }

        template<typename T,
            std::enable_if_t<std::is_same_v<T, f64>, bool> = true>
        T next()
        {
            return (f64)next<u64>() / (f64)(0xffffffffffffffffull);
        }

        template<typename T,
            std::enable_if_t<std::is_same_v<T, f64>, bool> = true>
        T next(T min, T max)
        {
            return min + (next<f64>() * (max - min));
        }

        // 2D point inside the unit circle
        template<std::floating_point T>
        base_vec2<T> next_in_circle()
        {
            base_vec2<T> v;
            for (i32 i = 0; i < 100; i++)
            {
                v = base_vec2<T>(
                    (T)2. * next<T>() - (T)1.,
                    (T)2. * next<T>() - (T)1.
                );
                if (length_squared(v) <= (T)1.) return v;
            }
            return v;
        }

        // 3D point inside the unit sphere
        template<std::floating_point T>
        base_vec3<T> next_in_sphere()
        {
            base_vec3<T> v;
            for (i32 i = 0; i < 100; i++)
            {
                v = base_vec3<T>(
                    (T)2. * next<T>() - (T)1.,
                    (T)2. * next<T>() - (T)1.,
                    (T)2. * next<T>() - (T)1.
                );
                if (length_squared(v) <= (T)1.) return v;
            }
            return v;
        }

        // 2D point on the unit circle, distanced 1 unit away from the origin
        template<std::floating_point T>
        base_vec2<T> next_on_circle()
        {
            vec2 v;
            f32 lensqr;
            for (i32 i = 0; i < 100; i++)
            {
                v = vec2(
                    (T)2. * next<T>() - (T)1.,
                    (T)2. * next<T>() - (T)1.
                );
                lensqr = length_squared(v);
                if (lensqr <= (T)1. && lensqr != (T)0.) break;
            }
            return v / sqrt(lensqr);
        }

        // 3D point on the unit sphere, distanced 1 unit away from the origin
        template<std::floating_point T>
        base_vec3<T> next_on_sphere()
        {
            vec3 v;
            f32 lensqr;
            for (i32 i = 0; i < 100; i++)
            {
                v = vec3(
                    (T)2. * next<T>() - (T)1.,
                    (T)2. * next<T>() - (T)1.,
                    (T)2. * next<T>() - (T)1.
                );
                lensqr = length_squared(v);
                if (lensqr <= (T)1. && lensqr != (T)0.) break;
            }
            return v / sqrt(lensqr);
        }

        // normally distributed 2D point centered around the origin (using the
        // Box-Muller Transform)
        // https://www.baeldung.com/cs/uniform-to-normal-distribution
        template<std::floating_point T>
        base_vec2<T> next_gauss()
        {
            T u1 = next<T>();
            T u2 = next<T>();
            T temp = sqrt((T)(-2.) * log(u1));
            return base_vec2<T>(
                temp * cos(tau<T> *u2),
                temp * sin(tau<T> *u2)
            );
        }

    private:
        u32 state[2];

        static u32 rot(u32 x, u32 k)
        {
            return (x << k) | (x >> (32u - k));
        }

        template<typename A>
            requires (sizeof(A) > 0 && sizeof(A) % sizeof(u32) == 0)
        void init(const A& seed)
        {
            static constexpr u32 prime0 = 7789;
            static constexpr u32 prime1 = 193;
            static constexpr u32 prime2 = 757;
            static constexpr u32 prime3 = 1310209793;
            static constexpr u32 prime4 = 423226613;
            static constexpr u32 prime5 = 522135209;
            static constexpr u32 prime6 = 720882293;

            u32 hash0 = 333223831;
            u32 hash1 = 888228317;
            for (usize i = 0; i < sizeof(A) / sizeof(u32); i++)
            {
                const u32 new_val =
                    prime0 + reinterpret_cast<const u32*>(&seed)[i];

                hash0 += prime1;
                hash1 += prime2;

                hash0 = (hash0 * prime3) ^ (new_val * prime4);
                hash1 = (hash1 * prime5) ^ (new_val * prime6);
            }

            next<u32>();
        }

        template<typename A, typename B>
            requires (sizeof(A) > 0 && sizeof(A) % sizeof(u32) == 0
        && sizeof(B) > 0 && sizeof(B) % sizeof(u32) == 0)
            void init(const A& seed0, const B& seed1)
        {
            static constexpr u32 prime0 = 7789;
            static constexpr u32 prime1 = 193;
            static constexpr u32 prime2 = 757;
            static constexpr u32 prime3 = 1310209793;
            static constexpr u32 prime4 = 423226613;
            static constexpr u32 prime5 = 522135209;
            static constexpr u32 prime6 = 720882293;

            u32 hash0 = 333223831;
            u32 hash1 = 888228317;
            for (usize i = 0; i < 2; i++)
            {
                const usize size =
                    ((i == 0) ? sizeof(A) : sizeof(B))
                    / sizeof(u32);

                for (usize j = 0; j < size; j++)
                {
                    const u32 new_val =
                        prime0
                        + (i == 0)
                        ? reinterpret_cast<const u32*>(&seed0)[j]
                        : reinterpret_cast<const u32*>(&seed1)[j];

                    hash0 += prime1;
                    hash1 += prime2;

                    hash0 = (hash0 * prime3) ^ (new_val * prime4);
                    hash1 = (hash1 * prime5) ^ (new_val * prime6);
                }
            }

            next<u32>();
        }

    };

}
