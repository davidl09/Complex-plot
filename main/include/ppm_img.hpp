#ifndef PPM_IMG_HPP
#define PPM_IMG_HPP

#include <fstream>
#include <ios>
#include <vector>
#include <array>
#include <cassert>
#include <complex>



enum RGB_COLOURS{
    RED,
    GREEN,
    BLUE
}RGB_COL;

class _RGBpix{

    std::vector<uint8_t> vals; //red, green, blue

    public:

        _RGBpix()
        {
            vals = {0, 0, 0};
        }

        _RGBpix(uint8_t red, uint8_t green, uint8_t blue)
        {
            vals.resize(3);
            vals[RED] = red;
            vals[GREEN] = green;
            vals[BLUE] = blue;
        }

        _RGBpix(std::vector<uint8_t>& arr)
        {
            vals = arr;
        }

        _RGBpix(unsigned char arr[3])
        {
            for(int i = 0; i < 3; i++)
            {
                vals[i] = arr[i];
            }
        }

        void set_val(std::vector<uint8_t>& new_vals)
        {
            vals = new_vals;
        }

        void set_val(std::vector<uint8_t>&& new_vals)
        {
            vals = new_vals;
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
        
        const std::vector<uint8_t>& self_v()
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
class PPM_IMG
{
    private:
        std::vector<_RGBpix> pixels;
        const unsigned int width, height;
        const std::string ppm_header;

    public:
        PPM_IMG(unsigned int height, unsigned int width) : 
        width(width), height(height), 
        ppm_header("P6 " + std::to_string(width) + " " + std::to_string(height) + " " + "255\n")
        {
            pixels.resize(width * height);
        }

        _RGBpix& at_pos(int i, int j) //returns read write ref. to any pixel
        {
            return pixels[i * width + j];
        }

        void set_all(_RGBpix colour)
        {
            for(auto i = pixels.begin(); i != pixels.end(); i++)
            {
                *i = colour;
            }
        }

        template <typename T>
        void set_colour_cmplx(int i, int j, std::complex<T> num)
        {
            assert(i >= 0 && i < height && j >= 0 && j < height);
            cmplx_to_colour<double>(at_pos(i, j), num);
        }

        template<typename T>
        void plot_cmplx_func(Parsing::Expression<T>& expr, double pixel_per_int)
        {
            for(int i = 0; i < 1000; ++i)
            {
                for(int j = 0; j < 1000; ++j)
                {
                    set_colour_cmplx<double>(i, j, expr.evaluate({{'z', {(j - 500)/pixel_per_int, (-i + 500)/pixel_per_int}}}));
                }
            }
        }

        void save(std::string filename)
        {
            std::ofstream output;

            output.open(filename + 
                (filename.length() > 4 && filename.substr(filename.length() - 4, 4) == ".ppm" ? "" : ".ppm"),
                std::ios::out | std::ios::binary
            );
            assert(output.is_open());

            output.write(ppm_header.c_str(), ppm_header.size());

            for(auto i = pixels.begin(); i != pixels.end(); ++i)
            {
                output.write((const char *)&(*(i->self_v().begin())), 3);
            }

            output.close();

        }
};


#endif