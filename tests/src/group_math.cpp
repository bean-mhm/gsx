#include "group_math.h"

// STD
#include <array>

// GSX
#include "gsx/gsx.h"

// Internal
#include "test.h"

using namespace math;

inline bool eq_f32(f32 a, f32 b)
{
    return math::abs(a - b) < epsilon;
}

template<typename T>
inline bool eq_vec(const T& a, const T& b)
{
    return (f32)max_component(math::abs(a - b)) < epsilon;
}

inline bool eq_polar(const polar_t& a, const polar_t& b)
{
    return min(
        math::abs(a.r - b.r),
        math::abs(a.theta - b.theta)
    ) < epsilon;
}

inline bool eq_spherical(const spherical_t& a, const spherical_t& b)
{
    return min(
        min(
            math::abs(a.r - b.r),
            math::abs(a.theta - b.theta)
        ),
        math::abs(a.phi - b.phi)
    ) < epsilon;
}

template<i32 n_row, i32 n_col>
inline bool eq_mat(const base_mat<n_row, n_col>& m1, const base_mat<n_row, n_col>& m2)
{
    for (i32 row = 0; row < n_row; row++)
    {
        for (i32 col = 0; col < n_col; col++)
        {
            if (math::abs(m1(row, col) - m2(row, col)) >= epsilon)
                return false;
        }
    }
    return true;
}

static void test_vec2()
{
    vec2 v(1.37f, 2.1f);
    test::assert(uvec2(v) == uvec2(1, 2), "type conversion");
    test::assert(eq_vec(vec2(mat1x2({ 7, 3 })), vec2(7, 3)), "mat1x2 constructor");
    test::assert(eq_vec(vec2(mat2x1({ 7, 3 })), vec2(7, 3)), "mat2x1 constructor");
    test::assert(mat1x2(vec2(0, 2)) == mat1x2({ 0, 2 }), "mat1x2 cast");
    test::assert(mat2x1(vec2(0, 2)) == mat2x1({ 0, 2 }), "mat2x1 cast");
    test::assert(ivec2(3, 2).to_string() == "[3, 2]", "to_string()");
    test::assert(eq_vec(v + vec2(5), vec2(6.37f, 7.1f)), "vec2 + vec2");
    test::assert(eq_vec(v - vec2(.5f), vec2(.87f, 1.6f)), "vec2 - vec2");
    v += vec2(5.f, 6.5f);
    test::assert(eq_vec(v, vec2(6.37f, 8.6f)), "vec2 += vec2");
    v -= vec2(5.f, 6.5f);
    test::assert(eq_vec(v, vec2(1.37f, 2.1f)), "vec2 -= vec2");
    test::assert(eq_vec(v * 2.f, vec2(2.74f, 4.2f)), "vec2 * scalar");
    test::assert(eq_vec(v * vec2(2.f, 1.f), vec2(2.74f, 2.1f)), "vec2 * vec2");
    test::assert(eq_vec(v / .5f, vec2(2.74f, 4.2f)), "vec2 / scalar");
    test::assert(eq_vec(v / vec2(.5f, 1.f), vec2(2.74f, 2.1f)), "vec2 / vec2");
    v *= 2.f;
    test::assert(eq_vec(v, vec2(2.74f, 4.2f)), "vec2 *= scalar");
    v /= 2.f;
    test::assert(eq_vec(v, vec2(1.37f, 2.1f)), "vec2 /= scalar");
    v *= vec2(2.f, 1.f);
    test::assert(eq_vec(v, vec2(2.74f, 2.1f)), "vec2 *= vec2");
    v /= vec2(2.f, 1.f);
    test::assert(eq_vec(v, vec2(1.37f, 2.1f)), "vec2 /= vec2");
    test::assert(vec2(4, 3) == vec2(4, 3), "vec2 == vec2");
    test::assert(vec2(4, 3) != vec2(4, 2), "vec2 != vec2");
    test::assert(-vec2(4, 3) == vec2(-4, -3), "-vec2");
    test::assert(eq_vec(2.f + v, vec2(3.37f, 4.1f)), "scalar + vec2");
    test::assert(eq_vec(10.f - v, vec2(8.63f, 7.9f)), "scalar - vec2");
    test::assert(eq_vec(2.f * v, vec2(2.74f, 4.2f)), "scalar * vec2");
    test::assert(eq_vec(10.f / v, vec2(7.2992700f, 4.7619047f)), "scalar / vec2");
    test::assert(eq_vec(sin(v), vec2(.9799081f, .8632094f)), "sin(vec2)");
    test::assert(eq_vec(pow(v, vec2(2.f)), v * v), "pow(vec2, 2)");
    test::assert(eq_vec(inversesqrt(v), vec2(.8543577f, .6900656f)), "inversesqrt(vec2)");
    test::assert(eq_vec(sign(ivec2(-80, 50)), ivec2(-1, 1)), "sign(ivec2)");
    test::assert(eq_vec(clamp01(vec2(.2f, 2.f)), vec2(.2f, 1.f)), "clamp01(vec2)");
    test::assert(eq_f32(distance(vec2(-1, 0), vec2(1, 3)), 3.6055513f), "distance(vec2, vec2)");
    test::assert(eq_vec(
        reflect(normalize(vec2(1, -1)), vec2(0, 1)),
        vec2(.7071068f)
    ), "reflect(vec2, vec2)");
    test::assert(min_component_index(vec2(2, 5)) == 0, "min_component_index(vec2)");
    test::assert(min_component_index(vec2(5, 2)) == 1, "min_component_index(vec2)");
    test::assert(max_component_index(vec2(2, 5)) == 1, "max_component_index(vec2)");
    test::assert(max_component_index(vec2(5, 2)) == 0, "max_component_index(vec2)");
}

