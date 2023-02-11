#pragma once

#include <boost/range/combine.hpp>
#include "heart.hpp"

namespace eg
{
    constexpr Sint rad_def = 40;
    constexpr Sint rad_min = 25;
    constexpr Sint rad_add = 50;
    constexpr Sint rad_max = rad_min + rad_add + 1;
    constexpr Sint rad_n_min = 1;
    constexpr Sint rad_n_add = 3;
    constexpr FP   rot_units = 0.0078125;
    constexpr Sint rot_n_max = 10;

    constexpr Sint min_distort = 0;
    constexpr Sint distort_rate = 1;
    constexpr Sint min_beats = 3;
    constexpr Sint add_beats = 3;

    class heart_anim
    {
    private:
        heart heart_;
        
        Sint distort_;
        Sint cx_, cy_;
        Sint beats_;
        
        FP rot_;
        FP rot_n_;
        FP rot_min_;
        FP rot_max_;

        Sint rad_;
        Sint rad_n_;
        Sint rad_min_;
        Sint rad_max_;

    public:
        heart_anim(const Sint pts = 720, const Sint ini_burn = 255)
            : heart_(pts, ini_burn)

        {
        }

        auto random_spawn(const int w, const int h)
        {
            cx_ = rand() % w;
            cy_ = rand() % h;

            rad_min_    = rad_min + rand() % (rad_add / 2);
            rad_max_    = rad_min_ + rand() % (1 + rad_max - rad_min_);
            rad_        = rad_min_ + rand() % (1 + rad_max_ - rad_min);
            rad_n_      = rad_n_min + rand() % rad_n_add;

            rot_        = M_PI2 * static_cast<FP>(rand()) / RAND_MAX;
            rot_min_    = rot_ - M_PI2 * (static_cast<FP>(rand()) / RAND_MAX);
            rot_max_    = rot_ + M_PI2 * (static_cast<FP>(rand()) / RAND_MAX);
            rot_n_      = rot_units * (1 + rand() % rot_n_max);

            beats_      = min_beats + rand() % add_beats;
            distort_    = min_distort;
        }

        auto animate(std::vector<Uint8> &heart_surface, int w, int h, int s)
        {
            // Bounce back if needed
            rot_ += rot_n_;
            if (rot_n_ > 0 and rot_ >= rot_max_) rot_n_ *= -1;
            else if (rot_n_ < 0 and rot_ <= rot_min_) rot_n_ *= -1;

            // Heart Beat and Distortion
            rad_ += rad_n_;
            if (rad_n_ > 0 and rad_ >= rad_max_) 
            {
                distort_ += distort_rate;
                rad_n_ *= -1;

            } else if (rad_n_ < 0 and rad_ <= rad_min_) 
            {
                --beats_;
                rad_n_ *= -1;
                distort_ += distort_rate;
            }

            // Recalc heart
            heart_.recalc(w, s, static_cast<FP>(rad_), rot_, cx_, cy_, distort_);
            for (const auto [ix, c] : boost::combine(heart_.get_heart(), heart_.get_col()))
                heart_surface.at(ix) = c;

            if (beats_ == 0) random_spawn(w, h);
        }
    };

}