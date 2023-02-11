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
        std::vector<std::vector<Uint32>>            heart_pal_;
        Sint                                        pal_ix_ = 0;
        std::vector<Uint8>                          heart_surface_;
        FP                                          fumes_ = 4.160;
        Sint                                        cx_, cy_;
//        std::vector<std::tuple<Sint, Sint>>         offset_;
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
            Sint N = 10;
            hearts_.reserve(N);
            // offset_.reserve(N);

            auto rad = 0.0;

            auto surface = SDL_GetWindowSurface(win_);

            for (auto i = 0; i < N; ++i)
            {
                auto ox = rand() % surface->w;
                auto oy = rand() % surface->h;;
                rad = 40.0 + rand() % 30;
                auto rad_min = rad - rand() % 30;
                auto rad_max = rad + rand() % 30;
                auto rad_n = -5 + rand() % 10;

                auto pi = M_PI2 * static_cast<FP>(rand()) / RAND_MAX;
                auto pi_min = pi - M_PI2 * (static_cast<FP>(rand()) / RAND_MAX);
                auto pi_max = pi + M_PI2 * (static_cast<FP>(rand()) / RAND_MAX);
                auto pi_n = -0.015625 * 5 + 0.015625 * static_cast<FP>(rand() % 10);

                hearts_.emplace_back(std::make_unique<heart_anim>(
                                        2880, 255, 5, ox, oy,
                                        pi, pi_n, pi_min, pi_max,
                                        rad, rad_n, rad_min, rad_max));
//                offset_.emplace_back(ox, oy);
            }
        }

        auto init_cxy()
        {
            auto surface = SDL_GetWindowSurface(win_);
            cx_ = surface->w / 2;
            cy_ = surface->h / 2;
        }

        auto init() -> void override
        {
            init_pal();
            init_heart_surface();
            init_black_surface();
            init_cxy();
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
                        // cx_ = e.motion.x;
                        // cy_ = e.motion.y;
                        break;

                    case SDL_MOUSEBUTTONUP:
                        if (e.button.button == SDL_BUTTON_RIGHT)
                            pal_ix_ = (pal_ix_ + 1) % heart_pal_.size();
                        break;

                    case SDL_MOUSEWHEEL:
                        fumes_ = fumes_ - (e.wheel.y * 0.01);
                        if (fumes_ > 4.5) fumes_ = 4.5;
                        else if (fumes_ < 3.9) fumes_ = 3.9;
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
            {
                h->animate(heart_surface_, surface->w, surface_size_);
            }

            // Add hell
            auto last_row = surface->w * (surface->h - 1);
            for (auto i = last_row; i < last_row + surface->w; ++i)
            {
                if (rand() % 100 >= 99)
                {
                    auto c = rand() % 255;
                    heart_surface_.at(i) = c;
                }
            }

            // Fire effect
            auto e = (surface->h * surface->w) - surface->w - 2;
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

                *(data + i) = heart_pal_.at(pal_ix_).at(new_c);
                heart_surface_.at(i) = new_c;
            }
        }
    };
}
