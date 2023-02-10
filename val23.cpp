#include <iostream>
#include <SDL.h>

#include "video_init.hpp"
#include "video.hpp"

using eg::video_init;
using eg::video_quit;
using eg::video;

auto main(int, char *[]) -> int
{
    try
    {
        video_init();

        auto val23  = video();
        val23.create_win("Happy Heart's 23", 1024, 768);
        val23.run();

        video_quit();

    } catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    
    return 0;
}

