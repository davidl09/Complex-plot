#ifndef CMPLX_COLOUR_HPP
#define CMPLX_COLOUR_HPP

#include <cmath>
#include <complex>
#include "ppm_img.hpp"

template <typename T>
static void cmplx_to_colour(_RGBpix& pix, std::complex<T> num)
{
    T mag = std::abs(num), arg = std::arg(num);
    mag = -5.0/(mag + 5) + 1;
    for(int i = 0; i < 3; ++i)
    {
        pix[i] = (unsigned char)(mag*(127.5*sin(arg + (M_PI * i) / 2) + 127.5));
    }
}



#endif