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
	{
		for (size_t i = 0; i < Svec.size(); i++)
		   	Svec[i] = new Server(); // --> Server(int socketPort, std::ifstream configFile);
		EventLoop(Svec, *this);
	}
	else
		printError("Bad config file " + conf.getFilePath());
}

int CreatServer(Server &server, IOMultiplexing &io)
{
	CreateSocket(server.getSocket(), server.getPort(), io);
	return 0;
}