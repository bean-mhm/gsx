#pragma once

// STD
#include <any>
#include <cstdint>

namespace tef::ecs
{

    // Event type
    using event_type_t = uint64_t;

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