static void test_vec3()
{
    vec3 v(1.37f, 2.1f, 9.f);
    test::assert(uvec3(v) == uvec3(1, 2, 9), "type conversion");
    test::assert(eq_vec(vec3(mat1x3({ 7, 3, 12 })), vec3(7, 3, 12)), "mat1x3 constructor");
    test::assert(eq_vec(vec3(mat3x1({ 7, 3, 12 })), vec3(7, 3, 12)), "mat3x1 constructor");
    test::assert(mat1x3(vec3(0, 2, 4)) == mat1x3({ 0, 2, 4 }), "mat1x3 cast");
    test::assert(mat3x1(vec3(0, 2, 4)) == mat3x1({ 0, 2, 4 }), "mat3x1 cast");
    test::assert(ivec3(3, 2, 1).to_string() == "[3, 2, 1]", "to_string()");
    test::assert(eq_vec(v + vec3(5), vec3(6.37f, 7.1f, 14.f)), "vec3 + vec3");
    test::assert(eq_vec(v - vec3(.5f), vec3(.87f, 1.6f, 8.5f)), "vec3 - vec3");
    v += vec3(5.f, 6.5f, 1.f);
    test::assert(eq_vec(v, vec3(6.37f, 8.6f, 10.f)), "vec3 += vec3");
    v -= vec3(5.f, 6.5f, 1.f);
    test::assert(eq_vec(v, vec3(1.37f, 2.1f, 9.f)), "vec3 -= vec3");
    test::assert(eq_vec(v * 2.f, vec3(2.74f, 4.2f, 18.f)), "vec3 * scalar");
    test::assert(eq_vec(v * vec3(2.f, 1.f, 3.f), vec3(2.74f, 2.1f, 27.f)), "vec3 * vec3");
    test::assert(eq_vec(v / .5f, vec3(2.74f, 4.2f, 18.f)), "vec3 / scalar");
    test::assert(eq_vec(v / vec3(.5f, 1.f, 1.f), vec3(2.74f, 2.1f, 9.f)), "vec3 / vec3");
    v *= 2.f;
    test::assert(eq_vec(v, vec3(2.74f, 4.2f, 18.f)), "vec3 *= scalar");
    v /= 2.f;
    test::assert(eq_vec(v, vec3(1.37f, 2.1f, 9.f)), "vec3 /= scalar");
    v *= vec3(2.f, 1.f, 3.f);
    test::assert(eq_vec(v, vec3(2.74f, 2.1f, 27.f)), "vec3 *= vec3");
    v /= vec3(2.f, 1.f, 3.f);
    test::assert(eq_vec(v, vec3(1.37f, 2.1f, 9.f)), "vec3 /= vec3");
    test::assert(vec3(4, 3, 5) == vec3(4, 3, 5), "vec3 == vec3");
    test::assert(vec3(4, 3, 5) != vec3(4, 2, 5), "vec3 != vec3");
    test::assert(-vec3(4, 3, 0) == vec3(-4, -3, 0), "-vec3");
    test::assert(eq_vec(2.f + v, vec3(3.37f, 4.1f, 11.f)), "scalar + vec3");
    test::assert(eq_vec(10.f - v, vec3(8.63f, 7.9f, 1.f)), "scalar - vec3");
    test::assert(eq_vec(2.f * v, vec3(2.74f, 4.2f, 18.f)), "scalar * vec3");
    test::assert(eq_vec(10.f / v, vec3(7.2992700f, 4.7619047f, 1.1111111f)), "scalar / vec3");
    test::assert(eq_vec(sin(v), vec3(.9799081f, .8632094f, 0.4121185f)), "sin(vec3)");
    test::assert(eq_vec(pow(v, vec3(.5f)), sqrt(v)), "pow(vec3, .5)");
    test::assert(eq_vec(
        inversesqrt(v),
        vec3(.8543577f, .6900656f, .3333333f)
    ), "inversesqrt(vec3)");
    test::assert(eq_vec(sign(ivec3(-80, 50, 60)), ivec3(-1, 1, 1)), "sign(ivec3)");
    test::assert(eq_vec(clamp01(vec3(.2f, 2.f, -10.f)), vec3(.2f, 1.f, 0.f)), "clamp01(vec3)");
    test::assert(eq_f32(
        distance(vec3(-1, 0, 0), vec3(1, 3, 0))
        , 3.6055513f
    ), "distance(vec3, vec3)");
    test::assert(min_component_index(vec3(2, 5, 7)) == 0, "min_component_index(vec3)");
    test::assert(min_component_index(vec3(5, 2, 7)) == 1, "min_component_index(vec3)");
    test::assert(min_component_index(vec3(7, 5, 2)) == 2, "min_component_index(vec3)");
    test::assert(max_component_index(vec3(6, 2, 4)) == 0, "max_component_index(vec3)");
    test::assert(max_component_index(vec3(2, 6, 4)) == 1, "max_component_index(vec3)");
    test::assert(max_component_index(vec3(2, 4, 6)) == 2, "max_component_index(vec3)");
}

