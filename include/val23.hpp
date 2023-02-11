#pragma once

#include <memory>
#include <vector>
#include <SDL_ttf.h>

#include "video.hpp"
#include "pal.hpp"
#include "heart_anim.hpp"

namespace eg
{

    constexpr size_t random_hearts = 5;
    constexpr Sint particle_size = 2880;
    constexpr Sint init_burn = 255;
    constexpr Sint burn_at = 20;
    constexpr FP fumes_def = 4.16;
    constexpr FP fumes_min = 3.9;
    constexpr FP fumes_max = 4.9;
    constexpr FP fumes_inc = 0.01;

    class val23 : public video 
    {
    
    private:
        std::vector<std::unique_ptr<heart_anim>>    hearts_;
        std::vector<std::vector<Uint32>>            heart_pal_;
        Sint                                        pal_ix_ = 0;
        std::vector<Uint8>                          heart_surface_;
        FP                                          fumes_ = fumes_def;
        int                                         surface_size_;

    public:

        auto init_pal()
        {
            auto surface = SDL_GetWindowSurface(win_);

            // Posh Pink Palette
            heart_pal_.emplace_back(
                get_palette_gradient(surface->format, { {0,     SDL_MapRGBA(surface->format, 0, 0, 0, 255)},
                                                        {50,    SDL_MapRGBA(surface->format, 50, 0, 50, 255)},
                                                        {100,   SDL_MapRGBA(surface->format, 255, 0, 100, 255)},
                                                        {150,   SDL_MapRGBA(surface->format, 255, 255, 0, 255)},
                                                        {255,   SDL_MapRGBA(surface->format, 255, 255, 255, 255)},
                                                        }).value());

            // Fire Palette
            heart_pal_.emplace_back(
                get_palette_gradient(surface->format, { {0,     SDL_MapRGBA(surface->format, 0, 0, 0, 255)},
                                                        {50,    SDL_MapRGBA(surface->format, 100, 0, 0, 255)},
                                                        {100,   SDL_MapRGBA(surface->format, 255, 175, 100, 255)},
                                                        {150,   SDL_MapRGBA(surface->format, 255, 255, 0, 255)},
                                                        {255,   SDL_MapRGBA(surface->format, 255, 255, 255, 255)},
                                                        }).value());

            // Icy Blue
            heart_pal_.emplace_back(
                get_palette_gradient(surface->format, { {0,     SDL_MapRGBA(surface->format, 0, 0, 0, 255)},
                                                        {50,    SDL_MapRGBA(surface->format, 0, 0, 100, 255)},
                                                        {100,   SDL_MapRGBA(surface->format, 0, 175, 255, 255)},
                                                        {150,   SDL_MapRGBA(surface->format, 0, 255, 255, 255)},
                                                        {255,   SDL_MapRGBA(surface->format, 255, 255, 255, 255)},
                                                        }).value());

        }

        auto init_heart_surface()
        {
            auto surface = SDL_GetWindowSurface(win_);
            surface_size_ = surface->w * surface->h;
            heart_surface_.resize(surface_size_, 0);
        }

        auto init_black_surface()
        {
            auto surface = SDL_GetWindowSurface(win_);
            SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 255));
        }

        auto init_random_hearts()
        {
            hearts_.reserve(random_hearts);
            auto surface = SDL_GetWindowSurface(win_);
            for (size_t i = 0; i < random_hearts; ++i)
            {
                auto &hanim = hearts_.emplace_back(std::make_unique<heart_anim>(particle_size, init_burn));
                hanim->random_spawn(surface->w, surface->h);
            }
        }

        auto init() -> void override
        {
            init_pal();
            init_heart_surface();
            init_black_surface();
            init_random_hearts();
        }

        auto event() -> bool override
        {
            SDL_Event e; 
            while(SDL_PollEvent(&e))
            { 
                switch (e.type)
                {
                    case SDL_QUIT: return false;
                    
                    case SDL_MOUSEMOTION:
                        break;

                    case SDL_MOUSEBUTTONUP:
                        if (e.button.button == SDL_BUTTON_RIGHT)
                            pal_ix_ = (pal_ix_ + 1) % heart_pal_.size();
                        break;

                    case SDL_MOUSEWHEEL:
                        fumes_ = fumes_ - (e.wheel.y * fumes_inc);
                        if (fumes_ > fumes_max) fumes_ = fumes_max;
                        else if (fumes_ < fumes_min) fumes_ = fumes_min;
                        break;
                }
            } 

            return true;
        }

        auto recalc() -> void override
        {
            // Get handle to surface
            auto surface = SDL_GetWindowSurface(win_);

            // Animate hearts
            for (const auto &h: hearts_)
                h->animate(heart_surface_, surface->w, surface->h, surface_size_);

            // Add hell
            auto last_row = surface->w * (surface->h - 1);
            for (auto i = last_row; i < last_row + surface->w; ++i)
            {
                if (rand() % 100 >= burn_at)
                {
                    auto c = 200 + rand() % 56;
                    heart_surface_.at(i) = c;
                }
            }

            // Fire effect
            auto data = static_cast<Uint32 *>(surface->pixels);
            for (auto i = 0; i < surface_size_; ++i)
            {
                auto new_c =    static_cast<Sint>((
                                    heart_surface_.at((i + surface->w) % surface_size_) + 
                                    heart_surface_.at((i + surface->w - 1) % surface_size_) + 
                                    heart_surface_.at((i + surface->w + 2) % surface_size_) + 
                                    heart_surface_.at((i + 1) % surface_size_)
                                    ) / fumes_);
                
                if (new_c > 255) new_c = 255;
                else if(new_c < 0) new_c = 0;

                *(data + i) = heart_pal_.at(pal_ix_).at(new_c);
                heart_surface_.at(i) = new_c;
            }
        }
    };
}
