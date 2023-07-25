#include "expr_parsing_cpp/parsing.hpp"
#include "ppm_img.hpp"
#include "complex_colour.hpp"

int main(){
	std::string expr, grid, name, input, temp;
	bool grid_show = false;
	while(true)
	{
		
		std::cout << "Enter a function of z to generate its complex plot.\n" 
					<< "Use a lowercase 'i' to represent the imaginary unit.\n"
					<< "Type 'f' to see a list of available functions and operators.\n";

		std::getline(std::cin, expr);
		if(expr == "f")
		{
			std::cout << "\n\nAvailable functions/operators:\n";
			for(auto& s : Parsing::operators)
				std::cout << s << (s.size() > 1 ? "z)\n" : (s == "^" ? "\n" : " "));
			
			std::cout << "\n\n";
			continue;
		}

		Parsing::Expression<std::complex<double>> func(expr);
		try
		{
			func.evaluate({{'z', {0, 0}}});
		}
		catch(...)
		{
			std::cout << "There was an error parsing your function, try a different input\n";
			continue;
		}

		int size, range;
		while(true)
		{
			std::cout << "Enter the range to be plotted, for example '10' will plot [[-10, 10], [-10i, 10i]]\n";
			std::cin >> temp;
			range = std::stoi(temp);
			if(range < 1 || range > 1000000) std::cout << "Invalid range value, please try again\n";
			else break;
		}
		while(true)
		{
			std::cout << "Enter the size of the image in pixels, for example 400 generates a 160,000 pixel image. Max of 5000x5000 px\n";
			std::cin >> temp;
			size = std::stod(temp);
			if(size < 0 || size > 5000) std::cout << "Image size too large or not valid, try again\n";
			else break;
		}

		std::cout << "Press g to overlay a grid, or any other key followed by 'ENTER' to use default settings\n";
		std::cin >> grid;

		if(grid == "g") grid_show = true;
		std::cout << "Plotting, please wait a few seconds...\n";

		PPM_IMG new_image(size, size);
		new_image.plot_cmplx_func_m<double>(func, range, grid_show, 16);
		
		std::cout << "Done plotting, enter a name for the generated image file\n";
		std::cin >> name;
		std::cout << "Saving file...\n";
		new_image.save_jpg(name);
		std::cout << "Done! Press q to exit, or any other key to generate another plot\n";
		std::cin >> input;
		if(input == "q") exit(0);
		else continue;

	}
	
}

//lime green is positive real
//purple is negative real
//orange is positive imaginary
//blue is negative imaginary


//3*z^4-7*z^3+(2/9)+z^2-z+10-34*i*z^6   										6 roots
//100*(sin(3*arg(exp(i *  (abs(z) - arg(z))))) * exp(i *  (abs(z) - arg(z)))) 	spiral