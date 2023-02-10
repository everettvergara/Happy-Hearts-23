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
        SDL_Window *win_ = NULL;
        

    public:

        video()
        {
        }

        ~video()
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

    };

}

using eg::video_init;
using eg::video;

auto main(int, char *[]) -> int
{
    try
    {
        auto init   = video_init();
        auto val23  = video();
        
        val23.create_win("Happy Heart's 23", 1024, 768);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    
    return 0;
}