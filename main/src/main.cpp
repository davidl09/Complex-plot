#include "expr_parsing_cpp/parsing.hpp"
#include "ppm_img.hpp"
#include "complex_colour.hpp"

int main(){
	PPM_IMG test(1000, 1000);
	Parsing::Expression<std::complex<double>> expr("z"); //(((cpow(z, 2) - 1)*cpow(z - 2 - I, 2))/(cpow(z, 2) + 2 + 2*I))
	test.plot_cmplx_func(expr, 10, false);
	test.save("test.ppm");
}

//lime green is positive real
//purple is negative real
//orange is positive imaginary
//blue is negative imaginary