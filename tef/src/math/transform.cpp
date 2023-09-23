#include "transform.h"

namespace tef::math
{

    std::ostream& operator<<(std::ostream& os, const transform_t& t)
    {
        os << t.to_string();
        return os;
    }

    std::string transform_t::to_string() const
    {
        return m.to_string();
    }

}
