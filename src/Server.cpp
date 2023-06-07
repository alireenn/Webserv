#include "../includes/Server.hpp"

Server::Server()
{
	//init_mimeTypes();
}

Server::~Server()
{}

Socket		&Server::getSocket(void)
{
	return _socket;
}

size_t		Server::getPort(void)
{
	return _port;
}

std::vector<std::string>	Server::getServerNames(void)
{
	return _serverNames;
}

std::vector<Location>		&Server::getLocations(void)
{
	return _locations;
}

std::vector<std::pair <std::string, std::string> >	Server::getErrorPages(void)
{
	return _errorPages;
}

std::string	Server::getUploadPath(void)
{
	return _uploadPath;
}

void Server::setSocket(Socket socket)
{
	_socket = socket;
}

void Server::setPort(size_t port)
{
	_port = port;
}

void Server::setServerNames(std::vector<std::string> serverNames)
{
	_serverNames = serverNames;
}

void Server::setLocations(std::vector<Location> locations)
{
	_locations = locations;
}

void Server::setErrorPages(std::vector<std::pair <std::string, std::string> > errorPages)
{
	_errorPages = errorPages;
}

void Server::setUploadPath(std::string uploadPath)
{
	_uploadPath = uploadPath;
}

//funzione che reinizializza il server
void Server::reset(void)
{
	_socket = Socket();
	_port = 0;
	_serverNames.clear();
	_locations.clear();
	_errorPages.clear();
	_uploadPath = "";
}

std::vector<std::string> &Server::getMimeTypes(void)
{
    return this->_mimeTypes;
}

void Server::init_MimeTypes(void)
{
    std::ifstream file;
    file.open("srcs/mime.types");
    std::string str;
    while(getline(file,str))
        _mimeTypes.push_back(str);
    file.close();
}

void Server::setCgi(std::pair<std::string,std::string> cgi)
{
    _cgi.push_back(cgi);
}

int &Server::getEpoll_fd(void)
{
	return epool_fd;
}

void Server::setEpoll_fd(int epool_fd)
{
	this->epool_fd = epool_fd;
}

std::ostream &operator<<(std::ostream &out, Server &server)
{
	out << "Server: " << server.getPort() << std::endl;
	out << "ServerNames: ";
	for (size_t i = 0; i < server.getServerNames().size(); i++)
		out << server.getServerNames()[i] << " ";
	out << std::endl;
	out << "Locations: ";
	for (size_t i = 0; i < server.getLocations().size(); i++)
		out << server.getLocations()[i] << " ";
	out << std::endl;
	out << "ErrorPages: ";
	for (size_t i = 0; i < server.getErrorPages().size(); i++)
		out << server.getErrorPages()[i].first << " " << server.getErrorPages()[i].second << " ";
	out << std::endl;
	out << "UploadPath: " << server.getUploadPath() << std::endl;
	return out;
}
