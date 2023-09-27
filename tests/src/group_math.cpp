#include "group_math.h"

// TEF
#include "tef/math/math.h"

// Internal
#include "test.h"

inline bool eq_float(float a, float b)
{
    return math::abs(a - b) < math::epsilon;
}

template<typename T>
inline bool eq_vec(const T& a, const T& b)
{
    return math::max_component(math::abs(a - b)) < math::epsilon;
}

static void test_vec2()
{
    math::vec2 v(1.372f, 2.1f);

    test::assert(eq_vec(2.f * v, math::vec2(2.744f, 4.2f)), "scalar * vec2");
    test::assert(eq_vec(v * 2.f, math::vec2(2.744f, 4.2f)), "vec2 * scalar");
}

void test_group_math()
{
    test::start_group("math");

    test::run("vec2", test_vec2);

    test::end_group();
}
