#ifndef LIBCPLOT_HPP
#define LIBCPLOT_HPP

#include <thread>
#include <cassert>
#include <vector>
#include <complex>
#include <mutex>

#include "expression.h"


namespace ComplexPlot
{
    template<typename T>
    static void cmplx_to_colour(unsigned char* pix, std::complex<T> num)
    {
        T mag = std::abs(num), arg = std::arg(num);
        mag = -5.0/(mag + 5) + 1;
        std::mutex m;
        m.lock();
        for(int i = 0; i < 3; ++i)
        {
            pix[i] = (unsigned char)(mag*(127.5*sin(arg + (M_PI * i) / 2) + 127.5));
        }
        m.unlock();
    }
};


static FILE* jpeg_out;
class BitMap
{
    int width, height, _max_width, _max_height;
    unsigned char* pixels;
    bool *kill;

    template<typename T>
    void plot_complex_sector
    (const Expression<std::complex<T>>& expr, int start_row, int num_rows, double maxval, bool grid)
    {
        T x, y;
        double pixel_per_int = std::min(height, width) / (2.0 * maxval);

        for(int row = 0; row < num_rows && row + start_row < height; row++)
        {

            for(int j = 0; j < width; j++)
            {
                if (*kill)
                    return;

                x = (j - width / 2) / pixel_per_int;
                y = (-(row + start_row) + height / 2) / pixel_per_int;

                if(grid && maxval <= 50 && (std::abs(y - std::floor(y)) < 0.005 || std::abs(x - std::floor(x)) < 0.005)) 
                    for(int i = 0; i < 3; ++i)
                    {
                        pixels[at_pos_index(row + start_row, j) + i] = 30;
                    }
                    
                else 
                    ComplexPlot::cmplx_to_colour(pixels + at_pos_index(row + start_row, j), expr.evaluate({
                                                                                                            {"z", {x, y}}, 
                                                                                                            {"i", {0, 1}},
                                                                                                            {"e", {std::numbers::e, 0}},
                                                                                                            {"pi", {std::numbers::pi}}
                                                                                                    
                    }));
            }
        }
    }


    [[nodiscard]] constexpr int at_pos_index(int row, int column) const
    {
        return 3 * (row * width + column);
    }

public:

    template<typename T>
    void plot_complex
    (Expression<std::complex<T>>& expr, double maxval, bool grid, int nthreads)
    {
        nthreads = std::min(nthreads, static_cast<int>(std::thread::hardware_concurrency())); //no more threads than available processors
        int rows_per_thread = height / nthreads; //size of each horizontal slice
        
        std::vector<std::thread> threads;
        threads.reserve(nthreads + 1);

        for(int row = 0; row < height - height % rows_per_thread; row += rows_per_thread)
        {

            threads.push_back(
                std::thread(
                    [=, this](){this->plot_complex_sector(expr, row, rows_per_thread, maxval, grid);}
                )
            );
        }

        if(height % nthreads)
        {
            threads.push_back(
                std::thread(
                    [=, this](){this->plot_complex_sector(expr, height - height % nthreads, height % nthreads, maxval, grid);}
                )
            );
        }
        
        for(auto& t : threads)
            t.detach();
    }


        BitMap(int width, int height);

        ~BitMap();

        void resize(int width, int height);

        void reserve(int maxwidth, int maxheight);

        unsigned char* get_data();

        int get_width() const;

        int get_height() const;

        void plot_complex_func(std::string&& expr, double maxval, bool grid, int nthreads);

        void save_jpeg(std::string filename);
};

#endif