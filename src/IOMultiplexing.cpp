#include "../includes/IOMultiplexing.hpp"
#include "IOMultiplexing.hpp"

IOMultiplexing::IOMultiplexing()
{
	
}

IOMultiplexing::~IOMultiplexing()
{
}



void IOMultiplexing::SetupServers(Config &conf)
{
	std::vector<Server> &Svec = conf.getServers();
	if (Svec.size())
		EventLoop(Svec, *this);
	else
		printError("Bad config file " + conf.getFilePath());
}

