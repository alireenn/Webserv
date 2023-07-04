#include "../includes/Location.hpp"

Location::Location() : _clientMaxBodySize("")
{}

Location::Location(Location const &obj)
{
	*this = obj;
}

Location::~Location()
{
}

std::string	&Location::getLocationPath()
{
	return (_locationPath);
}

std::vector<std::string>	&Location::getAllowedMethods()
{
	return (_allowedMethods);
}

std::string	&Location::getRoot()
{
	return (_root);
}

std::string	&Location::getClientMaxBodySize()
{
	return (_clientMaxBodySize);
}

std::vector<std::string> &Location::getIndex()
{
	return (_index);
}

std::string	&Location::getAutoIndex()
{
	return (_autoIndex);
}

std::string	&Location::getUploadPath()
{
	return (_uploadPath);
}

void	Location::setLocationPath(std::string &locationPath)
{
	_locationPath = locationPath;
}

void	Location::setAllowedMethods(std::vector<std::string> &allowedMethods)
{
	_allowedMethods = allowedMethods;
}

void	Location::setRoot(std::string &root)
{
	_root = root;
}

void	Location::setClientMaxBodySize(std::string &clientMaxBodySize)
{
	_clientMaxBodySize = clientMaxBodySize;
}

void	Location::setIndex(std::vector<std::string> &index)
{
	_index = index;
}

void	Location::setAutoIndex(std::string &autoIndex)
{
	_autoIndex = autoIndex;
}

void	Location::setUploadPath(std::string &uploadPath)
{
	_uploadPath = uploadPath;
}

void	Location::setRedirection(std::pair<std::string, std::string> &redirection)
{
	_redirection = redirection;
}

std::pair<std::string, std::string>	&Location::getRedirection()
{
	return (_redirection);
}

//operatore di overloading <<
// std::ostream	&operator<<(std::ostream &out, Location &location)
// {
// 	out << "locationPath: " << location.getLocationPath() << std::endl;
// 	out << "root: " << location.getRoot() << std::endl;
// 	out << "index: " << location.getIndex() << std::endl;
// 	out << "autoIndex: " << location.getAutoIndex() << std::endl;
// 	out << "uploadPath: " << location.getUploadPath() << std::endl;
// 	out << "redirection: " << location.getRedirection().first << " " << location.getRedirection().second << std::endl;
// 	out << "allowedMethods: ";
// 	for (std::vector<std::string>::iterator it = location.getAllowedMethods().begin(); it != location.getAllowedMethods().end(); it++)
// 		out << *it << " ";
// 	out << std::endl;
// 	out << "clientMaxBodySize: " << location.getClientMaxBodySize() << std::endl;
// 	return (out);
// }