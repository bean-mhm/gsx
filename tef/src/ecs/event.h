#pragma once

// STD
#include <any>

// Internal
#include "utils.h"

namespace tef::ecs
{

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
