#pragma once

#include <vector>
#include "video.hpp"
#include "pal.hpp"

namespace eg
{
    // struct point
    // {
    //     Uint32 ix;
    //     Uint8 c;
    // };

    class val23 : public video 
    {
    private:
        const Sint32 cx = 1024 / 2;
        const Sint32 cy = 768 / 2;
        const Sint32 heart_r_  = 75;
        const Sint32 heart_points_ = 360 * 10;
        const Sint32 init_burn_ = 125;
        
        std::vector<Uint32> heart_;
        std::vector<Uint32> heart_pal_;
        std::vector<Uint8> heart_surface_;


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
            // Reset to black
            auto surface = SDL_GetWindowSurface(win_);
            SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 255));
            
            // Full Heart Surface
            auto full_size = surface->w * surface->h;
            heart_surface_.resize(full_size, 0);

            // Generate Heart Pixels
            heart_.reserve(heart_points_ + 1);
            
            const auto pi2 = M_PI * 2.0;
            const auto inc = pi2 / heart_points_;

            for (auto i = 0.0, ctr = 0.0; i < pi2; i += inc, ctr = ctr + 1)
            {
                auto cosi = SDL_cos(i);
                auto abs_cosi = cosi < 0 ? -cosi : cosi;
                auto r = 2.0 - 2.0 * SDL_sin(i) + SDL_sin(i) * SDL_sqrt(abs_cosi) / (SDL_sin(i) + 1.4);
                auto x = cx + r * heart_r_ * SDL_cos(i);
                auto y = cy - r * heart_r_ * SDL_sin(i);
                auto c = 255 - rand() % init_burn_;
                
                auto ix = y * surface->w + x;

                heart_.emplace_back(ix);
                heart_surface_.at(ix) = c;
            }

            auto heart_pal = get_palette_gradient(
                                surface->format, {
                                    {0,     SDL_MapRGBA(surface->format, 0, 0, 0, 255)},
                                    {50,    SDL_MapRGBA(surface->format, 255, 60, 0, 255)},
                                    {100,    SDL_MapRGBA(surface->format, 255, 174, 0, 255)},
                                    {150,    SDL_MapRGBA(surface->format, 255, 255, 0, 255)},
                                    {255,    SDL_MapRGBA(surface->format, 255, 255, 255, 255)},
                                    });
            
            heart_pal_ = std::move(heart_pal.value());
        }

        auto update() -> void override
        {
            // Get handle to surface
            auto surface = SDL_GetWindowSurface(win_);

            // Update Heart Surface
            for (auto ix : heart_)
                heart_surface_.at(ix) = 255 - rand() % init_burn_;

            // Fire effect
            auto e = surface->h * surface->w - surface->w - surface->w;
            auto data = static_cast<Uint32 *>(surface->pixels);
            for (auto i = 0; i < e; ++i)
            {

                auto new_c =    (
                                heart_surface_.at(i + surface->w) + 
                                heart_surface_.at(i + surface->w - 1) + 
                                heart_surface_.at(i + surface->w + 2) + 
                                heart_surface_.at(i + surface->w + surface->w)
                                ) / 4.125;

                *(data + i) = heart_pal_.at(new_c);
                heart_surface_.at(i) = new_c;
            }
            
        }

    };

}