static void test_vec4()
{
    vec4 v(1.37f, 2.1f, 9.f, 1.f);
    test::assert(uvec4(v) == uvec4(1, 2, 9, 1), "type conversion");
    test::assert(eq_vec(vec4(mat1x4({ 7, 3, 2, -1 })), vec4(7, 3, 2, -1)), "mat1x4 constructor");
    test::assert(eq_vec(vec4(mat4x1({ 7, 3, 2, -1 })), vec4(7, 3, 2, -1)), "mat4x1 constructor");
    test::assert(mat1x4(vec4(0, 2, 4, 6)) == mat1x4({ 0, 2, 4, 6 }), "mat1x4 cast");
    test::assert(mat4x1(vec4(0, 2, 4, 6)) == mat4x1({ 0, 2, 4, 6 }), "mat4x1 cast");
    test::assert(ivec4(3, 2, 1, -5).to_string() == "[3, 2, 1, -5]", "to_string()");
    test::assert(eq_vec(v + vec4(5), vec4(6.37f, 7.1f, 14.f, 6.f)), "vec4 + vec4");
    test::assert(eq_vec(v - vec4(.5f), vec4(.87f, 1.6f, 8.5f, .5f)), "vec4 - vec4");
    v += vec4(5.f, 6.5f, 1.f, 1.f);
    test::assert(eq_vec(v, vec4(6.37f, 8.6f, 10.f, 2.f)), "vec4 += vec4");
    v -= vec4(5.f, 6.5f, 1.f, 1.f);
    test::assert(eq_vec(v, vec4(1.37f, 2.1f, 9.f, 1.f)), "vec4 -= vec4");
    test::assert(eq_vec(v * 2.f, vec4(2.74f, 4.2f, 18.f, 2.f)), "vec4 * scalar");
    test::assert(eq_vec(v * vec4(2.f, 1.f, 3.f, 0), vec4(2.74f, 2.1f, 27.f, 0)), "vec4 * vec4");
    test::assert(eq_vec(v / .5f, vec4(2.74f, 4.2f, 18.f, 2.f)), "vec4 / scalar");
    test::assert(eq_vec(v / vec4(.5f, 1.f, 1.f, 1), vec4(2.74f, 2.1f, 9.f, 1)), "vec4 / vec4");
    v *= 2.f;
    test::assert(eq_vec(v, vec4(2.74f, 4.2f, 18.f, 2.f)), "vec4 *= scalar");
    v /= 2.f;
    test::assert(eq_vec(v, vec4(1.37f, 2.1f, 9.f, 1.f)), "vec4 /= scalar");
    v *= vec4(2.f, 1.f, 3.f, 10.f);
    test::assert(eq_vec(v, vec4(2.74f, 2.1f, 27.f, 10.f)), "vec4 *= vec4");
    v /= vec4(2.f, 1.f, 3.f, 10.f);
    test::assert(eq_vec(v, vec4(1.37f, 2.1f, 9.f, 1.f)), "vec4 /= vec4");

    test::assert(vec4(4, 3, 5, 0) == vec4(4, 3, 5, 0), "vec4 == vec4");
    test::assert(vec4(4, 3, 5, 0) != vec4(4, 2, 5, 0), "vec4 != vec4");
    test::assert(-vec4(4, 3, 0, -4) == vec4(-4, -3, 0, 4), "-vec4");
    test::assert(eq_vec(2.f + v, vec4(3.37f, 4.1f, 11.f, 3.f)), "scalar + vec4");
    test::assert(eq_vec(10.f - v, vec4(8.63f, 7.9f, 1.f, 9.f)), "scalar - vec4");
    test::assert(eq_vec(2.f * v, vec4(2.74f, 4.2f, 18.f, 2.f)), "scalar * vec4");
    test::assert(eq_vec(10.f / v, vec4(7.2992700f, 4.7619047f, 1.1111111f, 10)), "scalar / vec4");
    test::assert(eq_vec(sin(v), vec4(.9799081f, .8632094f, 0.4121185f, 0.841471f)), "sin(vec4)");
    test::assert(eq_vec(pow(v, vec4(.5f)), sqrt(v)), "pow(vec4, .5)");
    test::assert(eq_vec(
        inversesqrt(v),
        vec4(.8543577f, .6900656f, .3333333f, 1.f)
    ), "inversesqrt(vec4)");
    test::assert(eq_vec(sign(ivec4(-80, 50, 60, 0)), ivec4(-1, 1, 1, 0)), "sign(ivec4)");
    test::assert(eq_vec(clamp01(vec4(.2f, 2.f, -10.f, 0)), vec4(.2f, 1, 0, 0)), "clamp01(vec4)");
    test::assert(eq_f32(
        distance(vec4(-1, 0, 0, 8), vec4(1, 3, 0, -18))
        , 26.2488095f
    ), "distance(vec4, vec4)");
    test::assert(min_component_index(vec4(2, 4, 6, 8)) == 0, "min_component_index(vec4)");
    test::assert(min_component_index(vec4(4, 2, 6, 8)) == 1, "min_component_index(vec4)");
    test::assert(min_component_index(vec4(4, 4, 2, 8)) == 2, "min_component_index(vec4)");
    test::assert(min_component_index(vec4(8, 4, 6, 2)) == 3, "min_component_index(vec4)");
    test::assert(max_component_index(vec4(8, -2, 4, 0)) == 0, "max_component_index(vec4)");
    test::assert(max_component_index(vec4(2, 17, 4, 0)) == 1, "max_component_index(vec4)");
    test::assert(max_component_index(vec4(8, -2, 40, 0)) == 2, "max_component_index(vec4)");
    test::assert(max_component_index(vec4(2, -2, 4, 30)) == 3, "max_component_index(vec4)");
}

