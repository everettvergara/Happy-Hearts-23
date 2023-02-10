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
        // const Sint32 cx = 1024 / 2;
        // const Sint32 cy = 768 / 2;

        std::vector<std::unique_ptr<heart_anim>> hearts_;

        // FP rot = 0.0;
        // FP rot_n = 0.025;
        // FP rot_min = -1.0;
        // FP rot_max = +1.0;

        // FP rad = 40.0;
        // FP rad_n = 1.0;
        // FP rad_min = 25.0;
        // FP rad_max = 80.0;


        std::vector<Uint32> heart_pal_;
        std::vector<Uint8> heart_surface_;


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



        }

        auto update() -> void override
        {

            // Get handle to surface
            auto surface = SDL_GetWindowSurface(win_);

            // Animate hearts
            for (auto &h : hearts_)
                h->animate(heart_surface_, surface->w);


            // Add Hell
            // auto last_row = (surface->h - 2) * surface->w;
            // for (int i = 0; i < surface->w; ++i)
            // {
            //     heart_surface_.at(last_row + i) = rand() % 200;
            // }

            // // Add Hell Fire
            // int a = 12345, b = 1103515245, rnd = a;
            // for (int i = 0; i < 1000; ++i)
            // {
            //     heart_surface_.at(rnd % heart_surface_.size()) = rand() % 100;
            //     rnd = (b * rnd + 1);
            // }

            // Fire effect
            auto e = (surface->h * surface->w) - surface->w - surface->w;
            auto data = static_cast<Uint32 *>(surface->pixels);
            for (auto i = 0; i < e; ++i)
            {
                auto new_c =    (
                                heart_surface_.at(i + surface->w) + 
                                heart_surface_.at(i + surface->w - 1) + 
                                heart_surface_.at(i + surface->w + 2) + 
                                heart_surface_.at(i + 1)
                                ) / 4.03125;

                *(data + i) = heart_pal_.at(new_c);
                heart_surface_.at(i) = new_c;
            }
        }

    };

}
