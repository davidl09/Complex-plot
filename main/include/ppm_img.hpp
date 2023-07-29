#ifndef PPM_IMG_HPP
#define PPM_IMG_HPP

#include <fstream>
#include <ios>
#include <vector>
#include <array>
#include <cassert>
#include <complex>
#include <thread>

#include "toojpeg.h"

FILE* jpeg_out;

enum RGB_COLOURS{
    RED,
    GREEN,
    BLUE
}RGB_COL;

class _RGBpix{

    std::array<uint8_t, 3> vals; //red, green, blue

    public:

        _RGBpix()
        {
            vals = {0, 0, 0};
        }

        _RGBpix(uint8_t red, uint8_t green, uint8_t blue)
        {
            vals = {red, green, blue};
        }

        _RGBpix(std::vector<uint8_t>& arr)
        {
            for(int i = 0; i < 3; ++i)
            {
                vals[i] = arr[i];
            }
            
        }

        _RGBpix(unsigned char arr[3])
        {
            for(int i = 0; i < 3; i++)
            {
                vals[i] = arr[i];
            }
        }

        void set_val(std::vector<uint8_t>& arr)
        {
            for(int i = 0; i < 3; ++i)
            {
                vals[i] = arr[i];
            }
        }

        void set_val(std::vector<uint8_t>&& arr)
        {
            for(int i = 0; i < 3; ++i)
            {
                vals[i] = arr[i];
            }
        }

        void set_val(unsigned char value, RGB_COLOURS colour)
        {
            vals[colour] = value;
        }

        void set_val(unsigned char& value, RGB_COLOURS& colour)
        {
            vals[colour] = value;
        }

        uint8_t& val(RGB_COLOURS col) //returns read-write ref to any colour
        {
            return vals[col];
        }

        uint8_t& operator[](int colour)
        {
            return vals[colour];
        }
        
        const std::array<uint8_t, 3>& self_v()
        {
            return vals;
        }
};
/*
A "magic number" for identifying the file type. A ppm image's magic number is the two characters "P6".
Whitespace (blanks, TABs, CRs, LFs).
A width, formatted as ASCII characters in decimal.
Whitespace.
A height, again in ASCII decimal.
Whitespace.
The maximum color value (Maxval), again in ASCII decimal. Must be less than 65536 and more than zero.
A single whitespace character (usually a newline).
A raster of Height rows, in order from top to bottom. Each row consists of Width pixels, in order from left to right. 
Each pixel is a triplet of red, green, and blue samples, in that order. 
Each sample is represented in pure binary by either 1 or 2 bytes. If the Maxval is less than 256, it is 1 byte. 
Otherwise, it is 2 bytes. The most significant byte is first.
*/
class RGB_BitMap
{
    private:
        _RGBpix* pixels;
        const int width, height;

    public:
        RGB_BitMap(unsigned int height, unsigned int width) : 
        width(width), height(height)
        {
            pixels = new _RGBpix[width * height];
        }

        ~RGB_BitMap()
        {
            delete[] pixels;
        }

        constexpr int get_height()
        {
            return height;
        }

        constexpr int get_width()
        {
            return width;
        }

        _RGBpix& at_pos(int i, int j) //returns read write ref. to any pixel
        {
            return pixels[i * width + j];
        }

        void set_all(_RGBpix colour)
        {
            for(auto i = 0; i < width * height; i++)
            {
                pixels[i] = colour;
            }
        }

        template <typename T>
        void set_colour_cmplx(int i, int j, std::complex<T> num)
        {
            assert(i >= 0 && i < height && j >= 0 && j < height);
            if(std::abs(num) == INFINITY) at_pos(i, j) = {255,255,255};
            cmplx_to_colour<T>(at_pos(i, j), num);
        }

        template<typename T>
        void plot_cmplx_func_sector //plot a function over a specified sector of a PPM_IMG bitmap
        (Parsing::Expression<std::complex<T>> expr, int start_row, int num_rows, int maxval, bool grid)
        {
            
            T x, y;
            double pixel_per_int = std::min(height, width) / (2.0 * maxval);

            for(int row = 0; row < num_rows && row + start_row < height; row++)
            {
            
                for(int j = 0; j < width; j++)
                {
                    x = (j - width / 2) / pixel_per_int;
                    y = (-(row + start_row) + height / 2) / pixel_per_int;

                    if(grid && maxval <= 50 && (std::abs(y - std::floor(y)) < 0.002 || std::abs(x - std::floor(x)) < 0.002)) 
                        at_pos(row + start_row, j) = {30, 30, 30};
                    else 
                        set_colour_cmplx<T>(row + start_row, j, expr.evaluate({{'z', {x, y}}}));
                }
            }
        }
        
        template<typename T>
        void plot_cmplx_func_m(Parsing::Expression<std::complex<T>>& expr, int maxval, bool grid, unsigned int nthreads = 16)
        {
            nthreads = (int)std::min(nthreads, std::thread::hardware_concurrency()); //no more threads than available processors
            int rows_per_thread = height / nthreads; //size of each horizontal slice

            std::cout << "Using " << nthreads << " threads...\n";
            std::vector<std::thread> threads;
            threads.reserve(nthreads + 1); 

            
            
            for(int row = 0; row < height - height % rows_per_thread; row += rows_per_thread)
            {
                threads.push_back(
                    std::thread(
                        [=, this](){this->plot_cmplx_func_sector(expr, row, rows_per_thread, maxval, grid);}
                    )
                );
            }

            if(height % nthreads)
            {
                threads.push_back(
                    std::thread(
                        [=, this](){this->plot_cmplx_func_sector(expr, height - height % nthreads, height % nthreads, maxval, grid);}
                    )
                );
            }
            
            for(auto& t : threads)
                t.join();
        }

        void save_jpg(std::string filename)
        {
            filename += (filename.length() > 4 && filename.substr(filename.length() - 4, 4) == ".jpg" ? "" : ".jpg");

            jpeg_out = fopen(filename.c_str(), "wb");

            //temporary due to issues with _RGBpix obj and jpeg storage
            unsigned char* temp_pix = new unsigned char[width * height * 3];

            //temporary, need better copy method
            for(int i = 0; i < width * height * 3; ++i)
            {
                temp_pix[i] = pixels[i/3][i%3];
            }

            TooJpeg::writeJpeg
            (
                [](unsigned char byte){fputc(byte, jpeg_out);},
                temp_pix, width, height, true, 100, false, NULL
            );

            fclose(jpeg_out);
            delete[] temp_pix;
        }
};


#endif