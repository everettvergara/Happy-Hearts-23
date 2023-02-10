#pragma once 

#include <vector>
#include "common.hpp"

namespace eg 
{
    class heart
    {
    
    private:
        
        Sint                x_;
        Sint                y_;
        Sint                r_;
        Sint                pts_;
        Sint                ini_burn_;
        Sint                distort_;

        std::vector<Uint>   heart_;

    public:

        heart(SDL_Surface *s, Sint x, Sint y, Sint r, Sint pts, Sint ini_burn, Sint distort)
            : r_(r), pts_(pts), ini_burn_(ini_burn), distort_(distort)
        {
            heart_.reserve(pts + 1);

            const auto pi2 = M_PI * 2.0;
            const auto inc = pi2 / pts_;

            for (auto i = 0.0, ctr = 0.0; i < pi2; i += inc, ctr = ctr + 1)
            {
                auto cosi = SDL_cos(i);
                auto abs_cosi = cosi < 0 ? -cosi : cosi;
                auto r = 2.0 - 2.0 * SDL_sin(i) + SDL_sin(i) * SDL_sqrt(abs_cosi) / (SDL_sin(i) + 1.4);
                auto x = static_cast<Sint32>(cx + r * heart_r_ * SDL_cos(i));
                auto y = static_cast<Sint32>(cy - r * heart_r_ * SDL_sin(i));

                x = x - heart_distort_ + rand() % (1 + heart_distort_ * 2);
                y = y - heart_distort_ + rand() % (1 + heart_distort_ * 2);

                auto c = 255 - rand() % init_burn_;
                
                auto ix = surface->w * y + x;
                heart_.emplace_back(ix);
                heart_surface_.at(ix) = c;
            }


        }


    };
}