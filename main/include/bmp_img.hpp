#include <fstream>
#include <vector>
#include <array>

enum rgb_colours{
    RED,
    GREEN,
    BLUE
}RGB_COL;

class _RGBpix{

    std::array<uint8_t, 3> vals; //red, green, blue

    public:
        _RGBpix(uint8_t red, uint8_t green, uint8_t blue)
        {
            vals[RED] = red;
            vals[GREEN] = green;
            vals[BLUE] = blue;
        }

        _RGBpix(_RGBpix& old_pix)
        {
            for(auto i = 0; i < 3; ++i)
            {
                vals[i] = old_pix.vals[i];
            }
        }

        _RGBpix(std::array<uint8_t, 3>& arr)
        {
            for(auto i = 0; i < 3; ++i)
            {
                vals[i] = arr[i];
            }
        }

        void set_val(std::vector<uint8_t>& new_vals)
        {
            for(auto i = 0; i < new_vals.size(); ++i)
            {
                vals[i] = new_vals[i];
            }
        }

        void set_val(std::vector<uint8_t>&& new_vals)
        {
            for(auto i = 0; i < new_vals.size(); ++i)
            {
                vals[i] = new_vals[i];
            }
        }

        void set_val(std::array<uint8_t, 3>&& new_vals)
        {
            for(auto i = 0; i < 3; ++i)
            {
                vals[i] = new_vals[i];
            }
        }

        std::array<uint8_t, 3>& get_vals()
        {
            return vals;
        }

    
};

class PPM_IMG
{
    private:
        std::vector<_RGBpix> self;
        const unsigned int width, height;

    public:
        PPM_IMG(unsigned int height, unsigned int width) : width(width), height(height)
        {
            self.resize(width * height);
        }

        _RGBpix& at(int _y, int _x) //returns read write ref. to any pixel
        {
            return self[_y * width + _x];
        }
};