static void test_bounds2()
{
    test::assert(
        ibounds2(ivec2(-1), ivec2(1)).to_string() == "[pmin=[-1, -1], pmax=[1, 1]]",
        "to_string()"
    );
    bounds2 b(vec2(-1), vec2(1));
    test::assert(eq_f32(b.area(), 4.f), "area()");
    test::assert(eq_vec(b.lerp(.5), vec2(0)), "lerp()");
    test::assert(eq_vec(b.offset_of(vec2(0)), vec2(.5)), "offset_of()");
    test::assert(
        union_(ibounds2(ivec2(-2), ivec2(0)), ivec2(3)) == ibounds2(ivec2(3), ivec2(-2)),
        "union_(bounds, point)"
    );
    test::assert(
        union_(
            ibounds2(ivec2(-2), ivec2(0)),
            ibounds2(ivec2(1), ivec2(4))
        ) == ibounds2(ivec2(-2), ivec2(4)),
        "union_(bounds, bounds)"
    );
    test::assert(
        intersect(
            ibounds2(ivec2(-2), ivec2(0)),
            ibounds2(ivec2(-1), ivec2(1))
        ) == ibounds2(ivec2(-1), ivec2(0)),
        "intersect(bounds, bounds)"
    );
    test::assert(overlaps(
        bounds2(vec2(0), vec2(3)),
        bounds2(vec2(2), vec2(4))
    ), "overlaps(bounds, bounds)");
    test::assert(inside(
        vec2(2),
        bounds2(vec2(0), vec2(3))
    ), "inside(point, bounds)");
}

