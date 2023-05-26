#include "../includes/Server.hpp"
#include "Server.hpp"

Server::Server(void)
{
	;
}

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
	{
		this->_port = this->_socket->getPort();
		amc::lout << "New server started listening on port "
						<< socketPort << std::endl;
	}
}

Server::~Server(void)
{
	delete (_socket);
}

Server::Server(Server &toCopy)
{
	// try that toCopy isRunning()!
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
//	if (this->_running)
//		amc::lout << timestamp << "New server started listening on port "
//						<< toCopy._port << std::endl;
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
		amc::lout << "New server started listening on port "
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

int Server::getPort(void) const
{
    return this->_port;
}
std::vector<std::string> Server::getServerNames()
{
    return std::vector<std::string>(this->_serverNames);
}

std::vector<std::pair<std::string, std::string> > Server::getcgi()
{
    return this->_cgi;
}

std::vector<std::pair<std::string, std::string> > Server::getErrorPages()
{
    return this->_errorPages;
}
bool Server::isRunning(void) const
{
	return (_running);
}


void	Server::setPort(size_t port)
{
	if (this->_socket)
		delete (this->_socket);
	this->_socket = new Socket(port);
	if (this->_socket->isRunning())
	{
		this->_running = true;
	}
	else
	{
		this->_running = false;
	}
	if (this->_running)
	{
		this->_port = this->_socket->getPort();
		//amc::lout << "New server started listening on port "
						//<< port << std::endl;
	}
}

void	Server::setUploadPath(std::string uploadPath)
{
	this->_uploadPath = uploadPath;
}

void	Server::setLocations(std::vector<Location> locations)
{
	this->_locations = locations;
}


void Server::setServerNames(std::vector<std::string> serverNames)
{
	this->_serverNames = serverNames;
}



void Server::setCgi(std::pair<std::string, std::string> cgi)
{
	// this->_cgi = cgi;
	this->_cgi.insert(this->_cgi.end(), cgi);
}

void	Server::setMimeTypes(std::vector<std::string> mimeTypes)
{
	this->_mimeTypes = mimeTypes;
}

void	Server::setErrorPages(std::vector<std::pair <std::string, std::string> > errorPages)
{
	this->_errorPages = errorPages;
}

void	Server::addLocation(Location location)
{
	this->_locations.push_back(location);
}