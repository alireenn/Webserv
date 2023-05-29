/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 13:15:07 by ccantale          #+#    #+#             */
/*   Updated: 2023/05/29 15:13:13 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_H
# define SOCKET_H


#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

class Socket{
    private:
        int _socket_fd;
        struct sockaddr_in  *_sockaddr;
    public:
        Socket();
        Socket(const Socket &rhs);
        Socket &operator=(const Socket &rhs);
        ~Socket();
        struct sockaddr_in *getSockAddr() const;
        int getSocketFd() const;
        void setSocketFd(int fd);
};
#endif
