#include "mat4.h"

namespace tef::math
{

    std::string mat4::to_string() const
    {
        std::ostringstream s("");
        s << "[";
        for (int i = 0; i < 4; ++i)
        {
            s << (i > 0 ? " [ " : "[ ");
            for (int j = 0; j < 4; ++j)
            {
                s << str::from_number(m[i][j]);
                if (j != 3)
                    s << "  ";
            }
            s << (i < 3 ? " ]\n" : " ]]");
        }
        return s.str();
    }

    std::ostream& operator<<(std::ostream& os, const mat4& m)
    {
        os << m.to_string();
        return os;
    }

}
