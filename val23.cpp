#include <iostream>
#include <SDL.h>

namespace eg
{

    class video_init
    {
    public:
        video_init()
        {
            if (auto t = SDL_Init(SDL_INIT_VIDEO); t < 0) 
                throw std::runtime_error("Could not init SDL Video!");
        }
        ~video_init()
        {
            SDL_Quit();
        }
    };

    class video
    {
    private:
        SDL_Window *win_;


        auto throw_if_winnull(SDL_Window *test) -> SDL_Window *
        {
            if (test == NULL) 
                throw std::runtime_error("Could not create window");

            return test;
        }

    public:

        video(const char *title, int w, int h)
            : win_(throw_if_winnull(
                    SDL_CreateWindow(
                        title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN)))
        {
        
        }
    };

}

using eg::video_init;
using eg::video;

auto main(int, char *[]) -> int
{
    try
    {
        auto init   = video_init();
        auto val23  = video("Happy Hearts 23", 1024, 768);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    
    return 0;
}