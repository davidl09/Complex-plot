#ifndef CMPLX_COLOUR_HPP
#define CMPLX_COLOUR_HPP

#include <cmath>
#include <complex>
#include "ppm_img.hpp"

template <typename T>
class CmplColour{
    public:
        static _RGBpix cmplx_to_rgb(std::complex<T> num)
        {
            T mag = std::abs(num), arg = std::arg(num);

            mag = ((T)(-1))/(mag + 1) + 1;

            unsigned char colours[3];

            for(int i = 0; i < 3; i++)
            {
                colours[i] = (unsigned char)(mag*(127.5*sin(arg + 2 * i * M_PI) + 127.5));
            }

            return _RGBpix(colours);
        }
};

#endif