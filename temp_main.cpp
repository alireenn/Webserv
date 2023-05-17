#include "includes/Server.hpp"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

/* Queste due funzioni servono a liberare la roba in caso di
** Ctrl + C, che sarà una causa abbastanza comune di interruzione
*/                                                              //
void	server_deleter(Server *toDelete)                        //
{                                                               //
	static Server	*server;                                //
                                                                //
	if (toDelete)                                           //
		server = toDelete;                              //
	else                                                    //
		delete (server);                                //
}                                                               //
                                                                //
void	sigint_handler(int signum)                              //
{                                                               //
	(void)signum;                                           //
	server_deleter(NULL);                                   //
	exit(EXIT_SUCCESS);                                     //
}                                        /////////////////////////

int	main(int argc, char **argv)
{
	Server			*testServer;
	socklen_t			clientAddrSize;
	struct sockaddr_in	clientAddr;
	struct sigaction 	sa;
	int			temp_sock;
	int			pid;

	if (argc != 2)
	{
		std::cerr << "One argiment, please." << std::endl;
		return (1);
	}
	testServer = new Server(atoi(argv[1]));
	if (!testServer->isRunning())
	{
		delete (testServer);
		return (1);
	}
	server_deleter(testServer);
	memset( &sa, 0, sizeof(sa) );
	sa.sa_handler = sigint_handler;
	sigfillset(&sa.sa_mask);
	sigaction(SIGINT,&sa,NULL);
	clientAddrSize = sizeof(clientAddr);
	std::cout << testServer->getSocket().getPort() << " "
				<< testServer->getFd() << std::endl;
	while (true)
	{
		temp_sock = accept(testServer->getFd(),
				(struct sockaddr *)&clientAddr, &clientAddrSize);
		if (temp_sock < 0)
		{
			// std::cerr << "accept() failed." << std::endl;
			continue ;
			// perror("accept");
			// exit(0);
		}
		else
		{
			pid = fork();
			if (pid < 0)
			{
				std::cerr << "fork() failed" << std::endl;
				continue ;
			}
			else if (pid == 0)
			{
				std::cout << temp_sock << std::endl;
				std::string	str;
				char		buff[1];
				delete (testServer);
				while (read(temp_sock, buff, 1) == 1)
					str.push_back(*buff);
				write(temp_sock, str.c_str(), str.size());
				exit(0);
		}
		else
			close(temp_sock);}
	}
	delete (testServer);
	return (0);
}
