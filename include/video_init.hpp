#pragma once

#include <stdexcept>
#include <SDL.h>
#include <SDL_ttf.h>

namespace eg
{
    auto video_init()
    {
        if (auto t = SDL_Init(SDL_INIT_VIDEO); t < 0) 
            throw std::runtime_error("Could not init SDL Video!");

        
    }

    auto video_quit()
    {
        SDL_Quit();
    }
}