static void test_bounds3()
{
    test::assert(
        ibounds3(ivec3(-1), ivec3(1)).to_string() == "[pmin=[-1, -1, -1], pmax=[1, 1, 1]]",
        "to_string()"
    );
    bounds3 b(vec3(-1), vec3(1));
    test::assert(eq_f32(b.volume(), 8.f), "volume()");
    test::assert(eq_vec(b.lerp(.5), vec3(0)), "lerp()");
    test::assert(eq_vec(b.offset_of(vec3(0)), vec3(.5)), "offset_of()");
    test::assert(
        union_(ibounds3(ivec3(-2), ivec3(0)), ivec3(3)) == ibounds3(ivec3(3), ivec3(-2)),
        "union_(bounds, point)"
    );
    test::assert(
        union_(
            ibounds3(ivec3(-2), ivec3(0)),
            ibounds3(ivec3(1), ivec3(4))
        ) == ibounds3(ivec3(-2), ivec3(4)),
        "union_(bounds, bounds)"
    );
    test::assert(
        intersect(
            ibounds3(ivec3(-2), ivec3(0)),
            ibounds3(ivec3(-1), ivec3(1))
        ) == ibounds3(ivec3(-1), ivec3(0)),
        "intersect(bounds, bounds)"
    );
    test::assert(overlaps(
        bounds3(vec3(0), vec3(3)),
        bounds3(vec3(2), vec3(4))
    ), "overlaps(bounds, bounds)");
    test::assert(inside(
        vec3(2),
        bounds3(vec3(0), vec3(3))
    ), "inside(point, bounds)");
}

static void test_polar()
{
    test::assert(eq_vec(
        polar_t(2.f, -pi / 6.f).cartesian(),
        vec2(1.7320508f, -1)
    ), "cartesian()");
    test::assert(eq_polar(
        polar_t(vec2(1.7320508f, -1)),
        polar_t(2.f, -pi / 6.f)
    ), "polar_t(vec2 cartesian)");
}

static void test_spherical()
{
    test::assert(eq_vec(
        spherical_t(.5f, 1.2f, 1.8f).cartesian(),
        vec3(-.1058806f, .4538320f, .1811789f)
    ), "cartesian()");
    test::assert(eq_spherical(
        spherical_t(vec3(-.1058806f, .4538320f, .1811789f)),
        spherical_t(.5f, 1.2f, 1.8f)
    ), "spherical_t(vec3 cartesian)");
}

