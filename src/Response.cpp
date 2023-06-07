/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 10:47:59 by mruizzo           #+#    #+#             */
/*   Updated: 2023/06/07 12:02:34 by mruizzo          ###   ########.fr       */
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

static int comp(const char *str, const char *path)
{
    int i = 0;
    while(str[i] && path[i])
    {
        if (str[i] == path[i])
            i++;
        else
            break;
    }
    if ((!path[i] && !str[i]) || ( !str[i] && path[i] == '/'))
        return 1;
    return 0;
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
    std::string response = "HTTP/1.1 200 OK\rContent-Type: text/html\r\nDate: " + date + "\r\n\r\n<!DOCTYPE html>\n<html>\n<head>\n<style>\nspan {\nfont-size: 120px;\n}\n</style>\n</head>\n<body>\n<span>Vamos!</span>\n</body>\n</html>";

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
	typedef std::vector<std::pair<std::string, std::string> > t_err;
	t_err	errorPages = this->_server.getErrorPages();

	std::cout << "\n\n\n\nHEY\n\n\n\n" << std::endl;
	std::cout << msg << std::endl;
	for (t_err::iterator it = errorPages.begin(); it != errorPages.end(); ++it)
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
			done = true;
			return (false);
		}
	}
	return (true);
}

bool Response::isValid(fd_set &r, fd_set &w)
{
	std::string method = _request.GetRequest().at("Method");
    std::string version = _request.GetRequest().at("Version");
	// std::cout << "HELLO    " << _request.GetRequest().at("Method") << std::endl;

	
    if (method != "GET" && method != "POST" && method != "PUT" && method != "PATCH" && method != "DELETE" && method != "COPY" && method != "HEAD" && method != "OPTIONS" && method != "LINK" && method != "UNLINK" && method != "PURGE" && method != "LOCK" && method != "UNLOCK" && method != "PROPFIND" && method != "VIEW" && version != "HTTP/1.1" && version != "HTTP/1.0" && version != "HTTP/2.0" && version != "HTTP/3.0")
	{
		if (sendError("400" , "Bad Request"))
		{
			std::cout << "400 Bad Request" << std::endl;
			std::string response = "HTTP/1.1 400 \r\nConnection: close\r\nContent-Length: 121\r\n\r\n";
			response += "<!DOCTYPE html><html><head><style>span {font-size: 120px;}</style></head><body><span>400 Bad Request</span></body></html>";
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
			std::string response = "HTTP/1.1 501 \r\nConnection: close\r\nContent-Length: 132\r\n\r\n";
			response += "<!DOCTYPE html><html><head><style>span {font-size: 120px;}</style></head><body><span>501 Method not Implemented</span></body></html>";
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
			std::string response = "HTTP/1.1 505 \r\nConnection: close\r\nContent-Length: 136\r\n\r\n";
			response += "<!DOCTYPE html><html><head><style>span {font-size: 120px;}</style></head><body><span>505 HTTP Version not Supported</span></body></html>";
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
	struct stat		fileStat; // La struttura stat contiene i metadati relativi a un file o a una directory, come ad esempio le informazioni sulle autorizzazioni, le dimensioni, le informazioni sul timestamp e altro ancora.

	stat(_full_path.c_str(), &fileStat); // la struttura stat verrà popolata con i dati corrispondenti al file specificato
	if ((access(_full_path.c_str(),F_OK) != -1 //il file esiste?
		&& S_ISDIR(fileStat.st_mode)) //il file è una directory?
		&& _path != "/"  && _path[_path.length() - 1] != '/') //il path non è la root e non termina con '/'?
	{
		std::cout << "301 Reindirizzamento permanente trovato" << std::endl;
		std::string response = "HTTP/1.1 301 Moved Permanently\r\nLocation: " + _path + "/\r\nContent-Length: 0\r\n\r\n";
		send(_client_fd, response.c_str(), response.length(), 0);
		FD_CLR(_client_fd, &w);
		FD_SET(_client_fd, &r);
		done = true;
		return false;
	}
	return true;
}

bool Response::checkForbidden(fd_set &r, fd_set &w)
{
	struct stat fileStat;
		
	stat(_full_path.c_str(), &fileStat);
	if(access(_full_path.c_str(),F_OK) != -1 && access(_full_path.c_str(),R_OK) == -1)
	{
		if((sendError("403","Forbidden")))
		{
			std::cout << "403 Accesso alla risorsa richiesta è vietato" << std::endl;
			std::string response = "HTTP/1.1 403 Forbidden\r\nConnection: close\r\nContent-Length: 119\r\n\r\n";
			response += "<!DOCTYPE html><html><head><style>span {font-size: 120px;}</style></head><body><span>403 Forbidden</span></body></html>";
			send(_client_fd, response.c_str(), response.length(), 0);
			FD_CLR(_client_fd, &w);
			FD_SET(_client_fd, &r);
			done = true;
			return false;
		}
		return false;
	}
	return true;
}

bool Response::handleAutoIndex(fd_set &r, fd_set &w)
{
	(void)r;
	(void)w;
	std::cout << "HANDLE AUTOINDEX da scrivere" << std::endl;
	return false;
}

bool Response::handleIndex()
{
	struct stat fileStat;
	stat(_full_path.c_str(), &fileStat);
	if (access(_full_path.c_str(), F_OK) != -1 && S_ISDIR(fileStat.st_mode))
		return true;
	for (size_t i = 0; i < _location.getIndex().size(); i++)
	{
		_full_path += "/" + _location.getIndex().at(i);
		if (access(_full_path.c_str(), F_OK) != -1)
			return true;
	}
	return false;
}

void Response::sendData(fd_set &r, fd_set &w)
{
	(void)r;
	(void)w;
	//startCgi();
	struct stat fileStat;

	stat(_full_path.c_str(), &fileStat);
	if (!ok)
	{
		/* code */
	}
	
}

bool Response::checkLocation(fd_set &r, fd_set &w)
{
	for (size_t i = 0; i < _server.getLocations().size(); i++)
	{
		if(comp(_server.getLocations().at(i).getLocationPath().c_str(), _path.c_str()))
		{
			_location = _server.getLocations().at(i);
			std::string tmp = _path;
			_full_path = _location.getRoot() + tmp.replace(tmp.find(_server.getLocations().at(i).getLocationPath()), _server.getLocations().at(i).getLocationPath().length(), "");
			char buff[1024];
			realpath(_full_path.c_str(), buff);
			_full_path = buff;
			if (_request.GetRequest().at("Method") == "DELETE")
			{
				std::string tmp = _path;
				_path = _location.getRoot() + tmp.replace(tmp.find(_server.getLocations().at(i).getLocationPath()), _server.getLocations().at(i).getLocationPath().length(), "");
				char buff[1024];
				realpath(_path.c_str(), buff);
				_full_path = buff;
			}
			return true;
		}
	}

	//se non trovo la location, allora cerco quella di default
	for (size_t i = 0; i < _server.getLocations().size(); i++)
	{
		if(_server.getLocations().at(i).getLocationPath() == "/")
		{
			_location = _server.getLocations().at(i);
			std::string tmp = _path;
			_full_path = _location.getRoot() + tmp.replace(tmp.find(_server.getLocations().at(i).getLocationPath()), _server.getLocations().at(i).getLocationPath().length(), "");
			char buff[1024];
			realpath(_full_path.c_str(), buff);
			_full_path = buff;
			if (_request.GetRequest().at("Method") == "DELETE")
			{
				std::string tmp = _path;
				_path = _location.getRoot() + tmp.replace(tmp.find(_server.getLocations().at(i).getLocationPath()), _server.getLocations().at(i).getLocationPath().length(), "");
				char buff[1024];
				realpath(_path.c_str(), buff);
				_full_path = buff;
			}
			return true;
		}
	}
	if (_request.GetRequest().at("Method") == "POST")
	{
		if (access(_request.getPathTmp().c_str(), F_OK) != -1)
			remove(_request.getPathTmp().c_str());
	}
	if (sendError("404", "Not Found"))
	{
		std::cout << "404 Risorsa non trovata" << std::endl;
		std::string response = "HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Length: 119\r\n\r\n";
		response += "<!DOCTYPE html><html><head><style>span {font-size: 120px;}</style></head><body><span>404 Not Found</span></body></html>";
		send(_client_fd, response.c_str(), response.length(), 0);
		FD_CLR(_client_fd, &w);
		FD_SET(_client_fd, &r);
		done = true;
	}
	return false;
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
	if (_path.find(_location.getLocationPath()) != std::string::npos)
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
			std::string response = "HTTP/1.1 405 \r\nConnection: close\r\nContent-Length: 128\r\n\r\n";
			response += "<!DOCTYPE html><html><head><style>span {font-size: 120px;}</style></head><body><span>405 Method Not Allowed</span></body></html>";
			send(_client_fd, response.c_str(), response.length(), 0);
			FD_CLR(_client_fd, &w);
			FD_SET(_client_fd, &r);
			done = true;
		}
		return false;
	}
	return true;
}

