#pragma once

// STD
#include <any>
#include <cstdint>

// Internal
#include "../internal_common/all.h"

namespace gsx::ecs
{

    // Event type
    using event_type_t = u64;

    // Event
    struct event_t
    {
        // Event type specified by the sender
        event_type_t type;

        // Event data
        std::any data;

        event_t(event_type_t type, const std::any& data);
    };

}
