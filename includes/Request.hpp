/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 13:11:06 by mruizzo           #+#    #+#             */
/*   Updated: 2023/06/08 16:06:56 by ccantale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

# include <iostream>
# include <unistd.h>
# include <map>
# include <fcntl.h>
# include <string.h>
# include <math.h>
# include "utils.h"

class Request
{
private:
    std::map<std::string, std::string> request;
    int header;
    int first_line;
    int done;
    int _length;
    int body_length;
    int fd;
    int finished;
    int send;
    int connection;
    int chunked;
    int ok;
    int lent_chunked;
    int skip;
    std::string path_tmp;
    std::string body;
    std::string buffer;
    u_int64_t full;
    u_int64_t size;
	
	public:
		Request();
		~Request();
		Request(const Request &req);
        Request &operator=(const Request &req);
		
        std::map<std::string, std::string> &GetRequest(void);
        int &getHeader(void);
        std::string getHeader(std::string str);
        int &GetFirstLine(void);
        void validRequest(std::string str);
        void checkRequest(char *tmp);
		void handleRequest(char *str);
		int &getLength(void);
        void setLength(int length);
        void getBody(char *str);
        void openFile(void);
        int &getFinished(void);
        int &getSend(void);
        void initMap(void);
        void handleHostPort(void);
        int &checkChunked(void);
        std::string sendBody(void);
        u_int64_t &getLentFull(void);
        void writeBody(void);
        std::string &getPathTmp(void);
        int getOk(void);
        void TransferChunked(void);
        int getLentChunked(std::string str);

};

#endif
