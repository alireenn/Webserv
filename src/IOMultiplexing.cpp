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
	//conf.parse();
   // std::vector<Server> &Svec = conf.getServers();
   Server &Svec = conf.getServers();
    // if (Svec.size())
    // {
    //     for (size_t i = 0; i < Svec.size(); i++)
    //        CreatServer(Svec[i], *this);
	CreatServer(Svec, *this);
        EventLoop(Svec, *this);
    // }
    // else
    //     printError("Bad config file " + conf.getFilePath());
}

int CreatServer(Server &server, IOMultiplexing &io)
{
    CreateSocket(server.getSocket(), server.getPort(), io);
    return 0;
}