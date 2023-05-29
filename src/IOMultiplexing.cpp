#include "../includes/IOMultiplexing.hpp"

IOMultiplexing::IOMultiplexing()
{
	_fdmax = 0;
}

IOMultiplexing::~IOMultiplexing()
{
}

void IOMultiplexing::StartServers(Config &conf)
{
	std::vector<Server *> &Svec = conf.getServers();
	if (Svec.size())
    {

		EventLoop(Svec);
    }
	// else
	// 	std::cerr << "Bad config file " << conf.getFilePath();
}


void IOMultiplexing::EventLoop(std::vector<Server *> &servers)
{
    int fd_client;
    // fd_set cpy_fdwrite, cpy_fdread;
    struct epoll_event event;
    int epoll_fd=epoll_create1(0);
    std::vector<std::pair<Client, Request> > ClientRequest;
    std::vector<Response> ReadyResponse;
    while (1)
    {
        if (epoll_fd == -1)
        {
            std::cerr << "Errore nella creazione dell'istanza epoll" << std::endl;
            exit(1);
        }
        // for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it)
        for (std::vector<Server *>::iterator it = servers.begin(); it != servers.end(); ++it) //<- bleah
        {
            Client new_client;
            sockaddr_in client_addr;
            socklen_t client_addr_size = sizeof(client_addr);   
            // fd_client = accept(it->getFd(), (struct sockaddr *)&client_addr, &client_addr_size);
            fd_client = accept((*it)->getFd(), (struct sockaddr *)&client_addr, &client_addr_size); // <- bleah
            if (fd_client != -1)
            {
                event.events = EPOLLIN; 
                event.data.fd = fd_client;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd_client, &event);
                new_client.setSocketFd(fd_client);
                // new_client.setServer(*it);
                new_client.setServer(**it); // <- bleah
                // ClientRequest.push_back(std::make_pair(new_client, Request()));
                std::cout << "Nuova connessione da " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;
                FD_SET(fd_client, &this->fdread);
                if (fd_client > _fdmax)
                    _fdmax = fd_client;
            }
            int n_events = epoll_wait(epoll_fd, &event, 10, 3);
            if (n_events == -1)
            {
                std::cerr << "Errore nella chiamata a epoll_wait" << std::endl;
                continue;
            }
            else
            {
               char buffer[1024];
               ssize_t bytes_read = recv(event.data.fd, buffer, 1024, 0);
               if (bytes_read == -1)
               {
                //    std::cerr << "Errore nella lettura del messaggio" << std::endl;
                   continue;
               }
               else if (bytes_read == 0)
               {
                   std::cout << "Il client ha chiuso la connessione" << std::endl;
                   continue;
               }
               else
               {
                //parte di codice che gestisce la richiesta, cambierà in base a come implementeremo la classe Response e request
                     std::cout << "Messaggio ricevuto: " << buffer << std::endl;
                     std::string response = "HTTP/1.1 200 OK\r\n\r\n<html><body><h1>It works!</h1></body></html>";
                     ssize_t bytes_sent = send(event.data.fd, response.c_str(), response.size(), 0);
                     if (bytes_sent == -1)
                     {
                          std::cerr << "Errore nell'invio del messaggio" << std::endl;
                          continue;
                     }
                     else
                     {
                          std::cout << "Messaggio inviato: " << response << std::endl;
                     }
               }
            }
        }
    }
}
