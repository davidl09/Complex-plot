#include "parsing.hpp"
#include "ppm_img.hpp"

int main(){
	PPM_IMG test(400, 400);
	test.set_all({255, 255, 255});
	test.save("test.ppm");
}
