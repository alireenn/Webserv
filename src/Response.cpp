/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 10:47:59 by mruizzo           #+#    #+#             */
/*   Updated: 2023/06/06 11:39:50 by mruizzo          ###   ########.fr       */
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
	using vecterr = std::vector<std::pair<std::string, std::string> >;
	vecterr	errorPages = this->_server.getErrorPages();

	std::cout << "\n\n\n\nHEY\n\n\n\n" << std::endl;
	for (vecterr::iterator it = errorPages.begin(); it != errorPages.end(); ++it)
	{
			if (it->first == code)
			{
					std::ifstream	page(it->second.c_str(), std::ifstream::in);
					std::string		toSend;
					std::string		line;

					if (!page.is_open())
							return (true);
					do
					{
						toSend += line;
						std::getline(page, line);
					} while (!line.empty());
					page.close();
					if (toSend.empty())
							return (true);
					std::cout << "\n\n\n\n" << toSend << "\n\n\n\n" << std::endl;
					send(_client_fd, toSend.c_str(), toSend.size(), 0);
					return (false);
			}
	}
	return (true);
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
	_path = deleteSpace(_request.GetRequest().at("Path"));
	if (!_location.getRedirection().second.empty())
	{
		std::cout << "302 Reindirizzamento temporaneo trovato" << std::endl;
		std::string response = "HTTP/1.1 302 Found\r\nLocation: " + _location.getRedirection().second + "\r\nContent-Length: 0\r\n\r\n";
		send(_client_fd, response.c_str(), response.length(), 0);
		FD_CLR(_client_fd, &w);
		FD_SET(_client_fd, &r);
		done = true;
		return false;
	}
	return true;
}

bool Response::handleMethod(fd_set &r, fd_set &w)
{
	_path = deleteSpace(_request.GetRequest().at("Path"));
	if (_path.find(_location.getLocationPath()) != -1) //c++ potrebbe rompere il cazzo per i tipi
	{
		if (!_location.getClientMaxBodySize().empty())
			len_server = strtoul(_location.getClientMaxBodySize().c_str(), NULL, 10);
		else
			len_server = -1;
		for (size_t i = 0; i < _location.getAllowedMethods().size(); i++)
		{
			if (deleteSpace(_location.getAllowedMethods().at(i)) == deleteSpace(_request.GetRequest().at("Method")))
				return true;
		}
		
		if (_request.GetRequest().at("Method") == "POST")
		{
			if (access(_request.getPathTmp().c_str(), F_OK) != -1)
				remove(_request.getPathTmp().c_str());
		}
		
		if (sendError("405", "Method Not Allowed"))
		{
			std::cout << "405 Method Not Allowed" << std::endl;
			std::string response = "HTTP/1.1 405 \r\nConnection: close\r\nContent-Length: 144\r\n\r\n";
			response += "<!DOCTYPE html>\n<html>\n<head>\n<style>\nspan {\nfont-size: 120px;\n}\n</style>\n</head>\n<body>\n<span>405 Method Not Allowed</span>\n</body>\n</html>";
			send(_client_fd, response.c_str(), response.length(), 0);
			FD_CLR(_client_fd, &w);
			FD_SET(_client_fd, &r);
			done = true;
		}
		return false;
	}
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
