/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOMultiplexing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 16:49:44 by ccantale          #+#    #+#             */
/*   Updated: 2023/06/28 18:05:48 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IOMultiplexing.hpp"



//scrivi una funzione che lancia un suono ogni volta che viene chiamata
void playSound()
{
	std::cout << "\a";
}

static std::string strtrim(std::string str)
{
	size_t first = str.find_first_not_of(' ');
	if (std::string::npos == first)
	{
		return str;
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

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
		if (AlreadyBind[i].second == (int)server.getPort())
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
		AlreadyBind.push_back(std::pair<int,int>(fd,server.getPort()));
		int val = 1;
		if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1)
		{
			std::cerr << "Errore nella chiamata a setsockopt" << std::endl;
			return;
		}
		fcntl(fd, F_SETFL, O_NONBLOCK);//torna -1 se fallisce
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

		server.getSocket().setSocketFd(fd);
		FD_SET(fd, &fdread);
		_fdmax = fd;
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

void IOMultiplexing::EventLoop(std::vector<Server>& servers)
{
	std::vector<int> epoll_fds(servers.size());
	std::vector<fd_set> fdreads(servers.size());
	std::vector<fd_set> fdwrites(servers.size());
	std::vector<int> fdmaxs(servers.size());
	std::vector<std::vector<epoll_event> > events(servers.size());

	std::vector<std::pair<Client, Request> > ClientsRequest;
	std::vector<Response> ResponseReady;

	for (size_t i = 0; i < servers.size(); i++)
	{
		int epoll_fd = epoll_create1(0);
		if (epoll_fd == -1)
		{
			std::cerr << "Errore nella creazione dell'istanza epoll per il server " << i << std::endl;
			continue;
		}

		fd_set fdread;
		FD_ZERO(&fdread);
		int fdmax = servers[i].getSocket().getSocketFd();
		FD_SET(fdmax, &fdread);

		fd_set fdwrite;
		FD_ZERO(&fdwrite);


		epoll_event event;
		event.events = EPOLLIN | EPOLLET;
		event.data.fd = fdmax;
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fdmax, &event) == -1)
		{
			std::cerr << "Errore nella chiamata a epoll_ctl per il server " << i << std::endl;
			close(epoll_fd);
			continue;
		}

		epoll_fds[i] = epoll_fd;
		fdreads[i] = fdread;
		fdwrites[i] = fdwrite;
		fdmaxs[i] = fdmax;
		events[i].resize(fdmax + 1);
	}

	fd_set fdwrite_cp;

	while (true)
	{
		for (size_t i = 0; i < servers.size(); i++)
		{
			// fd_set fdread_cp = fdreads[i];
			fdwrite_cp = fdwrites[i];
			int epoll_fd = epoll_fds[i];
			int fdmax = fdmaxs[i];
			std::vector<epoll_event>& server_events = events[i];

			int fd_pronti = epoll_wait(epoll_fd, server_events.data(), fdmax + 1, 10);
			if (fd_pronti == -1)
			{
				std::cerr << "Errore nella chiamata a epoll_wait per il server " << i << std::endl;
				close(epoll_fd);
				continue;
			}
			else if (fd_pronti == 0)
			{
				// std::cout << "Timeout scaduto per il server " << i << std::endl;
				continue;
			}

			for (int j = 0; j < fd_pronti; j++)
			{
				int fd = server_events[j].data.fd;
				if (fd == servers[i].getSocket().getSocketFd())
				{
					struct sockaddr_in client_addr;
					socklen_t client_len = sizeof(client_addr);
					int client_fd = accept(fd, (struct sockaddr*)&client_addr, &client_len);
					if (client_fd == -1)
					{
						std::cerr << "Errore nella chiamata a accept" << std::endl;
						return;
					}
					fcntl(client_fd, F_SETFL, O_NONBLOCK);
					server_events[j].events = EPOLLIN | EPOLLET;
					server_events[j].data.fd = client_fd;
					if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &server_events[j]) == -1)
					{
						std::cerr << "Errore nella chiamata a epoll_ctl" << std::endl;
						return;
					}
					Client client;
					client.setSocketFd(client_fd);
					client.setServer(servers[i]);
					Request request;
					ClientsRequest.push_back(std::make_pair(client, request));
					std::cout << "Nuova connessione accettata sul server " << i << std::endl;
					FD_SET(client_fd, &fdreads[i]);
					FD_SET(client_fd, &fdwrites[i]);
					if (client_fd > fdmaxs[i])
						fdmaxs[i] = client_fd;
				}
				else
				{
					for (size_t k = 0; k < ClientsRequest.size(); k++)
					{
						if (fd == ClientsRequest[k].first.getSocketFd())
						{
							char buffer[10002];
							int nread = recv(fd, buffer, 10000, 0);
							if (nread == -1)
							{
								FD_CLR(ClientsRequest[k].first.getSocketFd(), &fdreads[k]);
								close(ClientsRequest[k].first.getSocketFd());
								ClientsRequest.erase(ClientsRequest.begin() + k);
							}
							else if (nread == 0)
							{
								std::cout << "Client disconnesso" << std::endl;
								FD_CLR(ClientsRequest[k].first.getSocketFd(), &fdreads[k]);
								close(ClientsRequest[k].first.getSocketFd());
								ClientsRequest.erase(ClientsRequest.begin() + k); // k o i? Questo e' il problema...
							}
							else
							{
								if (ClientsRequest[k].second.getOk() == 0)
									ClientsRequest[k].second = Request();
								ClientsRequest[k].second.setLength(nread);
								ClientsRequest[k].second.handleRequest(buffer);
								if (ClientsRequest[k].second.getFinished() == 1)
								{
									std::cout << "Richiesta ricevuta sul server " << i << std::endl;
									bool found = false;
									Response response;
									FD_CLR(fd, &fdreads[k]);
									FD_SET(fd, &fdwrites[k]);
									for (size_t l = 0; l < servers.size(); l++)
									{
										for (size_t x = 0; x < servers[l].getServerNames().size(); x++)
										{
											std::cout << "Server " << l << " nome " << servers[l].getServerNames()[x] << std::endl;
											std::cout << "Host " << strtrim(ClientsRequest[k].second.GetRequest().at("Host")) << std::endl;
											if (servers[l].getServerNames()[x] == strtrim(ClientsRequest[k].second.GetRequest().at("Host")))
											{
												std::cout << "Server " << l << " trovato" << std::endl;
												response = Response(ClientsRequest[k].second, servers[l], ClientsRequest[k].first.getSocketFd());
												ResponseReady.push_back(response);
												found = true;
												break;
											}
										}
									}

									if (!found)
									{
										for (size_t l = 0; l < servers.size(); l++)
										{
											if (servers[l].getServerNames().empty() && servers[l].getPort() == ClientsRequest[k].first.getServer().getPort())
											{
												response = Response(ClientsRequest[k].second, servers[l], ClientsRequest[k].first.getSocketFd());
												ResponseReady.push_back(response);
												found = true;
												break;
											}
										}
									}
									if (!found)
									{
										response = Response(ClientsRequest[k].second, ClientsRequest[k].first.getServer(), ClientsRequest[k].first.getSocketFd());
										ResponseReady.push_back(response);
									}
								}
							}
						}
					}
				}
			}
		}


		for (size_t j = 0; j < ResponseReady.size(); j++)
		{
			for(size_t i=0; i<servers.size(); i++)
			{

				if(FD_ISSET(ResponseReady[j].getClientFD(), &fdwrites[i]))
				{
					ResponseReady[j].handler(fdwrites[i], fdreads[i]);
					if (ResponseReady[j].getDone() == true)
					{
						ResponseReady.erase(ResponseReady.begin() + j);
						playSound();
						std::cout << "Richiesta inviata sul server " << i << std::endl;
					}
					break;
				}
			}
		}
	}
}

