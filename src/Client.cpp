/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:23:25 by mruizzo           #+#    #+#             */
/*   Updated: 2023/05/23 12:52:32 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"
#include "Client.hpp"

Client::Client(){}

Client::~Client() 
{
	
}

int Client::getSocketFd()
{
   return (this->_client_socket.getFd());
}

// void Client::setSocketFd(int fd)
// {
//     this->_client_socket.setSocketFd(fd);
// }

int Client::getSocketFd()
{
	return (this->_client_socket.getFd());
}

void Client::setServer(Server & clinet_server)
{
    _server = clinet_server;
}

Server & Client::getServer()
{
    return _server;
}