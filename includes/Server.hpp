#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <vector>
# include "Socket.hpp"
# include "Location.hpp"

class Server
{
	private:
		Socket					*_socket;
		size_t 					_port;
		bool					_running;
		std::vector<std::string>		_serverNames;
		std::vector<Location>			_locations;
		std::vector<std::pair
			<std::string, std::string> >	_errorPages;
		std::string				_uploadPath;
		std::vector<std::string>		_mimeTypes;

							Server(void);

	public:
			Server(int socketPort); // --> Server(int socketPort, std::ifstream configFile);
			~Server(void);
			Server(Server &toCopy);
		Server	&operator=(Server &toCopy);

		Socket	&getSocket(void);
		bool	isRunning(void) const;
};

#endif
