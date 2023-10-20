#include "event.h"

namespace gsx::ecs
{

    event_t::event_t(event_type_t type, const std::any& data)
        : type(type), data(data)
    {}

}
