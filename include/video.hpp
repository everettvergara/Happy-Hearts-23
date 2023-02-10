#pragma once

#include <stdexcept>
#include <SDL.h>

namespace eg
{

    constexpr int   FPS_    = 60;
    constexpr int   MSPF_   = 1000 / FPS_;

    class video
    {

    private:
        SDL_Window      *win_   = NULL;
        
    public:

        video()
        {
        }

        virtual ~video()
        {
            if (win_) SDL_DestroyWindow(win_);
        }

        auto create_win(const char *title, int w, int h)
        {
            win_    =   SDL_CreateWindow(
                            title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                            w, h, SDL_WINDOW_SHOWN);

            if (win_ == NULL) 
                throw std::runtime_error("Could not create window.");
        }

        virtual auto update() -> void
        {
        }

        auto run()
        {
            bool quit = false;



            do
            {
                auto start = SDL_GetTicks();

                SDL_Event e; 
                while(SDL_PollEvent(&e))
                { 
                    if (e.type == SDL_QUIT) 
                    {
                        quit = true;
                        break; 
                    }
                } 

                // Perform delay
                if (auto elapsed = SDL_GetTicks() - start;
                    elapsed < MSPF_) SDL_Delay(MSPF_ - elapsed);

            } while (not quit);
            
        }
    };

}
