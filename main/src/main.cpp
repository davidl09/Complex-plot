#include "expr_parsing_cpp/parsing.hpp"
#include "ppm_img.hpp"

int main(){
	PPM_IMG test(1000, 1000);
	test.set_all({255, 255, 255});
	test.save("test.ppm");
}
