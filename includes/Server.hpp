#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <vector>
# include "Socket.hpp"
# include "Location.hpp"

class Server
{
	private:
		bool												_running;
		Socket												*_socket;
		size_t 												_port;
		std::string											_uploadPath;
		std::vector<Location>								_locations;
		std::vector<std::string>							_serverNames;
		std::vector<std::string>							_mimeTypes;
		std::vector<std::pair <std::string, std::string> >	_errorPages;

							Server(void);

	public:
			/*
			** After creation, make sure to
			** check that isRunning() == true.
			*/
			Server(int socketPort); // --> Server(int socketPort, std::ifstream configFile);
			~Server(void);
			Server(Server &toCopy);
		Server	&operator=(Server &toCopy);

		Socket	&getSocket(void);
		int		getFd(void) const;
		bool	isRunning(void) const;
};

#endif
