/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 15:27:00 by mruizzo           #+#    #+#             */
/*   Updated: 2023/06/07 16:15:33 by ccantale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <fstream>
# include <sys/stat.h>
# include "Request.hpp"
# include "Server.hpp"

class Response
{
	private:
		Request _request;
		Server _server;
		int _client_fd;
		char str[1025];
		int len;
		int ok;
		int res_len;
		int fd;
		int size;
		int _send;
		int done;
		std::string _path;
		std::string _full_path;
		int en_handle;
		int post;
		std::string _upload;
		int fd_upload;
		u_int64_t len_server;
		int fd_error;
		int error;
		std::string status_code;
		Location _location;
		std::map<std::string, std::string> _headers; //serve per cookie e cgi

	public:
			Response();
			~Response();
			Response(Request &request, Server &server, int client_fd);

			int getClientFD() const;
			int &getDone(void);
			void setDone(int done);
			void test(fd_set& r, fd_set& w);

			void handler(fd_set &r, fd_set &w);
			bool isValid(fd_set &r, fd_set &w);
			bool isSubjectCompliant(fd_set & r, fd_set & w);
			void sendError(std::string code, std::string message, fd_set &r, fd_set &w);
			bool redirectPath(fd_set &r, fd_set &w);
			bool checkForbidden(fd_set &r, fd_set &w);
			bool handleAutoIndex(fd_set &r, fd_set &w);
			bool handleIndex(void);
			void sendData(fd_set &r, fd_set &w);
			bool checkLocation(fd_set &r, fd_set &w);
			bool handleRedirection(fd_set &r, fd_set &w);
			bool handleMethod(fd_set &r, fd_set &w);
			std::string getType(std::string path);

};

#endif