// static bool	checkUpload(fd_set &r, fd_set &w)
// {
// 	if (!_server.getUploadPath().empty())
// 	{
// 			_upload = _server.GetUploadPath() + _request.GetRequest().at("PATH").substr(last_slash());
// 			return (true);
// 	}
// 	std::cout << "Response 500 Internal Server Error " << std::endl;
//     std::string response = "HTTP/1.1 500 \r\nConnection: close\r\nContent-Length: 85";
//     response += "\r\n\r\n<!DOCTYPE html><head><title>Internal Server Error</title></head><body> </body></html>";
//     send(_ClientFD, response.c_str(), response.length(), 0);
//     FD_CLR(_ClientFD, &w);
//     FD_SET(_ClientFD, &r);
//     done = 1;
// 	// L'if("POST") lo levo, che tanto qui ci entra solo se è POST
// 	if (access(/*path temporaneo della request, ma non so bene che è, domani lo chiedo a Michi*/.c_str(), F_OK != -1)
// 			remove(/*sempre lo stesso file*/.c_str());
// }

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
				// if (ok(?) || checkUpload(r, w))
					std::cout << "POST da scrivere" << std::endl;
			}
			else if (tmp == "DELETE")
			{
				std::cout << "DELETE da scrivere" << std::endl;
			}
		}
	}	
}
