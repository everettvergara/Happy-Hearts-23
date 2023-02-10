#pragma once 

#include <vector>
#include "common.hpp"

namespace eg 
{
    constexpr auto M_PI2 = M_PI * 2.0;

    class heart
    {
    
    private:
        
        Sint                r_;
        Sint                pts_;
        Sint                ini_burn_;
        Sint                distort_;
        const FP            inc_;

        std::vector<Uint>   heart_;
        std::vector<FP>     heart_r_cache_;
        std::vector<FP>     heart_cos_cache_;
        std::vector<FP>     heart_sin_cache_;

    public:

        heart(Sint r, Sint pts, Sint ini_burn, Sint distort)
            : r_(r), pts_(pts), ini_burn_(ini_burn), distort_(distort), inc_(M_PI2 / pts)
        {
            heart_.reserve(pts + 1);
            heart_r_cache_.reserve(pts + 1);
            heart_cos_cache_.reserve(pts + 1);
            heart_sin_cache_.reserve(pts + 1);

            // Compute for R of heart
            for (auto i = 0.0, ctr = 0.0; i < M_PI2; i += inc_)
            {
                auto cosi = SDL_cos(i);
                auto abs_cosi = cosi < 0 ? -cosi : cosi;
                auto r = 2.0 - 2.0 * SDL_sin(i) + SDL_sin(i) * SDL_sqrt(abs_cosi) / (SDL_sin(i) + 1.4);

                heart_r_cache_.emplace_back(r);
                heart_cos_cache_.emplace_back(SDL_cos(i));
                heart_sin_cache_.emplace_back(SDL_sin(i));
            }
        }

        auto recalc(Sint cx, Sint cy, FP rot)
        {
        
        }
    };
}