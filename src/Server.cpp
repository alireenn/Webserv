#include "../includes/Server.hpp"

Server::Server(int socketPort)
{
	this->_socket = new Socket(socketPort);
	if (_socket->isRunning())
		this->_running = true;
	else
		this->_running = false;


}

Server::~Server(void)
{
	delete (_socket);
}

Server::Server(Server &toCopy)
{
	this->_socket = new Socket(toCopy.getSocket());
	if (_socket->isRunning())
		this->_running = true;
	else
		this->_running = false;
	this->_port = toCopy._port;
	this->_serverNames = toCopy._serverNames;
	this->_locations = toCopy._locations;
	this->_errorPages = toCopy._errorPages;
	this->_uploadPath = toCopy._uploadPath;
	this->_mimeTypes = toCopy._mimeTypes;
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
	return (*this);
}

Socket	&Server::getSocket(void)
{
	return (*_socket);
}

bool	Server::isRunning(void) const
{
	return (_running);
}
