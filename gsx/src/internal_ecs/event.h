#pragma once

#include <any>
#include <cstdint>

#include "../internal_common/all.h"

namespace gsx::ecs
{

    using event_type_t = u64;

    class event_t
    {
    public:
        event_type_t type;

        std::any data;

        event_t(event_type_t type, const std::any& data);

    };

}
