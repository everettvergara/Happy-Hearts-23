#pragma once

#include <vector>
#include "video.hpp"
#include "pal.hpp"

namespace eg
{
    struct point
    {
        Sint32 x, y;
        Uint32 c;
    };

    class val23 : public video 
    {
    private:
        const Sint32 cx = 1024 / 2;
        const Sint32 cy = 768 / 2;
        const Sint32 heart_r_  = 300;
        const Sint32 heart_points_ = 360 * 10;
        
        std::vector<point> heart_;
        std::vector<Uint32> heart_pal_;


    inline auto pset(SDL_Surface *surface, int x, int y, Uint32 c)
    {
        auto data = static_cast<Uint32 *>(surface->pixels);
        *(data + surface->w * y + surface->w + x) = c;
    }


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

            heart_.reserve(heart_points_ + 1);
            
            const auto pi2 = M_PI * 2.0;
            const auto inc = pi2 / heart_points_;

            for (auto i = 0.0, ctr = 0.0; i < pi2; i += inc, ctr = ctr + 1)
            {
                auto r = 2.0 - 2.0 * SDL_sin(i) + SDL_sin(i) * SDL_sqrt(SDL_abs(SDL_cos(i))) / (SDL_sin(i) + 1.4);
                auto x = cx + r * heart_r_ * SDL_cos(i);
                auto y = cy + r * heart_r_ * SDL_sin(i);
                heart_.emplace_back(x, y, 255);
            }
            // auto heart_pal = get_palette_gradient(
            //                     surface->format, {
            //                         {0,     SDL_MapRGBA(surface->format, 255, 255, 255, 255)},
            //                         {50,    SDL_MapRGBA(surface->format, 255, 255, 0, 255)},
            //                         {100,    SDL_MapRGBA(surface->format, 255, 174, 0, 255)},
            //                         {150,    SDL_MapRGBA(surface->format, 255, 60, 0, 255)},
            //                         {255,    SDL_MapRGBA(surface->format, 0, 0, 0, 255)},
            //                         });
            
            // heart_pal_ = std::move(heart_pal.value());
        }

        auto update() -> void override
        {
            // Get handle to surface
            auto surface = SDL_GetWindowSurface(win_);

            // Pset Heart to buff
            // for (const auto [x, y, c] : heart_)
            // {
            //     pset(surface, x, y, 255);
            // }
        }

    };

}
