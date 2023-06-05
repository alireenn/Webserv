/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 10:47:59 by mruizzo           #+#    #+#             */
<<<<<<< HEAD
<<<<<<< HEAD
/*   Updated: 2023/06/05 16:48:10 by ccantale         ###   ########.fr       */
=======
/*   Updated: 2023/06/05 16:54:11 by mruizzo          ###   ########.fr       */
>>>>>>> f5307309cebc9a43493d93a3001f3cc6cfd73967
=======
/*   Updated: 2023/06/05 16:59:10 by mruizzo          ###   ########.fr       */
>>>>>>> c9f8ca6771b1302baf939c281c2875c7595698e3
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

static std::string deleteSpace(std::string str)
{
	for (int i = 0; str[i]; i++)
        if (str[i] == ' ')
            str.erase(i, 1);
    return str;
}

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

static std::string	getDate(void)
{
	time_t		rawtime;
	struct tm	*timeInfo;
	char		buffer[80];
	std::string	ret;

	time(&rawtime);
	timeInfo = localtime (&rawtime);

	strftime(buffer, 80, "%a,%e %b %G %T GMT+2", timeInfo);
	ret = buffer;
	return (ret);
}

void Response::test(fd_set& r, fd_set& w) 
{
	(void)r;
	(void)w;

	std::string	date = getDate();
    std::string response = "ciao" + date; //"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\n<html>\n<head>\n<style>\nspan {\nfont-size: 120px;\n}\n</style>\n</head>\n<body>\n<span>Vamos!</span>\n</body>\n</html>";

	std::cout << date << std::endl;
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

bool Response::sendError(std::string code, std::string msg)
{
	//da completare
	return true;
}

bool Response::isValid(fd_set &r, fd_set &w)
{
	std::string method = _request.GetRequest().at("Method");
    std::string version = _request.GetRequest().at("Version");
    if (method != "GET" && method != "POST" && method != "PUT" && method != "PATCH" && method != "DELETE" && method != "COPY" && method != "HEAD" && method != "OPTIONS" && method != "LINK" && method != "UNLINK" && method != "PURGE" && method != "LOCK" && method != "UNLOCK" && method != "PROPFIND" && method != "VIEW" && version != "HTTP/1.1" && version != "HTTP/1.0" && version != "HTTP/2.0" && version != "HTTP/3.0")
	{
		if (sendError("400" , "Bad Request"))
		{
			std::cout << "400 Bad Request" << std::endl;
			std::string response = "HTTP/1.1 400 \r\nConnection: close\r\nContent-Length: 133\r\n\r\n";
			response += "<!DOCTYPE html>\n<html>\n<head>\n<style>\nspan {\nfont-size: 120px;\n}\n</style>\n</head>\n<body>\n<span>400 Bad Request</span>\n</body>\n</html>";
			send(_client_fd, response.c_str(), response.length(), 0);
			FD_CLR(_client_fd, &w);
			FD_SET(_client_fd, &r);
			done = true;
			return false;
		}
		
	}
	return true;
}

bool Response::isSubjectCompliant(fd_set &r, fd_set &w)
{
	std::string method = _request.GetRequest().at("Method");
    std::string version = _request.GetRequest().at("Version");
	if (method != "GET" && method != "POST" && method != "DELETE")
	{
		if (sendError("501", "Method not Implemented"))
		{
			std::cout << "501 Method not Implemented" << std::endl;
			std::string response = "HTTP/1.1 501 \r\nConnection: close\r\nContent-Length: 144\r\n\r\n";
			response += "<!DOCTYPE html>\n<html>\n<head>\n<style>\nspan {\nfont-size: 120px;\n}\n</style>\n</head>\n<body>\n<span>501 Method not Implemented</span>\n</body>\n</html>";
			send(_client_fd, response.c_str(), response.length(), 0);
			FD_CLR(_client_fd, &w);
			FD_SET(_client_fd, &r);
			done = true;
		}
		return false;
	}
	if (version != "HTTP/1.1" && version != "HTTP/1.0" )
	{
		if (sendError("505", "HTTP Version not Supported"))
		{
			std::cout << "505 HTTP Version not Supported" << std::endl;
			std::string response = "HTTP/1.1 505 \r\nConnection: close\r\nContent-Length: 148\r\n\r\n";
			response += "<!DOCTYPE html>\n<html>\n<head>\n<style>\nspan {\nfont-size: 120px;\n}\n</style>\n</head>\n<body>\n<span>505 HTTP Version not Supported</span>\n</body>\n</html>";
			send(_client_fd, response.c_str(), response.length(), 0);
			FD_CLR(_client_fd, &w);
			FD_SET(_client_fd, &r);
			done = true;
		}
		return false;
	}
	return true;
}

bool Response::redirectPath(fd_set &r, fd_set &w)
{
	std::cout << "REDIRECT PATH da scrivere" << std::endl;
	return true;
}

bool Response::checkForbidden(fd_set &r, fd_set &w)
{
	std::cout << "CHECK FORBIDDEN da scrivere" << std::endl;
	return true;
}

bool Response::handleAutoIndex(fd_set &r, fd_set &w)
{
	std::cout << "HANDLE AUTOINDEX da scrivere" << std::endl;
	return true;
}

bool Response::handleIndex()
{
	std::cout << "HANDLE INDEX da scrivere" << std::endl;
	return true;
}

void Response::sendData(fd_set &r, fd_set &w)
{
	std::cout << "SEND DATA da scrivere" << std::endl;
}

bool Response::checkLocation(fd_set &r, fd_set &w)
{
	std::cout << "CHECK LOCATION da scrivere" << std::endl;
	return true;
}

bool Response::handleRedirection(fd_set &r, fd_set &w)
{
	std::cout << "HANDLE REDIRECTION da scrivere" << std::endl;
	return true;
}

bool Response::handleMethod(fd_set &r, fd_set &w)
{
	std::cout << "HANDLE METHOD da scrivere" << std::endl;
	return true;
}

void Response::handler(fd_set &r, fd_set &w)
{
	if (!ok)
		_full_path = _path = deleteSpace(_request.GetRequest().at("Path"));
	if (ok || (isValid(r,w) && isSubjectCompliant(r,w) && checkLocation(r,w)))
	{
		if(ok || (handleRedirection(r,w) && handleMethod(r,w)))
		{
			std::string tmp = deleteSpace(_request.GetRequest().at("Method"));
			if(tmp == "GET")
			{
				if (ok || (redirectPath(r,w) && checkForbidden(r,w)))
					if(ok || handleIndex() || handleAutoIndex(r,w))
						sendData(r,w);
			}
			else if (tmp == "POST")
			{
				std::cout << "POST da scrivere" << std::endl;
			}
			else if (tmp == "DELETE")
			{
				std::cout << "DELETE da scrivere" << std::endl;
			}
		}
	}	
}
