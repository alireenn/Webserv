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
	std::vector<Server> &Svec = conf.getServers();
	if (Svec.size())
		EventLoop(Svec);
	else
		std::cerr << "Bad config file " << conf.getFilePath();
}


void IOMultiplexing::EventLoop(std::vector<Server> &servers)
{
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) 
	{
        std::cerr << "Errore nella chiamata a epoll_create1" << std::endl;
        return;
    }

    epoll_event event;
    event.events = EPOLLIN; // Evento di lettura
    event.data.fd = this->_fdmax; // Il file descriptor da monitorare
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, this->_fdmax, &event) == -1) 
	{
        // Errore nell'aggiunta del file descriptor all'istanza epoll
        std::cerr << "Errore nell'aggiunta del file descriptor all'istanza epoll" << std::endl;	
        close(epoll_fd);
        return;
    }
						//ultima roba "sbobinata"
    std::vector<std::pair<Client, Request>> ClientRequest;
    std::vector<Response> ReadyResponse;

    while (1) {
        epoll_event events[this->_fdmax + 1];
        int ready_fds = epoll_wait(epoll_fd, events, this->_fdmax + 1, 3000); // Timeout di 3 secondi
        if (ready_fds == -1) {
            // Errore nella chiamata a epoll_wait
            // Gestisci l'errore di conseguenza
            close(epoll_fd);
            return;
        } else if (ready_fds == 0) {
            // Timeout scaduto, nessun evento disponibile
            continue;
        }

        for (int i = 0; i < ready_fds; i++) {
            int fd = events[i].data.fd;

            // Gestisci l'evento di lettura sul server
            for (size_t j = 0; j < servers.size(); j++) {
                int fdserver = servers[j].getSocket().getFd();
                if (fd == fdserver) {
                    // ...
                    // Gestisci l'accettazione del nuovo client
                    // ...

                    // Aggiungi il nuovo file descriptor del client all'istanza epoll
                    epoll_event client_event;
                    client_event.events = EPOLLIN | EPOLLET; // Evento di lettura con modalità edge-triggered
                    client_event.data.fd = fd_client;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd_client, &client_event) == -1) {
                        // Errore nell'aggiunta del file descriptor del client all'istanza epoll
                        // Gestisci l'errore di conseguenza
                        close(fd_client);
                    }

                    break;
                }
            }

            // Gestisci l'evento di lettura sui client
            for (size_t j = 0; j < ClientRequest.size(); j++) {
                if (fd == ClientRequest[j].first.getSocketFd()) {
                    // ...
                    // Gestisci la lettura dalla connessione del client
                    // ...

                    break;
                }
            }

            // Gestisci l'evento di scrittura sui client
            for (size_t j = 0; j < ReadyResponse.size(); j++) {
                if (fd == ReadyResponse[j].getClientFD()) {
                    // ...
                    // Gestisci la scrittura sulla connessione del client
                    // ...

                    break;
                }
            }
        }
    }

    close(epoll_fd);
}
