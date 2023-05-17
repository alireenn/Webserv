#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include "Location.hpp"

class Server
{
	private:
		Socket _socket;
		std::vector<std::string> _serverNames;
		std::vector<Location> _locations;
		std::vector<std::pair<std::string, std::string> > _error_pages;
		std::string _uploadPath;
		std::vector<std::string> mime_types;
		size_t _port;
	public:
		// Server(/* args */);
		// ~Server();

};




#endif