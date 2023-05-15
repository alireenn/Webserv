#include "includes/Server.hpp"
#include <stdlib.h>

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "One argiment, please." << std::endl;
		return (1);
	}

	Server	testServer(atoi(argv[1]));

	if (!testServer.isRunning())
		return (1);
	return (0);
}
