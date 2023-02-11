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
        Sint                distort_;
        const FP            inc_;

        std::vector<Uint>   heart_;
        std::vector<Uint>   heart_col_;
        std::vector<FP>     heart_r_cache_;
        std::vector<FP>     heart_cos_cache_;
        std::vector<FP>     heart_sin_cache_;

    public:

        heart(const Sint pts = 720, const Sint ini_burn = 255, const Sint distort = 5)
            : pts_(pts), ini_burn_(ini_burn), distort_(distort), inc_(M_PI2 / pts)
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

        auto get_heart() const -> const std::vector<Uint> &
        {
            return heart_;
        }

        auto get_col() const -> const std::vector<Uint> &
        {
            return heart_col_;
        }

        auto recalc(int w, int size, Sint cx, Sint cy, FP rad, FP rot)
        {
            for (auto i = 0; i < pts_; ++i)
            {
                auto x = heart_cos_cache_.at(i) * SDL_cos(rot) - heart_sin_cache_.at(i) * SDL_sin(rot);
                auto y = heart_sin_cache_.at(i) * SDL_cos(rot) + heart_cos_cache_.at(i) * SDL_sin(rot);
                
                auto nx = static_cast<Sint>(cx + rad * heart_r_cache_.at(i) * x);
                auto ny = static_cast<Sint>(cy - rad * heart_r_cache_.at(i) * y);

                nx -= distort_ + rand() % (1 + distort_ * 2);
                ny -= distort_ + rand() % (1 + distort_ * 2);

                auto ix = w * ny + nx;

                heart_.at(i) = ix; 
                heart_.at(i) = heart_.at(i) >= size ? 0 : heart_.at(i);
                heart_col_.at(i) = 255 - rand() % ini_burn_;
            }
        }
    };
}