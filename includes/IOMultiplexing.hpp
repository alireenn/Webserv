/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOMultiplexing.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 14:48:24 by mruizzo           #+#    #+#             */
/*   Updated: 2023/05/29 15:10:23 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IOMULTIPLEXING_HPP
#define IOMULTIPLEXING_HPP

#include <sys/epoll.h>



#include "Config.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Client;

class IOMultiplexing
{
private:
	int _fdmax;
	fd_set fdwrite;
    fd_set fdread;
	std::vector<std::pair<int ,int> > AlreadyBind;
	
public:
	IOMultiplexing(/* args */);
	~IOMultiplexing();
	
	void StartServers(Config &conf);
	void EventLoop(std::vector<Server> &Svec);
	void CreateServer(Server &server);
};



#endif