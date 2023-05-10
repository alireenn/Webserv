#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>

class Server
{
	private:
	public:
		Socket _socket;
		size_t _port;
		std::vector<std::string> _serverNames;
		std::vector<Location> _locations;
		std::vector<std::pair<std::string, std::string> > _error_pages;
		std::string _uploadPath;
		std::vector<std::string> mime_types;
		// Server(/* args */);
		// ~Server();

};




#endif