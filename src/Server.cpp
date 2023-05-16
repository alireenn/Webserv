#include "../includes/Server.hpp"

Server::Server(int socketPort)
{
	this->_socket = new Socket(socketPort);
	if (this->_socket->isRunning())
	{
		this->_running = true;
	}
	else
	{
		this->_running = false;
	}
	if (this->_running)
		std::cout << "New server started listening on port "
						<< socketPort << std::endl;
}

Server::~Server(void)
{
	delete (_socket);
}

Server::Server(Server &toCopy)
{
	this->_socket = new Socket(toCopy.getSocket());
	if (this->_socket->isRunning())
		this->_running = true;
	else
		this->_running = false;
	this->_port = toCopy._port;
	this->_serverNames = toCopy._serverNames;
	this->_locations = toCopy._locations;
	this->_errorPages = toCopy._errorPages;
	this->_uploadPath = toCopy._uploadPath;
	this->_mimeTypes = toCopy._mimeTypes;
	if (this->_running)
		std::cout << "New server started listening on port "
						<< toCopy._port << std::endl;
}

Server	&Server::operator=(Server &toCopy)
{
	this->_socket = toCopy._socket;
	this->_port = toCopy._port;
	this->_serverNames = toCopy._serverNames;
	this->_locations = toCopy._locations;
	this->_errorPages = toCopy._errorPages;
	this->_uploadPath = toCopy._uploadPath;
	this->_mimeTypes = toCopy._mimeTypes;
	this->_running = toCopy._running;
	if (this->_running)
		std::cout << "New server started listening on port "
						<< toCopy._port << std::endl;
	return (*this);
}

Socket	&Server::getSocket(void)
{
	return (*_socket);
}

int	Server::getFd(void) const
{
	return (_socket->getFd());
}

bool	Server::isRunning(void) const
{
	return (_running);
}
