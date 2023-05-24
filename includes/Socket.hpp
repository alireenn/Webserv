/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccantale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 13:15:07 by ccantale          #+#    #+#             */
/*   Updated: 2023/05/24 18:29:14 by ccantale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_H
# define SOCKET_H

# include <iostream>
# include <cstring>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include "log.hpp"

class	Socket
{
	public:
				/*
				** After creation, make sure to
				** check that isRunning() == true.
				*/
					Socket(int port);
					Socket(Socket &toCopy);
					~Socket(void);
		Socket			&operator=(Socket &toCopy);
		struct sockaddr_in	&getAddr(void);
		int			getFd(void) const;
		int			getPort(void) const;
		bool			isRunning(void) const;

		class SocketFailException: public std::exception
		{
			public:
				const char	*what(void) const throw()
				{
					return (" failed. Port nbr: ");
				}
		};

	private:
		struct sockaddr_in	_addr;
		int			_fd;
		int			_port;
		bool			_running;
					Socket(void);
};
		
#endif
