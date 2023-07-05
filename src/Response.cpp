/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 16:15:21 by mruizzo           #+#    #+#             */
/*   Updated: 2023/07/05 16:43:31 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

std::string ft_toString(long long n)
{
    std::string str;
    if (n == 0)
        return "0";
    while (n != 0)
    {
        str.insert(str.begin(), n % 10 + '0');
        n /= 10;
    }
	return str;
}

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
    std::string response = "HTTP/1.1 200 OK\rContent-Type: text/html\r\nDate: " + date
			+ "\r\n\r\n<!DOCTYPE html>\n<html>\n<head>\n<style>\nspan {\nfont-size: 120px;\n}\n</style>\n</head>\n<body>\n<span>Vamos!</span>\n</body>\n</html>";

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

static void	errorPageNotFound(std::string &errorNbr, std::string &error, fd_set &r, fd_set &w,
								int _client_fd, int *done)
{
	std::string	header;
	std::string body;
	std::string message;

	std::cout << "Response " << errorNbr << " " << error << std::endl;
	header = "HTTP/1.1 " + errorNbr + "\r\nConnection: close\r\nContent-Length: ";
	body = "\r\n\r\n<!DOCTYPE html><head><style>span {font-size: 120px;}</style></head><body>" + error + "</body></html>";
	message = header + ft_toString(body.length() - 4) + body;
	send(_client_fd, message.c_str(), message.size(), 0);
	FD_CLR(_client_fd, &w);
	FD_SET(_client_fd, &r);
	*done = 1;
}

void Response::sendError(std::string errorCode, std::string message, fd_set &r, fd_set &w)
{
	for (size_t i = 0; i < _server.getErrorPages().size(); i++)
	{
		if(_server.getErrorPages().at(i).first == errorCode)
		{
			if(access(_server.getErrorPages().at(i).second.c_str(), F_OK) != -1)
			{
				std::string response;
				std::cout << "Error: page not found" << std::endl;
				char buffer[10024];
				struct stat filestat;
				stat(_server.getErrorPages().at(i).second.c_str(), &filestat);
				fd_error = open(_server.getErrorPages().at(i).second.c_str(), O_RDONLY);
				response = "HTTP/1.1 " + errorCode + " " + message + "\r\nContent-Type: text/html\r\nContent-Length: " + ft_toString(filestat.st_size) + "\r\n\r\n";
				send(_client_fd, response.c_str(), response.size(), 0);
				int len = read(fd_error, buffer, 10024);
				send(_client_fd, buffer, len, 0);
				done = true;
				return ;
			}	
		}
	}
	errorPageNotFound(errorCode, message, r, w, _client_fd, &done);
}

static bool	checkMethodAndVersion(std::string &method, std::string &version)
{				
    if (method != "GET" && method != "POST" && method != "PUT" && method != "PATCH"
			&& method != "DELETE" && method != "COPY" && method != "HEAD"
			&& method != "OPTIONS" && method != "LINK" && method != "UNLINK"
			&& method != "PURGE" && method != "LOCK" && method != "UNLOCK"
			&& method != "PROPFIND" && method != "VIEW" && version != "HTTP/1.1"
			&& version != "HTTP/1.0" && version != "HTTP/2.0" && version != "HTTP/3.0")
	{
			return (false);
	}
	return (true);
}

bool Response::isValid(fd_set &r, fd_set &w)
{
	std::string method = _request.GetRequest().at("Method");
    std::string version = _request.GetRequest().at("Version");

	if (!checkMethodAndVersion(method, version))
	{
		sendError("400" , "Bad Request", r, w);
		return (false);
	}
	return (true);
}

bool Response::isSubjectCompliant(fd_set &r, fd_set &w)
{
	std::string method = _request.GetRequest().at("Method");
    std::string version = _request.GetRequest().at("Version");

	if (method != "GET" && method != "POST" && method != "DELETE")
	{
		sendError("501", "Method not Implemented", r, w);
		return (false);
	}
	if (version != "HTTP/1.1" && version != "HTTP/1.0" )
	{
		sendError("505", "HTTP Version not Supported", r, w);
		return (false);
    }
    return (true);
}

static std::string getExtension(std::string str)
{
    std::string tmp;
    int i = str.size();
    while (i != 0 && i--)
    {
        if (str[i] != '.')
            tmp.insert(tmp.begin(), str[i]);
        else
            return tmp;
    }
    return tmp;
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

std::string Response::getType(std::string path)
{
    std::string tmp = getExtension(path);
    for (int i = 0; i < (int)_server.getMimeTypes().size(); i++)
	{
		int pos = _server.getMimeTypes()[i].find(tmp, 0);
        if (pos != -1)
            return _server.getMimeTypes()[i].substr(0, _server.getMimeTypes()[i].find("|", 0));
	}
    return "text/plain";
}

bool Response::checkForbidden(fd_set &r, fd_set &w)
{
	struct stat fileStat;
		
	stat(_full_path.c_str(), &fileStat);
	if(access(_full_path.c_str(),F_OK) != -1 && access(_full_path.c_str(),R_OK) == -1)
	{
		sendError("403","Forbidden", r, w);		
		return false;
	}
	return true;
}

int Response::checkInside(fd_set read, fd_set write)
{
		// std::cout << "checkInside  "  << _full_path << std::endl;
	if (access(_full_path.c_str(), F_OK) == -1)
	{
			sendError("204", "No Content", read, write);
			FD_CLR(_client_fd, &write);
			FD_SET(_client_fd, &read);
			done = true;
			if (_request.GetRequest().at("Method") == "POST")
			{
				if (access(_request.getPathTmp().c_str(), F_OK) != -1)
					remove(_request.getPathTmp().c_str());
			}
			return (0);
	}
	return (1);
}

int Response::check_permission(fd_set &read, fd_set &write)
{
	struct stat fileStat;
	int status = 0;

	if (S_ISDIR(fileStat.st_mode) && (access(_full_path.c_str(), W_OK) == -1 || access(_full_path.c_str(), X_OK)))
		status = 1;
	else if(!S_ISDIR(fileStat.st_mode) && access(_full_path.c_str(), W_OK) == -1)
		status = 1;
	//else
		//DA FARE UN CHECK ULTERIORE SUL CONTENUTO DELLA DIRECTORY (nella ref si chiama check_all)
	if (status)
	{
		sendError("403", "Forbidden", read, write);
		FD_CLR(_client_fd, &write);
		FD_SET(_client_fd, &read);
		done = true;
    	return (0);
	}
	return (1);
}

void DirDeleater(std::string to_delete)
{
    struct dirent *d;
    struct stat s;
    stat(to_delete.c_str(), &s);
    to_delete += "/";
    DIR *dir = opendir(to_delete.c_str());
    while (S_ISDIR(s.st_mode) && (d = readdir(dir)) != NULL)
    {
        std::string str1 = to_delete + d->d_name;
        struct stat s1;
        stat(str1.c_str(), &s1);
        if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, ".."));
        else
        {
            if (S_ISDIR(s1.st_mode))
                DirDeleater(to_delete + d->d_name);
            else if (!S_ISDIR(s1.st_mode))
                unlink(((const char *)str1.c_str()));
        }
    }
    if (dir)
        closedir(dir);
    rmdir(to_delete.c_str());
}

void Response::deleater(fd_set read, fd_set write)
{
	struct stat fileStat;
	stat(_full_path.c_str(), &fileStat);
	if (!S_ISDIR(fileStat.st_mode))
		unlink(_full_path.c_str());
	else
		DirDeleater(_full_path);
	FD_CLR(_client_fd, &write);
	FD_SET(_client_fd, &read);
	done = true;
	std::cout << "200 OK" << std::endl;
	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 130\r\n\r\n";
	response += "<!DOCTYPE html>\n<html>\n<head>\n<style>\nspan {\nfont-size: 120px;\n}\n</style>\n</head>\n<body>\n<span>File deleted</span>\n</body>\n</html>";
	send(_client_fd, response.c_str(), response.length(), 0);
}

static std::string stringtrim(std::string str)
{
	std::string str1;
	for (size_t i = 0; i < str.size(); i++)
		if (str[i] != ' ')
			str1 += str[i];
	return str1;
}

void Response::loadEnv(char **env)
{
	std::string str;
	_env.clear();
	for (size_t i = 0; env[i]; i++)
		_env.push_back(env[i]);
}

static std::string ft_toUpper(std::string str)
{
	std::string str1;
	for (size_t i = 0; i < str.size(); i++)
		str1 += toupper(str[i]);
	return str1;
}

char **createEnv(std::vector<std::string> _env)
{
	char **env = new char *[_env.size() + 1];
	for (size_t i = 0; i < _env.size(); i++)
		env[i] = strdup(_env[i].c_str());
	env[_env.size()] = NULL;
	return env;
}

void Response::startCgi()
{
	for (size_t i = 0; i < _server.getCgi().size(); i++)
	{
		if(_server.getCgi().at(i).second.find(getExtension(_full_path)) != std::string::npos)
		{
			loadEnv(_server.getEnv());
			for (std::map<std::string, std::string>::iterator it = _request.GetRequest().begin(); it != _request.GetRequest().end(); it++)
				_env.push_back("HTTP_" + ft_toUpper(it->first) + "=" + stringtrim(it->second));//ft_toUpper da creare
			char **env = createEnv(_env);//da creare
			char *argv[] = { (char *)_server.getCgi().at(i).first.c_str(), 
				(char *)_full_path.c_str(), NULL };
			unlink("tmp/cgi_output");//da capire
			unlink("tmp/cgi_output.html");//da capire
			int fd;
			pid_t pid = fork();
			if (pid == 0)
			{
				fd = open("tmp/cgi_output", O_CREAT | O_RDWR, 0666);
				dup2(fd, 1);
				execve(*argv, argv, env);
				close(fd);
			}
			else
				waitpid(pid, NULL, 0);
			std::ifstream file("tmp/cgi_output");
			if (getExtension(_full_path) == "php")
			{
				if (file.is_open())
				{
					std::string line;
					while (getline(file, line))
					{
						std::string key = line.substr(0, line.find(":"));
						std::string value = line.substr(line.find(":") + 1, line.length());
						_headers.insert(std::pair<std::string, std::string>(key, value));
						if (line.length() ==  1 && line.find("\r") != std::string::npos)
							break;
					}
				}
				if (file.eof())
					file.close();
			}
			std::ofstream myfile("tmp/cgi_output.html");
			if (file.is_open())
			{
				if (!file.is_open())
					file.open("tmp/cgi_output");
				std::string line;
				while (getline(file, line))
				{
					if (line.length() ==  1 && line.find("\r") != std::string::npos)
						break;
					myfile << line;
				}
				myfile.close();
				file.close();
			}
			for (size_t i = 0; i < _env.size(); i++)
				delete env[i];
			delete[] env;
			
			_full_path = "tmp/cgi_output.html";
		}
	}
}

void Response::sendData(fd_set &r, fd_set &w)
{
	startCgi();
	struct stat fileStat;

	stat(_full_path.c_str(), &fileStat);
	if (access(_full_path.c_str(), F_OK) != -1)
	{
		if (!ok)
		{
			stat(_full_path.c_str(), &fileStat);
			size = fileStat.st_size;
			fd = open(_full_path.c_str(), O_RDONLY);
			bzero(str, 1025);
			std::string header;
			std::cout << "200 OK" << std::endl;
			std::cout << _full_path << std::endl;
			header = (char *)"HTTP/1.1 200 OK\r\nContent-Length: " + ft_toString(size)+ "\r\nContent-type: ";
            header += deleteSpace(getType(_full_path)) + ((_headers["Set-Cookie"] == "") ? "" : ("\r\nSet-Cookie: " + _headers["Set-Cookie"])) + "\r\nConnection: " + deleteSpace(_request.GetRequest().at("Connection")) + "\r\n\r\n";
            
			write(_client_fd, header.c_str(), header.size());
			write(1, header.c_str(), header.size());
			ok = true;
		}
		len = read(fd, str, 1024);
		_send = send(_client_fd, str, len, 0);
		res_len += _send;
		if (_send == -1)
		{
			FD_CLR(_client_fd, &w);
			FD_SET(_client_fd, &r);
			done = true;
			close(fd);
		}
		else if (res_len >= size)
		{
			FD_CLR(_client_fd, &w);
			FD_SET(_client_fd, &r);
			close(fd);
			res_len = 0;
			done = true;
		}	
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
			char buff[1025];
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
			char buff[1025];
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
		return true;
	}
	sendError("404", "Not Found", r, w);
	//done = true;
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
			len_server = strtoull(_location.getClientMaxBodySize().c_str(), NULL, 10);
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
		
		sendError("405", "Method Not Allowed", r, w);
		//done = true;
		return false;
	}
	return true;
}

bool Response::handleAutoIndex(fd_set &r, fd_set &w)
{
	std::ofstream file;
	if (_location.getAutoIndex() == "on")
	{
		std::string tmp = _full_path;
		struct stat fileStat;
		stat(_full_path.c_str(), &fileStat);
		if (access(_full_path.c_str(), F_OK) != -1 && !S_ISDIR(fileStat.st_mode))
			return true;
		if (S_ISDIR(fileStat.st_mode))
		{
			_full_path = "www/autoindex/autoindex.html";
			file.open(_full_path.c_str());
			file.clear();
			file << generateAutoIndex(tmp);
			file.close();
			return true;
		}
	}
	sendError("404", "Not Found", r, w);
	return false;
}

bool Response::handleIndex()
{
	struct stat fileStat;

	stat(_full_path.c_str(), &fileStat);
	if ((access(_full_path.c_str(), F_OK) != -1) && !S_ISDIR(fileStat.st_mode))
		return true;
	for (size_t i = 0; i < _location.getIndex().size(); i++)
	{
		_full_path += "/" + _location.getIndex().at(i);
		if (access(_full_path.c_str(), F_OK) != -1)
			return true;
	}
	return false;
}

static bool	checkUpload(Server &_server, Request &_request, std::string &_upload)
{
	std::string request_str;

	if (_server.getUploadPath().empty())
	{
		if (access(_request.getPathTmp().c_str(), F_OK) != -1)
			remove(_request.getPathTmp().c_str());
		return (false);
	}
	request_str = _request.GetRequest().at("Path");
	_upload = _server.getUploadPath() + request_str.substr(utils::last_slash(request_str));
	printf("getpath: %s\n", _server.getUploadPath().c_str());
	return (true);
}

// static void	uploadFail(int _client_fd, Request &_request, int *done, fd_set &r, fd_set &w)
// {
//     std::string response = "HTTP/1.1 500 \r\nConnection: close\r\nContent-Length: 85";

// 	std::cout << "Response 500 Internal Server Error " << std::endl;
//     response += "\r\n\r\n<!DOCTYPE html><head><title>Internal Server Error</title></head><body> </body></html>";
//     send(_client_fd, response.c_str(), response.length(), 0);
// 	FD_CLR(_client_fd, &w);
// 	FD_SET(_client_fd, &r);
//     *done = 1;
// 	if (access(_request.getPathTmp().c_str(), F_OK) != -1)
// 		remove(_request.getPathTmp().c_str());
// }

static bool	checkRequest(Request &_request, Response &response,
							u_int64_t len_server, fd_set &r, fd_set &w)
{
	if (_request.checkChunked() == 0 && _request.GetRequest().at("Content-Length").empty())
	{
		if (access(_request.getPathTmp().c_str(), F_OK) != -1)
			remove(_request.getPathTmp().c_str());
		response.sendError("411", "Length required", r, w);
		return (false);
	}
	if ((u_int64_t)_request.getLength() > len_server && _request.checkChunked() == 0)
	{
		std::cout << "len server : " << len_server << std::endl;
		std::cout << "request lengsb : " << _request.getLength() << std::endl;
		if (access(_request.getPathTmp().c_str(), F_OK) != -1)
			remove(_request.getPathTmp().c_str());
		response.sendError("413", "Payload too large", r, w);
		return (false);
	}
	return (true);
}

static void	writeBody(Request &_request, std::string _upload,
						int _client_fd, int *done, fd_set &r, fd_set &w)
{
	std::string	message;
	
	std::cout << "Response 201 Created " << std::endl;
    rename(_request.getPathTmp().c_str(), _upload.c_str());
    message = (char *)"HTTP/1.1 201 Created\r\nLocation: ";
    message += _upload + "\r\nContent-Length: 131\r\n\r\n";
	message += "<!DOCTYPE html>\n<html>\n<head>\n<style>\nspan {\nfont-size: 120px;\n}\n</style>\n</head>\n<body>\n<span>File uploaded</span>\n</body>\n</html>";
	send(_client_fd, message.c_str(), message.size(), 0);
    FD_CLR(_client_fd, &w);
    FD_SET(_client_fd, &r);
    *done = true;
}

void Response::handler(fd_set &r, fd_set &w)
{
	if (!ok)
		_full_path = _path = deleteSpace(_request.GetRequest().at("Path"));
	if (ok || (isValid(r,w) && isSubjectCompliant(r,w) && checkLocation(r,w)))
	{
		if(ok || (handleRedirection(r,w) && handleMethod(r,w)))
		{
			if(deleteSpace(_request.GetRequest().at("Method")) == "GET")
			{
				if (ok || (redirectPath(r,w) && checkForbidden(r,w)))
					if(ok || handleIndex() || handleAutoIndex(r,w))
						sendData(r,w);
			}
			else if (deleteSpace(_request.GetRequest().at("Method")) == "POST")
			{
				if (checkUpload(_server, _request, _upload)	&& checkRequest(_request, *this, len_server, r, w))
				{
					writeBody(_request, _upload, _client_fd, &done, r, w);
				}
				// else
					// uploadFail(_client_fd, _request, &done, r, w);
			}
			else if (deleteSpace(_request.GetRequest().at("Method")) == "DELETE")
			{
				std::cout << "DELETE" << std::endl;
				if (checkInside(r, w))
					if (check_permission(r, w))
						deleater(r, w);
			}
		}
	}	
}
