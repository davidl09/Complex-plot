#include "expr_parsing_cpp/parsing.hpp"
#include "ppm_img.hpp"
#include "complex_colour.hpp"

int main(){
	PPM_IMG test(1000, 1000);

	std::string in;
	std::cout << "Enter an expression to plot\n";
	std::cin >> in;

	test.set_all({255, 255, 255});
	test.save("test.ppm");
}
