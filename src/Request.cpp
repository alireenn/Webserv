/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 12:02:14 by mruizzo           #+#    #+#             */
/*   Updated: 2023/06/05 11:55:06 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"

Request::Request()
{
	this->header = 0;
	this->first_line = 0;
	this->done = 0;
	this->_length = 0;
	this->body_length = 0;
	this->fd = 0;
	this->finished = 0;
	this->send = 0;
	this->size = 0;
	this->connection = 0;
	this->chunked = 0;
	this->full = 0;
	this->ok = 0;
	this->path_tmp = "";
	this->lent_chunked = 0;
	this->skip = 0;
}

Request::~Request()
{
}

Request::Request(const Request &req)
{
	*this = req;
}

Request &Request::operator=(const Request &req)
{
	if (this != &req)
	{
		this->body = req.body;
		this->body_length = req.body_length;
		this->fd = req.fd;
		this->header = req.header;
		this->_length = req._length;
		this->first_line = req.first_line;
		this->finished = req.finished;
		this->request = req.request;
		this->done = req.done;
		this->send = req.send;
		this->size = req.size;
		this->connection = req.connection;
		this->chunked = req.chunked;
		this->full = req.full;
		this->ok = req.ok;
		this->path_tmp = req.path_tmp;
		this->lent_chunked = req.lent_chunked;
		this->skip = req.skip;
	}
	return (*this);
}

std::map<std::string, std::string> &Request::GetRequest(void)
{
	return (request);
}

int &Request::getHeader(void)
{
	return (this->header);
}

int &Request::GetFirstLine(void)
{
	return (this->first_line);
}

void Request::validRequest(std::string str)
{
	int index = 0;
	int del = 0;
	std::string tmp;

	del = str.find(" ", index);
	tmp = str.substr(index, del);
	request.at("Method") = tmp;
	index = del;
	del = str.find(" ", index + 1);
	tmp = str.substr(index , del - index);
	request.at("Path") = tmp;
	index = del + 1;
	del = str.find("\r\n", index + 1);
	tmp = str.substr(index, del - index);
	request.at("Version") = tmp;
	if (!(request.at("Method") == "GET" || request.at("Method") == "POST" || request.at("Method") == "DELETE")
		&& (request.at("Version") == "HTTP/1.1" || request.at("Version") == "HTTP/1.0"))
		first_line = 1;
}

std::string Request::getHeader(std::string str)
{
	std::string tmp;

	for (int i = 0; str[i] != '\r'; i++)
	{
		if (str[i] == '\r' && str[i + 1] == '\n' && str[i + 2] == '\r' && str[i + 3] == '\n')
		{
			tmp += "\r\n\r\n";
			return tmp;
		}
		tmp.push_back(str[i]);
	}
	return (str);
}

static std::string delete_space(std::string str)
{   
    for(int i = 0; str[i]; i++)
        if (str[i] == ' ')
            str.erase(i,1);
    return str;
}

static int last_slash(std::string tmp)
{
    int a = 0;
    int i = 0;
    a = i = tmp.find("/",0);
    while(i != -1)
    {
        a = i;
        i = tmp.find("/",i+1);
    }
    return a;
}

void Request::getBody(char *str)
{
	std::string tmp = str;
	int i=tmp.find("\r\n\r\n", 0) + 4;
	int tmp_body_length = 0;
	while (i < _length)
	{
		body.push_back(str[i]);
		i++;
		tmp_body_length++;
	}
	body_length = tmp_body_length;
}

void Request::openFile(void)
{
	path_tmp = "tmp" + delete_space(request.at("Path").substr(last_slash(request.at("Path"))));
	fd = open(path_tmp.c_str(), O_CREAT | O_RDWR , 0777);
}

int &Request::getFinished(void)
{
	return (this->finished);
}

int &Request::getSend(void)
{
	return (this->send);
}

void Request::initMap(void)
{
	request.insert(std::pair<std::string, std::string>("Method", ""));
	request.insert(std::pair<std::string, std::string>("Path", ""));
	request.insert(std::pair<std::string, std::string>("Version", ""));
	request.insert(std::pair<std::string, std::string>("Host", ""));
	request.insert(std::pair<std::string, std::string>("Content-Length", ""));

}

void Request::handleHostPort(void)
{
	request.insert(std::pair<std::string, std::string>("Port", ""));
	std::string tmp = request.at("Host");
	int i = tmp.find(":", 0);
	if (i !=-1)
	{
		request.at("Host") = tmp.substr(0, i);
		request.at("Port") = tmp.substr(i + 1,4);
	}
}

int &Request::checkCunked(void)
{
	return (this->chunked);
}

std::string Request::sendBody(void)
{
	return body;
}

u_int64_t &Request::getLentFull(void)
{
	return (this->full);
}

void Request::writeBody(void)
{
	int tmp = write(fd, body.c_str(), body.length());
	full += tmp;
	if (full >= size)
	{
		close(fd);
		finished = 1;
		ok = 0;
	}
}

void Request::checkRequest(char *tmp)
{
	std::string str = tmp;

	if ((int) str.find("\r\n\r\n", 0) != -1)
	{
		header = 1;
		buffer = getHeader(str);
		validRequest(buffer);
		getBody(tmp);
		first_line = 1;
	}
}

void Request::handleRequest(char *str)
{
	int index = 0;
	std::string delimiter = "\r\n";
	std::string last = "\r\n\r\n";
	std::string value;
	std::string check = str;
	int hold = 0;
	int i=0;

	if(ok) 
	{
		body_length = _length;
		std::string tmp(str, body_length);
		body = tmp;
		if(chunked)
			TransferChunked(); // Processa il corpo con trasferimento chunked
		else
			writeBody(); // Scrive il corpo sdella richiesta 
	}

	if (!finished && !ok)
	{	
		initMap();
		checkRequest(str);
		if (first_line)
		{
			index = buffer.find(delimiter, index) + 2;
			do
			{
				hold = buffer.find(delimiter, index);
				value = buffer.substr(index, hold - index);
				if((int) value.find("Connection", 0) != -1)
					connection = 1;
				if ((int) value.find("chunked", 0) != -1)
					chunked = 1;
				i = value.find(": ", 0);
				if ((int) value.find("Content-Length", 0) != -1)
					request.at("Content-Length") = value.substr(i + 1, value.size() - i);
				if ((int) value.find("Host") != -1)
					request.at("Host") = value.substr(i + 1, value.size() - i);
				else
					request.insert(std::pair<std::string, std::string>(value.substr(0, i), value.substr(i + 1, value.size() - i)));
				value.clear();
				index = hold + 2;
			} while (buffer.substr(buffer.find(delimiter, index - 2), buffer.size()) != last);
		}
		finished = 1;

		if(!connection)
			request.insert(std::pair<std::string, std::string>("Connection", "close"));
		handleHostPort();
		if (request.at("Method") == "POST" && (!request.at("Content-Length").empty() || chunked))
		{
			openFile();
			size =strtoull(request.at("Content-Length").c_str(), NULL, 10);
			ok = 1;
			finished = 0;
			if (chunked)
			{
				body = delimiter + body;
				TransferChunked();
			}
			else
				writeBody();
		}
	}
}

int &Request::getLength(void)
{
	return (_length);
}

void Request::setLength(int length)
{
	_length = length;
}

int Request::getOk(void)
{
	return ok;
}

void Request::TransferChunked(void)
{
	char buff;
//     std::string tmp;
    // int bl_tmp;
	// bl_tmp = 0;
//     (void) a;
	if(lent_chunked > (int) body.length())
	{
		write(fd, body.c_str(), body.length());
		lent_chunked -= body.length();
	}
	else
	{
		if(lent_chunked == 0)
		{
			lent_chunked = getLentChunked(body);
			if(lent_chunked == 0)
			{
				close(fd);
				finished = 1;
				ok = 0;
			}
			else
				body.erase(0, skip);
		}
		for (int i = 0; !finished && i <(int) body.length() && lent_chunked; i++)
		{
			if (lent_chunked)
			{
				buff = body[i];
				write(fd, &buff, 1);
			}
			lent_chunked--;
			if (lent_chunked == 0)
			{
				lent_chunked = getLentChunked(body.substr(i));
				// bl_tmp = body.length() - i;
				if (lent_chunked == 0)
				{
					lent_chunked = getLentChunked(body.substr(i));
					if (lent_chunked == 0)
					{
						close(fd);
						finished = 1;
						ok = 0;
					}
					i += skip;
				}
			}
		}
	}
}

static int hex_dec(char *str)
{
     int i = 0, val, len;
    int n = 0;
     len = strlen(str) - 1;
    while (str[i] != '\0')
    {
        if (str[i] >= '0' && str[i] <= '9')
            val = str[i] - 48;
        else if (str[i] >= 'a' && str[i] <= 'f') 
            val = str[i] - 97 + 10;
        else if (str[i] >= 'A'&& str[i] <= 'F') 
            val = str[i] - 65 + 10;
        n += val * pow(16, len);
        len--;
        i++;
    }
    return n;
}

int Request::getLentChunked(std::string str)
{
	std::string tmp;
	for(int i = 0; i < (int) str.length(); i++)
	{
		if(str[i] == '\r' && str[i + 1] == '\n')
		{
			i+=2;
			skip = 2;
			while (i < (int)str.length() && str[i] != '\r' && str[i + 1] != '\n')
			{
				tmp.push_back(str[i++]);
				skip++;
			}
			if(tmp.empty())
			{
				skip += 2;
				return hex_dec((char *)tmp.c_str());
			}
			else
				skip = 0;
		}
	}
	return 0;
}


std::string &Request::getPathTmp(void)
{
	return path_tmp;
}