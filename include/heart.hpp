#pragma once 

#include <vector>
#include "common.hpp"

namespace eg 
{
    constexpr auto M_PI2 = M_PI * 2.0;

    class heart
    {
    
    private:
        
        Sint                pts_;
        Sint                ini_burn_;
        const FP            inc_;

        std::vector<Sint>   heart_;
        std::vector<Uint>   heart_col_;
        std::vector<FP>     heart_r_cache_;
        std::vector<FP>     heart_cos_cache_;
        std::vector<FP>     heart_sin_cache_;

    public:

        heart(const Sint pts = 720, const Sint ini_burn = 255)
            : pts_(pts), ini_burn_(ini_burn), inc_(M_PI2 / pts)
        {
            heart_.resize(pts + 1, 0);
            heart_col_.resize(pts + 1, 0);
            heart_r_cache_.reserve(pts + 1);
            heart_cos_cache_.reserve(pts + 1);
            heart_sin_cache_.reserve(pts + 1);

            // Compute for R of heart
            for (auto i = 0.0; i < M_PI2; i += inc_)
            {
                auto cosi = SDL_cos(i);
                auto abs_cosi = cosi < 0 ? -cosi : cosi;
                auto r = 2.0 - 2.0 * SDL_sin(i) + SDL_sin(i) * SDL_sqrt(abs_cosi) / (SDL_sin(i) + 1.4);

                heart_r_cache_.emplace_back(r);
                heart_cos_cache_.emplace_back(SDL_cos(i));
                heart_sin_cache_.emplace_back(SDL_sin(i));
            }
        }

        auto get_pts() const -> Sint 
        {
            return pts_;
        }

        auto get_heart() const -> const std::vector<Sint> &
        {
            return heart_;
        }

        auto get_col() const -> const std::vector<Uint> &
        {
            return heart_col_;
        }

        auto recalc(int w, int size, FP rad, FP rot, Sint cx, Sint cy, Sint distort)
        {
            auto last_row = size - w;
            for (auto i = 0; i < pts_; ++i)
            {
                auto x = heart_cos_cache_.at(i) * SDL_cos(rot) - heart_sin_cache_.at(i) * SDL_sin(rot);
                auto y = heart_sin_cache_.at(i) * SDL_cos(rot) + heart_cos_cache_.at(i) * SDL_sin(rot);
                
                auto nx = static_cast<Sint>(cx + rad * heart_r_cache_.at(i) * x);
                auto ny = static_cast<Sint>(cy - rad * heart_r_cache_.at(i) * y);

                nx -= distort + rand() % (1 + distort * 2);
                ny -= distort + rand() % (1 + distort * 2);

                auto ix = w * ny + nx;

                heart_.at(i) = ix; 
                
                if (heart_.at(i) < 0)
                {
                    heart_.at(i) = 0;
                    heart_col_.at(i) = 0;

                } else if (heart_.at(i) >= size)
                {
                    heart_.at(i) = last_row + (ix % w) - (w * (1 + rand() % 10));
                    heart_col_.at(i) = 255;

                } else 
                {
                    heart_col_.at(i) = 255 - rand() % ini_burn_;
                }

            }
        }
    };
}