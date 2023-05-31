/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 10:47:59 by mruizzo           #+#    #+#             */
/*   Updated: 2023/05/31 14:23:05 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

Response::Response()
{
}

Response::~Response()
{
}

Response::Response(Request &request, Server &server, int client_fd)
{
	_request = request;
	_server = server;
	_client_fd = client_fd;
	len = 0;
	ok = 0;
	res_len = 0;
	fd = 0;
	size = 0;
	_send = 0;
	done = 0;
	en_handle = 0;
	post = 0;
	fd_upload = 0;
	len_server = 0;
	fd_error = 0;
	error = 0;
}

int Response::getClientFD() const
{
    return _client_fd;
}

int &Response::getDone(void)
{
    return done;
}

void Response::setDone(int done)
{
	this->done = done;
}

void Response::handler(fd_set& r, fd_set& w) 
{
	(void)r;
	(void) w;
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\n<html>\n<head>\n<style>\nspan {\nfont-size: 120px;\n}\n</style>\n</head>\n<body>\n<span>Vamos!</span>\n</body>\n</html>";

        const char* response_data = response.c_str();
        size_t response_length = response.length();
        ssize_t bytes_sent = send(_client_fd, response_data, response_length, 0);

        if (bytes_sent == -1) {
            std::cerr << "Error sending response to client" << std::endl;
        } else if (static_cast<size_t>(bytes_sent) < response_length) {
            // Not all data was sent, you may need to handle this case
            std::cerr << "Not all data was sent to client" << std::endl;
        }

        done = true;
}
