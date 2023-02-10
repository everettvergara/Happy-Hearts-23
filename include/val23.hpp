#pragma once

#include <vector>
#include "video.hpp"

namespace eg
{
    struct point
    {
        Sint32 x, y;
    };

    class val23 : public video 
    {
    private:
        const Sint32 cx = 1024 / 2;
        const Sint32 cy = 768 / 2;
        const Sint32 heart_r_  = 300;
        const Sint32 heart_points_ = 360 * 10;
        std::vector<point> heart_;

    public:

        val23()
        {
        }

        ~val23()
        {
        }

        auto init() -> void override
        {
            auto surface = SDL_GetWindowSurface(win_);
            SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 255));

            heart_.reserve(heart_points_);
            
            const auto pi2 = M_PI * 2.0;
            const auto inc = pi2 / heart_points_;

            for (auto i = 0.0; i < pi2; i += inc)
            {
                auto r = 2.0 - 2.0 * SDL_sin(i) + SDL_sin(i) * SDL_sqrt(SDL_abs(SDL_cos(i))) / (SDL_sin(i) + 1.4);
                auto x = cx + r * heart_r_ * SDL_cos(i);
                auto y = cy + r * heart_r_ * SDL_sin(i);
                heart_.emplace_back(x, y);
            }
        }

        auto update() -> void override
        {
        }

    };

}
