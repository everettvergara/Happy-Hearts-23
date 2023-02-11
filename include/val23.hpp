#pragma once

#include <memory>
#include <vector>
#include <boost/range/combine.hpp>

#include "video.hpp"
#include "pal.hpp"
#include "heart_anim.hpp"

namespace eg
{
    class val23 : public video 
    {
    private:
        std::vector<std::unique_ptr<heart_anim>>    hearts_;
        std::vector<Uint32>                         heart_pal_;
        std::vector<Uint8>                          heart_surface_;
        FP                                          fumes_ = 4.0625;

    public:

        auto init() -> void override
        {
            // Reset to black
            auto surface = SDL_GetWindowSurface(win_);
            SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 255));
            
            // Fire Palette
            // auto heart_pal = get_palette_gradient(
            //                     surface->format, {
            //                         {0,     SDL_MapRGBA(surface->format, 0, 0, 0, 255)},
            //                         {50,    SDL_MapRGBA(surface->format, 100, 0, 0, 255)},
            //                         {100,    SDL_MapRGBA(surface->format, 255, 174, 0, 255)},
            //                         {150,    SDL_MapRGBA(surface->format, 255, 255, 0, 255)},
            //                         {255,    SDL_MapRGBA(surface->format, 255, 255, 255, 255)},
            //                         });

            // Posh Pink Palette
            auto heart_pal = get_palette_gradient(
                                surface->format, {
                                    {0,     SDL_MapRGBA(surface->format, 0, 0, 0, 255)},
                                    {50,    SDL_MapRGBA(surface->format, 50, 0, 50, 255)},
                                    {100,    SDL_MapRGBA(surface->format, 255, 0, 100, 255)},
                                    {150,    SDL_MapRGBA(surface->format, 255, 255, 0, 255)},
                                    {255,    SDL_MapRGBA(surface->format, 255, 255, 255, 255)},
                                    });


            heart_pal_ = std::move(heart_pal.value());


            auto full_size = surface->w * surface->h;
            heart_surface_.resize(full_size, 0);

            hearts_.emplace_back(std::make_unique<heart_anim>(
                                    1440, 255, 5,
                                    1024 / 2, 768 / 2,
                                    0, 0.0625, -1.0, +1.0,
                                    40.0, 5, 25.0, 60.0));

            hearts_.emplace_back(std::make_unique<heart_anim>(
                                    1440, 255, 3,
                                    1024 / 2 + 1024 / 4, 768 / 3,
                                    0, 0.0625, -M_PI2, +M_PI2,
                                    40.0, -1, 10.0, 50.0));

            hearts_.emplace_back(std::make_unique<heart_anim>(
                                    1440, 255, 3,
                                    1024 / 2 + 1024 / 4, 2 * 768 / 3,
                                    0, -0.0625, -4.0, +4.0,
                                    10.0, +5, 10.0, 50.0));

        }


        auto event() -> bool override
        {
            SDL_Event e; 
            while(SDL_PollEvent(&e))
            { 
                switch (e.type)
                {
                    case SDL_QUIT: return false;
                    case SDL_MOUSEWHEEL:
                        fumes_ = fumes_ - (e.wheel.y * 0.01);
                        SDL_Log("%.6f", fumes_);
                }
            } 

            return true;
        }

        auto recalc() -> void override
        {
            // Get handle to surface
            auto surface = SDL_GetWindowSurface(win_);

            // Animate hearts
            for (auto &h : hearts_)
                h->animate(heart_surface_, surface->w);

            // Fire effect
            auto e = (surface->h * surface->w) - surface->w - surface->w;
            auto data = static_cast<Uint32 *>(surface->pixels);
            for (auto i = 0; i < e; ++i)
            {
                auto new_c =    static_cast<Sint>((
                                    heart_surface_.at(i + surface->w) + 
                                    heart_surface_.at(i + surface->w - 1) + 
                                    heart_surface_.at(i + surface->w + 2) + 
                                    heart_surface_.at(i + 1)
                                    ) / fumes_);
                
                if (new_c > 255) new_c = 255;
                else if(new_c < 0) new_c = 0;

                *(data + i) = heart_pal_.at(new_c);
                heart_surface_.at(i) = new_c;
            }
        }
    };
}
