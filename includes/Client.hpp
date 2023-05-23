/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:19:58 by mruizzo           #+#    #+#             */
/*   Updated: 2023/05/23 12:53:00 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <vector>
#include <iostream>
#include "Server.hpp"
#include "Socket.hpp"

class Client
{
private:
	Server _server;
	Socket _client_socket;
public:
	Client();
	~Client();

	void	setServer(Server  &server_client);
	Server	&getServer();
	int		getSocketFd();
};

#endif