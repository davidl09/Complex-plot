#include "complex_colour.hpp"
#include "toojpeg.h"


FILE* jpeg_out;
class BitMap
{
    const int width, height;
    unsigned char* pixels;

    public:
        BitMap(int width, int height) : width(width), height(height)
        {
            pixels = new unsigned char[3 * width * height];
        }

        ~BitMap()
        {
            delete[] pixels;
        }

        void save_jpeg(std::string filename)
        {
            filename += (filename.length() > 4 && filename.substr(filename.length() - 4, 4) == ".jpg" ? "" : ".jpg");

            jpeg_out = fopen(filename.c_str(), "wb");

            //temporary due to issues with _RGBpix obj and jpeg storage
            unsigned char* temp_pix = pixels;

            TooJpeg::writeJpeg
            (
                [](unsigned char byte){fputc(byte, jpeg_out);},
                temp_pix, width, height, true, 100, false, NULL
            );

            fclose(jpeg_out);
            delete[] temp_pix;
        }
}