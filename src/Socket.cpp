/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccantale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 16:59:33 by ccantale          #+#    #+#             */
/*   Updated: 2023/05/13 18:00:09 by ccantale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

static bool	check_port_repetition(int *fd, int port,
			std::vector<Socket &> &socketList, Socket &new_socket)
{
	for (Socket &s: socketList)
	{
		if (s.getPort() == port)
		{
			*fd = s.getFd();
			socketList.push_back(new_socket);
			return (false);
		}
	}
	return (true);
}

static int	init_socket(struct sockaddr_in *addr)
{
	int			fd;
	int			val;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
        {
		std::cerr << "Socket creation ";
		return (-1);
	}
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, 1) == -1)
	{
		std::cerr << "setsockopt() ";
		return (-1);
	}
	fcntl(fd, F_SETFL, O_NONBLOCK);
	if (bind(fd, static_cast<struct sockaddr *>(_addr), sizeof(*_addr)) == -1)
	{
		std::cerr << "bind() ";
		return (-1);
	}
	if (listen(fd, SOMAXCONN) == -1)
	{
		std::cerr << "listen() ";
		return (-1);
	}
	return (fd);
}

Socket::Socket(int port)
{
	std::vector<Socket &>	&socketList = #@%$::getSocketList(); // da definire

	_running = false;
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(port);
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	try
	{
		if (!check_port_repetition(&_fd, port, socketList, *this))
			throw PortRepetitionException();
		_fd = init_socket(&_addr);
		if (_fd < 0)
			throw SocketFailException();
		_running = true;
		std::cout << "New server started listening on port " << port << std::endl;
		socketList.push_back(*this);
		io.setFdRead(fd); // da definire
		io.setFdMax(fd); // da definire
	}
	catch (PortRepetitionException &e) {
		_running = true;
		std::cerr << e.what() << port << "." << std::endl;
	}
	catch (SocketFailException &e) {
		std::cerr << e.what() << port << "." << std::endl;
	}
}

Socket::Socket(Socket &toCopy)
{
	this->_addr = toCopy._addr;
	this->_fd = toCopy._fd;
	this->_port = toCopy._port;
}

Socket::~Socket
{
	close(_fd);
}

Socket	&Socket::operator=(Socket &toCopy)
{
	this->_addr = toCopy._addr;
	this->_fd = toCopy._fd;
	this->_port = toCopy._port;
	return (*this);
}

struct sockaddr_in	&Socket::getAddr(void) const
{
	return (_addr);
}

int	Socket::getFd(void) const
{
	return (_fd);
}

int	Socket::getPort(void) const
{
	return (_port);
}

bool	Socket::isRunning(void) const
{
	return (_running);
}
