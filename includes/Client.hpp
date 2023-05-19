/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 12:19:58 by mruizzo           #+#    #+#             */
/*   Updated: 2023/05/19 12:38:34 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <vector>
#include <iostream>
#include "Server.hpp"

class Client
{
private:
	Server *_server;
public:
	Client();
	~Client();

	void	setServer(Server *server);
	int		getSocketFd(void) const;
};

#endif