#include "quaternion.h"

namespace tef::math
{

    std::ostream& operator<<(std::ostream& os, const quaternion_t& q)
    {
        os << q.to_string();
        return os;
    }

}
