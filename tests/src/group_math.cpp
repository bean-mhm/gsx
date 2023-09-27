#include "group_math.h"

// TEF
#include "tef/math/math.h"

// Internal
#include "test.h"

using namespace math;

inline bool eq_float(float a, float b)
{
    return math::abs(a - b) < epsilon;
}

template<typename T>
inline bool eq_vec(const T& a, const T& b)
{
    return (float)max_component(math::abs(a - b)) < epsilon;
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
    test::assert(eq_float(distance(vec2(-1, 0), vec2(1, 3)), 3.6055513f), "distance(vec2, vec2)");
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
    test::assert(eq_vec(pow(v, vec3(.5f)), sqrt(v)), "pow(vec3, 2)");
    test::assert(eq_vec(
        inversesqrt(v),
        vec3(.8543577f, .6900656f, .3333333f)
    ), "inversesqrt(vec3)");
    test::assert(eq_vec(sign(ivec3(-80, 50, 60)), ivec3(-1, 1, 1)), "sign(ivec3)");
    test::assert(eq_vec(clamp01(vec3(.2f, 2.f, -10.f)), vec3(.2f, 1.f, 0.f)), "clamp01(vec3)");
    test::assert(eq_float(
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

void test_group_math()
{
    test::start_group("math");
    test::run("vec2", test_vec2);
    test::run("vec3", test_vec3);
    test::end_group();
}
