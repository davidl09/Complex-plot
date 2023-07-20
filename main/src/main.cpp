#include "expr_parsing_cpp/parsing.hpp"
#include "ppm_img.hpp"
#include "complex_colour.hpp"

int main(){
	PPM_IMG test(1000, 1000);
	Parsing::Expression<std::complex<double>> expr("z");
	test.plot_cmplx_func(expr, 80);
	test.save("test.ppm");
}
