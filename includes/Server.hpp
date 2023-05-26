#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
//# include <vector>
# include "Socket.hpp"
# include "Location.hpp"

typedef enum e_status
{
	NO_SERVER_YET,
	CONFIGURING_SERVER
} t_status;


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
		std::vector<std::pair <std::string, std::string> >	_cgi;
		std::vector<std::pair <std::string, std::string> >	_errorPages;



	public:
			/*
			** After creation, make sure to
			** check that isRunning() == true.
			*/
			Server(void);
			Server(int socketPort); // --> Server(int socketPort, std::ifstream configFile);
			~Server(void);
			Server(Server &toCopy);

  		char **_env;
		Server	&operator=(Server &toCopy);

		std::vector<std::pair <std::string, std::string> >	getcgi();
		std::vector<std::pair <std::string, std::string> >	getErrorPages();
		std::vector<std::string>							getServerNames();
		int													getFd(void) const;
		int 												getPort(void) const;
		std::vector<Location>								&getLocations(void);
		bool												isRunning(void) const;
	

		Socket		&getSocket(void);
		void		setPort(size_t port);
		void		setUploadPath(std::string uploadPath);
		void		setLocations(std::vector<Location> locations);
		void		setMimeTypes(std::vector<std::string> mimeTypes);
		void		setCgi(std::pair <std::string, std::string> cgi);
		void		setServerNames(std::vector<std::string> serverNames);
		void		setErrorPages(std::vector<std::pair <std::string, std::string> > errorPages);

		void 		addLocation(Location location);
		void		reset(void);
};

//operatore di overload << per la classe server
// std::ostream &operator<<(std::ostream &out, Server &server);


#endif
