#pragma once 

#include "common.hpp"

namespace eg 
{
    class heart
    {
    
    private:
        const Sint r_         = 75;
        const Sint pts_       = 500;
        const Sint ini_burn_  = 255;
        const Sint distort_   = 10;

    public:

        heart(Sint r, Sint pts, Sint ini_burn, Sint distort)
            : r_(r), pts_(pts), ini_burn_(ini_burn), distort_(distort)
        {
        }
    };
}