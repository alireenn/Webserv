#include "../includes/Server.hpp"

Server::Server(int socketPort)// --> Server(int socketPort, std::ifstream configFile);
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


void	Server::setPort(size_t port)
{
	this->_port = port;
}

void	Server::setUploadPath(std::string uploadPath)
{
	this->_uploadPath = uploadPath;
}

void	Server::setLocations(std::vector<Location> locations)
{
	this->_locations = locations;
}

void	Server::setServerNames(std::vector<std::string> serverNames)
{
	this->_serverNames = serverNames;
}

void	Server::setMimeTypes(std::vector<std::string> mimeTypes)
{
	this->_mimeTypes = mimeTypes;
}

void	Server::setErrorPages(std::vector<std::pair <std::string, std::string> > errorPages)
{
	this->_errorPages = errorPages;
}