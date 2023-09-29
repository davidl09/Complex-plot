#include "libcplot.hpp"
#include "toojpeg.h"

#include <cstring>

extern "C"
{
    BitMap::BitMap(int width, int height) : width(width), height(height), _max_width(width), _max_height(height), kill(new bool)
    {
        pixels = new unsigned char[3 * width * height];
        *kill = false;
    }

    BitMap::~BitMap()
    {
        delete[] pixels;
        delete kill;
    }
    
    void BitMap::resize(int new_width, int new_height)
    {
        if(new_width * new_height * 3 > _max_width * _max_height * 3)
        {
            auto new_ptr = new unsigned char[3 * new_width * new_height];

            std::memcpy(new_ptr, pixels, 3 * width * height);

            delete[] pixels;
            pixels = new_ptr;

            _max_width = new_width;
            _max_height = new_height;
        }

        width = new_width;
        height = new_height;            

    }

    void BitMap::reserve(int new_width, int new_height)
    {
        if(new_height * new_width * 3 > _max_width * _max_height * 3)
        {
            auto new_ptr = new unsigned char[3 * new_width * new_height];

            std::memcpy(new_ptr, pixels, 3 * width * height);
/*
            for(int row = 0; row < std::min(height, new_height); row++)
            {
                std::memcpy(new_ptr, pixels, 3 * std::min(width, new_width));
            }
*/
            delete[] pixels;
            pixels = new_ptr;

            _max_width = new_width;
            _max_height = new_height;
        }
    }

    unsigned char* BitMap::get_data()
    {
        return pixels;
    }

    int BitMap::get_width() const
    {
        return width;
    }

    int BitMap::get_height() const
    {
        return height;
    }

    void BitMap::plot_complex_func(std::string&& expr, double maxval, bool grid, int nthreads)
    {
        Parsing::Expression<std::complex<double>> func(expr);
        func.validate_with_except({{'z', 0}, {'i', {0, 1}}});
        BitMap::plot_complex<double>(func, maxval, grid, nthreads);
    }

    void BitMap::save_jpeg(std::string filename)//Does what it says. .jpg extension not necessary in filename
    {
        filename += (filename.length() > 4 && filename.substr(filename.length() - 4, 4) == ".jpg" ? "" : ".jpg");
        jpeg_out = fopen(filename.c_str(), "wb");
        TooJpeg::writeJpeg([](unsigned char byte){fputc(byte, jpeg_out);}, pixels, width, height, true, 100, false, NULL);
        fclose(jpeg_out);
        
    }

}