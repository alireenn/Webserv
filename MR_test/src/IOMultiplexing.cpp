#include "../includes/IOMultiplexing.hpp"

IOMultiplexing::IOMultiplexing()
{
	_fdmax = 0;
}

IOMultiplexing::~IOMultiplexing()
{
}

void IOMultiplexing::CreateServer(Server &server)
{
    int fd;
    struct sockaddr_in *sockaddr = server.getSocket().getSockAddr();
    sockaddr->sin_family = AF_INET;
    sockaddr->sin_addr.s_addr = INADDR_ANY;
    sockaddr->sin_port = htons(server.getPort());

    int used = 0;
    int usedfd;

    for(size_t i=0; i < AlreadyBind.size(); i++)
    {
        if (AlreadyBind[i].second == server.getPort())
        {
            used = 1;
            usedfd = AlreadyBind[i].first;
        }
    }

    if (!used)
    {
        if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            std::cerr << "Errore nella creazione del socket" << std::endl;
            return;
        }
        AlreadyBind.push_back(std::make_pair(fd,server.getPort()));
        int val = 1;
        if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1)
        {
            std::cerr << "Errore nella chiamata a setsockopt" << std::endl;
            return;
        }
        fcntl(fd, F_SETFL, O_NONBLOCK);
        if (bind(fd, (struct sockaddr *)sockaddr, sizeof(*sockaddr)) == -1)
        {
            std::cerr << "Errore nella chiamata a bind" << std::endl;
            return;
        }
        if (listen(fd,SOMAXCONN) == -1)
        {
            std::cerr << "Errore nella chiamata a listen" << std::endl;
            return;
        }
        std::cout << "Server in ascolto sulla porta " << server.getPort() << std::endl;
        
        struct epoll_event event;
        event.events = EPOLLIN | EPOLLET;
        event.data.fd = fd;
        int epoll_fd = epoll_create1(0);
        if (epoll_fd == -1)
        {
            std::cerr << "Errore nella creazione dell'istanza epoll" << std::endl;
            return;
        }
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1)
        {
            std::cerr << "Errore nella chiamata a epoll_ctl" << std::endl;
            return;
        }
        server.getSocket().setSocketFd(fd);
        _fdmax = fd;
        FD_SET(fd, &fdread);
        server.setEpoll_fd(epoll_fd);// da capire
    }
    else
    {
        server.getSocket().setSocketFd(usedfd);
    }
}

void IOMultiplexing::StartServers(Config &conf)
{
	std::vector<Server> &Svec = conf.getServers();
	if (Svec.size())
    {
        for(size_t i = 0; i < Svec.size(); i++) 
            CreateServer(Svec[i]);
		EventLoop(Svec);
    }
	// else
	// 	std::cerr << "Bad config file " << conf.getFilePath();
}


void IOMultiplexing::EventLoop(std::vector<Server> &servers)
{
    int fd_client;
    // fd_set cpy_fdwrite, cpy_fdread;
    struct epoll_event event;
    std::vector<std::pair<Client, Request> > ClientRequest;
    std::vector<Response> ReadyResponse;
    while (1)
    {
        
        for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it)
        {
            Client new_client;
            sockaddr_in client_addr;
            socklen_t client_addr_size = sizeof(client_addr);  
            int fd_server = it->getSocket().getSocketFd();
            fd_client = accept(fd_server, (sockaddr *)&client_addr, &client_addr_size);
            if (fd_client != -1)
            {
                event.events = EPOLLIN; 
                event.data.fd = fd_client;
                epoll_ctl(it->getEpoll_fd(), EPOLL_CTL_ADD, fd_client, &event);
                new_client.setSocketFd(fd_client);
                new_client.setServer(*it);
                ClientRequest.push_back(std::make_pair(new_client, Request()));
                std::cout << "Nuova connessione da " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;
                FD_SET(fd_client, &this->fdread);
                if (fd_client > _fdmax)
                    _fdmax = fd_client;
            }
            int n_events = epoll_wait(it->getEpoll_fd(), &event, 10, 3000);
            if (n_events == -1)
            {
                std::cerr << "Errore nella chiamata a epoll_wait" << std::endl;
                continue;
            }
            else
            {
               char buffer[1024];
               ssize_t bytes_read = recv(event.data.fd, buffer, 1024, 0);
            //    if (bytes_read == -1)
            //    {
            //     //    std::cerr << "Errore nella lettura del messaggio" << std::endl;
            //        continue;
            //    }
            //    else if (bytes_read == 0)
            //    {
            //        std::cout << "Il client ha chiuso la connessione" << std::endl;
            //        continue;
            //    }
               if (bytes_read > 0)
               {
                //parte di codice che gestisce la richiesta, cambierà in base a come implementeremo la classe Response e request
                     std::cout << "Messaggio ricevuto: " << buffer << std::endl;
                     std::string response = "HTTP/1.1 200 OK\r\n\r\n<html><body><h1>Vamos!</h1></body></html>";
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
