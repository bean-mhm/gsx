#include "tef_utils.h"

namespace tef
{
    const char* cstr_from_bool(bool v)
    {
        return v ? "true" : "false";
    }

    float elapsed_sec(std::chrono::steady_clock::time_point t_start)
    {
        return 1e-9f * std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - t_start).count();
    }

    float elapsed_sec(
        std::chrono::steady_clock::time_point t_start,
        std::chrono::steady_clock::time_point t_end
    )
    {
        return 1e-9f * std::chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start).count();
    }

}
