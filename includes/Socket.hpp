/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccantale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 13:15:07 by ccantale          #+#    #+#             */
/*   Updated: 2023/05/13 18:22:59 by ccantale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_H
# define SOCKET_H

# include <iostream>
# include <cstring>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>
//# include "Da dovunque venga la socketList che serve al costruttore"

class	Socket
{
	public:
				/* after creation, make sure to
				** check that getFd() != -1.
				** Alternatively, you can check that
				** isRunning() == true. Same same.
				*/
					Socket(int port);
					Socket(Socket &toCopy);
					~Socket(void);
		Socket			&operator=(Socket &toCopy);
		struct sockaddr_in	&getAddr(void) const;
		int			&getFd(void) const;
		int			&getPort(void) const;
		bool			isRunning(void) const;

		class	PortRepetitionException: public std::exception
		{
			public:
				PortRepetitionException(void);
				virtual const char	*what(void) const throw()
				{
					return ("Port repeted. Port nbr: ");
				}
		};
		class SocketFailException: public std::exception
		{
			public:
				SocketFailException(void);
				virtual const char	*what(void) const throw()
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
