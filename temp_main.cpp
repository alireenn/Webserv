#define RESET       =   "\033[0m" 
#define BLACK       =   "\033[30m"
#define RED         =   "\033[31m"
#define GREEN       =   "\033[32m"
#define YELLOW      =   "\033[33m"
#define BLUE        =   "\033[34m"
#define MAGENTA     =   "\033[35m"
#define CYAN        =   "\033[36m"
#define WHITE       =   "\033[37m"
#define BOLDBLACK   =   "\033[1m\033[30m"
#define BOLDRED     =   "\033[1m\033[31m"
#define BOLDGREEN   =   "\033[1m\033[32m"
#define BOLDYELLOW  =   "\033[1m\033[33m"
#define BOLDBLUE    =   "\033[1m\033[34m"
#define BOLDMAGENTA =   "\033[1m\033[35m"
#define BOLDCYAN    =   "\033[1m\033[36m"
#define BOLDWHITE   =   "\033[1m\033[37m"

#include "includes/Server.hpp"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fstream>
#include <string>
#include <sstream>


void sendHTMLPage(int clientSocket)
{
	std::ifstream file("index.html");
	std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	std::stringstream ss;
	ss << content.size();
	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + ss.str() + "\r\n\r\n" + content;
	write(clientSocket, response.c_str(), response.size());
}


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
	signal(SIGPIPE, SIG_IGN);//La chiamata a signal(SIGPIPE, SIG_IGN) imposta il gestore dei segnali per il segnale SIGPIPE in modo da ignorare il segnale quando viene ricevuto, anziché gestirlo con il comportamento predefinito del sistema operativo, che di solito è terminare il processo.
	server_deleter(testServer);
	memset( &sa, 0, sizeof(sa) );
	sa.sa_handler = sigint_handler;
	sigfillset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	clientAddrSize = sizeof(clientAddr);
	std::cout << testServer->getSocket().getPort() << " "
				<< testServer->getFd() << std::endl;
	while (true)
	{
		temp_sock = accept(testServer->getFd(),
				(struct sockaddr *)&clientAddr, &clientAddrSize);
		if (temp_sock < 0)
		{
			std::cerr << "accept() failed." << std::endl;
			// exit(0);
		}
		pid = fork();
		if (pid == 0)
		{
						// std::cerr << "fork() failed" << std::endl;
			std::string	str;
			char		buff[1];
			delete (testServer);
			int temp;
			while ((temp = read(temp_sock, buff, 1)) == 1)
			{
				str.push_back(*buff);
			}
			write(temp_sock, str.c_str(), str.size());
			std::cout << temp_sock << std::endl;
			exit(0);
		}
		else
		{
			sendHTMLPage(temp_sock);
			close(temp_sock);
		}
		    // Raccolta dei processi figli completati
		while (waitpid(-1, NULL, WNOHANG) > 0)
			;

	}
	delete (testServer);
	return (0);
}
