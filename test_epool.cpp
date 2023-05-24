#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <cstring>

#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket, epoll_fd;
    struct sockaddr_in address{};
    char buffer[BUFFER_SIZE];
    struct epoll_event event, events[MAX_EVENTS];

    // Creazione del socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Errore nella creazione del socket" << std::endl;
        return 1;
    }

    // Impostazione dell'indirizzo e della porta del server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // Binding del socket all'indirizzo e alla porta
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Errore nel binding del socket" << std::endl;
        return 1;
    }

    // Ascolto delle connessioni in arrivo
    if (listen(server_fd, 5) < 0) {
        std::cerr << "Errore nell'ascolto delle connessioni" << std::endl;
        return 1;
    }

    // Creazione dell'istanza epoll
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "Errore nella creazione dell'istanza epoll" << std::endl;
        return 1;
    }

    // Aggiunta del socket del server all'istanza epoll
    event.events = EPOLLIN;
    event.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        std::cerr << "Errore nell'aggiunta del socket del server all'istanza epoll" << std::endl;
        return 1;
    }

    std::cout << "Il server è in ascolto sulla porta 8080..." << std::endl;

    while (true) {
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_events == -1) {
            std::cerr << "Errore nell'attesa degli eventi epoll" << std::endl;
            return 1;
        }

        for (int i = 0; i < num_events; ++i) {
            if (events[i].data.fd == server_fd) {
                // Nuova connessione in arrivo
                new_socket = accept(server_fd, nullptr, nullptr);
                if (new_socket == -1) {
                    std::cerr << "Errore nell'accettazione della nuova connessione" << std::endl;
                    return 1;
                }

                // Aggiunta del nuovo socket client all'istanza epoll
                event.events = EPOLLIN;
                event.data.fd = new_socket;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1) 
				{
                // Aggiunta del nuovo socket client all'istanza epoll
                event.events = EPOLLIN;
                event.data.fd = new_socket;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1) {
                    std::cerr << "Errore nell'aggiunta del nuovo socket client all'istanza epoll" << std::endl;
                    return 1;
                }

                std::cout << "Nuova connessione accettata. Socket: " << new_socket << std::endl;
            } else {
                // Lettura dei dati dal socket client
					char buffer[BUFFER_SIZE];
					ssize_t num_bytes = recv(new_socket ,buffer, BUFFER_SIZE - 1, 0);
					if (num_bytes == -1) {
						std::cerr << "Errore nella lettura dal socket client" << std::endl;
						return 1;
					} else if (num_bytes == 0) {
						// Connessione chiusa dal client
						std::cout << "Connessione chiusa. Socket: " << new_socket << std::endl;
						close(new_socket);
					} else {
						// Elaborazione dei dati ricevuti
						buffer[num_bytes] = '\0'; // Aggiungi il terminatore di stringa
						std::cout << "Dati ricevuti dal socket \n" << new_socket << ": " << buffer << std::endl;

						// Invio di una risposta al client
						const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
						send(new_socket, response, strlen(response), 0);
					}

                }
            }
        }
    }

    // Chiusura dei socket e dell'istanza epoll
    close(server_fd);
    close(epoll_fd);

    return 0;
}


				
