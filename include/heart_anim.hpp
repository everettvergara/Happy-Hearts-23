#pragma once

#include <boost/range/combine.hpp>
#include "heart.hpp"

namespace eg
{

    class heart_anim
    {
    private:
        heart heart_;
        Sint beats_ = 3;
        
        FP rot_ = 0.0;
        FP rot_n_ = 0.025;
        FP rot_min_ = -1.0;
        FP rot_max_ = +1.0;

        FP rad_ = 40.0;
        FP rad_n_ = 1.0;
        FP rad_min_ = 25.0;
        FP rad_max_ = 80.0;

    public:
        heart_anim(
            const Sint pts = 720, 
            const Sint ini_burn = 255, 
            const Sint distort = 5,
            const Sint cx = 1024 / 2,
            const Sint cy = 768 / 2,
            const Sint beats = 3,

            const FP rot = 0.0,
            const FP rot_n = 0.025,
            const FP rot_min = -1.0,
            const FP rot_max = +1.0,

            const FP rad = 40.0,
            const FP rad_n = 1.0,
            const FP rad_min = 25.0,
            const FP rad_max = 80.0
            )
            
            : 
            heart_(pts, ini_burn, distort, cx, cy),
            beats_(beats),
            rot_(rot),
            rot_n_(rot_n),
            rot_min_(rot_min),
            rot_max_(rot_max),
            rad_(rad),
            rad_n_(rad_n),
            rad_min_(rad_min),
            rad_max_(rad_max)
        {
        }

        auto random_spawn(const int w, const int h)
        {
            heart_.set_cx(rand() % w);
            heart_.set_cy(rand() % h);

            rad_ = 40.0 + rand() % 30;
            rad_min_ = rad_ - rand() % 30;
            rad_max_ = rad_ + rand() % 30;
            rad_n_ = -5 + rand() % 10;

            rot_ = M_PI2 * static_cast<FP>(rand()) / RAND_MAX;
            rot_min_ = rot_ - M_PI2 * (static_cast<FP>(rand()) / RAND_MAX);
            rot_max_ = rot_ + M_PI2 * (static_cast<FP>(rand()) / RAND_MAX);
            rot_n_ = -0.015625 * 5 + 0.015625 * static_cast<FP>(rand() % 10);            

            beats_ = 3 + rand() % 10;
        }

        auto animate(std::vector<Uint8> &heart_surface, int w, int h, int s)
        {
            rot_ += rot_n_;
            if (rot_n_ > 0 and rot_ >= rot_max_) rot_n_ *= -1;
            else if (rot_n_ < 0 and rot_ <= rot_min_) rot_n_ *= -1;

            rad_ += rad_n_;
            if (rad_n_ > 0 and rad_ >= rad_max_) rad_n_ *= -1;
            else if (rad_n_ < 0 and rad_ <= rad_min_) 
            {
                --beats_;
                rad_n_ *= -1;
            }

            heart_.recalc(w, s, rad_, rot_);
            for (const auto [ix, c] : boost::combine(heart_.get_heart(), heart_.get_col()))
                heart_surface.at(ix) = c;

            if (beats_ == 0) random_spawn(w, h);
        }

    };

}