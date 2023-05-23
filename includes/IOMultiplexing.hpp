/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOMultiplexing.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 14:48:24 by mruizzo           #+#    #+#             */
/*   Updated: 2023/05/23 16:24:13 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IOMULTIPLEXING_HPP
#define IOMULTIPLEXING_HPP

#include <sys/epoll.h>

#include "Config.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Request.hpp"

class IOMultiplexing
{
private:
	int _fdmax;
public:
	IOMultiplexing(/* args */);
	~IOMultiplexing();
	
	void StartServers(Config &conf);
	void EventLoop(std::vector<Server> &Svec);
};



#endif