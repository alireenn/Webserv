#ifndef SERVER_HPP
# define SERVER_HPP
# include <string>
# include <fstream>
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
		Socket												_socket;
		size_t 												_port;
		std::vector<std::string>							_serverNames;
		std::vector<Location>								_locations;
		std::vector<std::pair <std::string, std::string> >	_errorPages;
		std::string											_uploadPath;
		std::vector<std::string>							_mimeTypes;

		int epool_fd;
  		
		//roba trovata public
		char **_env;
		std::string root;
  		int en_handle;
		std::vector<std::pair<std::string,std::string> > _cgi;
	    int a;




	public:
		Server();
		~Server();


		// Server	&operator=(Server &toCopy);

		size_t												getPort(void);
		std::vector<std::pair <std::string, std::string> >	getErrorPages();
		std::vector<std::string>							getServerNames();
		std::vector<Location>								&getLocations(void);
	
		Socket						&getSocket(void);
		int        					&getEpoll_fd(void);
		std::vector<std::string>	&getmime_types(void);

		void		setEpoll_fd(int epool_fd);
		void		setPort(size_t port);
		void		setSocket(Socket socket);
		void		setUploadPath(std::string uploadPath);
		void		setLocations(std::vector<Location> locations);
		void		setMimeTypes(std::vector<std::string> mimeTypes);
		void		setCgi(std::pair <std::string, std::string> cgi);
		void		setServerNames(std::vector<std::string> serverNames);
		void		setErrorPages(std::vector<std::pair <std::string, std::string> > errorPages);

		std::string	getUploadPath(void);
		void		reset(void);
		void		init_MimeTypes(void);

};

// operatore di overload << per la classe server
std::ostream &operator<<(std::ostream &out, Server &server);


#endif
