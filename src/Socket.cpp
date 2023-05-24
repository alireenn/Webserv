/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 16:59:33 by ccantale          #+#    #+#             */
/*   Updated: 2023/05/24 18:56:48 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdio.h>
#include "../includes/Socket.hpp"

static int	init_socket(struct sockaddr_in *addr)
{
	int			fd;
	int			val	= 1;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == 0)
        {
		std::cerr << "Socket creation ";
		return (-1);
	}
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1)
	{
		std::cerr << "setsockopt() ";
		return (-1);
	}
	//fcntl(fd, F_SETFL, O_NONBLOCK);
	if (bind(fd, (struct sockaddr *)addr, sizeof(*addr)) == -1)
	{
		perror("binf ");
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

	this->_running = false;
	memset(&_addr, '\0', sizeof(_addr));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(port);
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	try
	{
		_fd = init_socket(&_addr);
		if (_fd < 0)
			throw SocketFailException();
		this->_port = port;
		this->_running = true;
		//io.setFdRead(fd); // da risistemare
		//io.setFdMax(fd); // da risistemare
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

Socket::~Socket(void)
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

struct sockaddr_in	&Socket::getAddr(void)
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

void Socket::setSocketFd(int fd)
{
    this->_fd = fd;
}