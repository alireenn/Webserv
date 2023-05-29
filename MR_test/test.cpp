#include "log.hpp"

int	main(void)
{
	int nbr = 8080;

	log::init();
	log::cout << timestamp << "Hello!" << 10 << " " << nbr << std::endl;
	log::cwar << timestamp << "This is still ok, but pay attenytion, for Vim's sake..." << std::endl;
	log::cerr << timestamp << "Error: nothing works here!" << std::endl;
	log::close();
}

