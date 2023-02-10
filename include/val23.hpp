#pragma once

#include <vector>
#include <boost/range/combine.hpp>

#include "video.hpp"
#include "pal.hpp"
#include "common.hpp"
#include "heart.hpp"

namespace eg
{
    class val23 : public video 
    {
    private:
        const Sint32 cx = 1024 / 2;
        const Sint32 cy = 768 / 2;

        heart heart_;
        FP rot = 0.0;
        std::vector<Uint32> heart_pal_;
        std::vector<Uint8> heart_surface_;


    inline auto pset(SDL_Surface *surface, int x, int y, Uint32 c)
    {
        auto data = static_cast<Uint32 *>(surface->pixels);
        *(data + surface->w * y + x) = c;
    }

    inline auto pset(SDL_Surface *surface, int ix, Uint32 c)
    {
        auto data = static_cast<Uint32 *>(surface->pixels);
        *(data + ix) = c;
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
            
            // Generate Palette
            auto heart_pal = get_palette_gradient(
                                surface->format, {
                                    {0,     SDL_MapRGBA(surface->format, 0, 0, 0, 255)},
                                    {50,    SDL_MapRGBA(surface->format, 100, 0, 0, 255)},
                                    {100,    SDL_MapRGBA(surface->format, 255, 174, 0, 255)},
                                    {150,    SDL_MapRGBA(surface->format, 255, 255, 0, 255)},
                                    {255,    SDL_MapRGBA(surface->format, 255, 255, 255, 255)},
                                    });
            heart_pal_ = std::move(heart_pal.value());


            auto full_size = surface->w * surface->h;
            heart_surface_.resize(full_size, 0);
            heart_.recalc(surface->w, cx, cy, 75.0, rot);
            for (const auto [ix, c] : boost::combine(heart_.get_heart(), heart_.get_col()))
                heart_surface_.at(ix) = c;
        }

        auto update() -> void override
        {

            // Get handle to surface
            auto surface = SDL_GetWindowSurface(win_);


            rot += 0.05;
            heart_.recalc(surface->w, cx, cy, 75.0, rot);
            for (const auto [ix, c] : boost::combine(heart_.get_heart(), heart_.get_col()))
                heart_surface_.at(ix) = c;

            // Fire effect
            auto e = (surface->h * surface->w) - surface->w - surface->w;
            auto data = static_cast<Uint32 *>(surface->pixels);
            for (auto i = 0; i < e; ++i)
            {
                auto new_c =    (
                                heart_surface_.at(i + surface->w) + 
                                heart_surface_.at(i + surface->w - 1) + 
                                heart_surface_.at(i + surface->w + 2) + 
                                heart_surface_.at(i + surface->w + surface->w)
                                ) / 4.0625;

                *(data + i) = heart_pal_.at(new_c);
                heart_surface_.at(i) = new_c;
            }
            
        }

    };

}