static void test_matrix()
{
    test::assert(is_identity(mat4()), "is_identity(mat4)");
    test::assert(eq_mat(
        mat3({ 2, 2, 4, 4, 3, -1, 8, -1, 0 }) * mat3({ 10, 3, 2, -5, 5, -5, -1, -20, 1 }),
        mat3({ 6, -64, -2, 26, 47, -8, 85, 19, 21 })
    ), "mat3 * mat3");
    test::assert(eq_mat(
        base_mat<5, 2>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 })
        * base_mat<2, 3>({ 6, 5, 4, 3, 2, 1 }),
        base_mat<5, 3>({ 12, 9, 6, 30, 23, 16, 48, 37, 26, 66, 51, 36, 84, 65, 46 })
    ), "mat5x2 * mat2x3");
    test::assert(eq_mat(
        5 * mat2({ 1, 2, 3, 4 }),
        mat2({ 5, 10, 15, 20 })
    ), "scalar * mat2");
    test::assert(eq_f32(
        determinant(mat3({ 3, 5, 8, 7, 2.5f, 6, 4, -20, 1 })),
        -747.5f
    ), "determinant(mat3)");
    test::assert(eq_mat(
        inverse(mat3({ 3, 5, 8, 7, 2.5f, 6, 4, -20, 1 })),
        mat3(
            {
                -.1638800f, +.2207360f, -.0133779f,
                -.0227425f, +.0387960f, -.0508361f,
                +.2006690f, -.1070230f, +.0367893f
            }
        )
    ), "inverse(mat3)");
    test::assert(eq_mat(
        transpose(mat2x3({ 3, 5, 8, 7, 2.5f, 6 })),
        mat3x2({ 3, 7, 5, 2.5f, 8, 6 })
    ), "transpose(mat3)");
}

static void test_transform()
{
    test::assert(eq_vec(
        transform::apply_point_2d_h(
            transform::translate_2d_h(vec2(1, -4)),
            vec2(10, 20)
        ),
        vec2(11, 16)
    ), "translate_2d_h(), apply_point_2d_h()");
    mat4 inv;
    transform::translate_3d_h(vec3(-100), &inv);
    test::assert(eq_vec(
        transform::apply_point_3d_h(inv, vec3(2, 9, -2)),
        vec3(102, 109, 98)
    ), "translate_3d_h(), apply_point_3d_h()");
    test::assert(eq_vec(
        transform::apply_vector_3d(
            transform::scale_3d(vec3(10, 20, -40)),
            vec3(1, 2, -10)
        ),
        vec3(10, 40, 400)
    ), "scale3D(), apply_vector3D()");
    test::assert(eq_vec(
        transform::apply_point_2d_h(
            transform::translate_2d_h(vec2(100)) * transform::rotate_2d_h(-pi / 6.f),
            vec2(10, 20)
        ),
        vec2(118.6602554f, 112.3205109f)
    ), "rotate_2d_h(), translate_2d_h(), apply_point_2d_h()");
}

static void test_prng()
{
    prng_t prng;

    for (usize i = 0; i < 100; i++)
    {
        i32 a = prng.next_i32(-100, 100);
        test::assert(a >= -100 && a <= 100, std::format("next_i32(-100, 100) = {}", a));

        u32 b = prng.next_u32(10, 50);
        test::assert(b >= 10 && b <= 50, std::format("next_u32(10, 50) = {}", b));
    }

    std::array<u32, 100> hist;
    hist.fill(0);
    for (usize i = 0; i < 1000000; i++)
    {
        f32 num = prng.next_f32();
        u32 category = (u32)math::floor(num * 99.9999f);
        hist[category]++;
    }
    for (usize i = 0; i < 100; i++)
    {
        u32 count = hist[i] / 100;
        test::assert(count > 95 && count < 105, "next_f32() distribution");
    }
}

void test_group_math()
{
    test::start_group("math");
    test::run("vec2", test_vec2);
    test::run("vec3", test_vec3);
    test::run("vec4", test_vec4);
    test::run("bounds2", test_bounds2);
    test::run("bounds3", test_bounds3);
    test::run("polar", test_polar);
    test::run("spherical", test_spherical);
    test::run("matrix", test_matrix);
    test::run("transform", test_transform);
    test::run("prng", test_prng);
    test::end_group();
}
