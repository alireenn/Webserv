/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 13:16:58 by mruizzo           #+#    #+#             */
/*   Updated: 2023/05/11 17:18:55 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <vector>

class Location
{
	private:
		std::string							_locationPath;	
		std::vector<std::string>			_allowedMethods;
		std::string							_root;
		std::string							_clientMaxBodySize;
		std::string							_index;
		std::string							_autoIndex;	
		std::string							_uploadPath;
		std::pair<std::string, std::string>	_redirection;
	public:
		Location();
		~Location();

		std::string							&getLocationPath();
		std::vector<std::string>			&getAllowedMethods();
		std::string							&getRoot();
		std::string							&getClientMaxBodySize();
		std::string							&getIndex();
		std::string							&getAutoIndex();
		std::string							&getUploadPath();
		std::pair<std::string, std::string>	&getRedirection();

		void								setLocationPath(std::string &locationPath);
		void								setAllowedMethods(std::vector<std::string> &allowedMethods);
		void								setRoot(std::string &root);
		void								setClientMaxBodySize(std::string &clientMaxBodySize);
		void								setIndex(std::string &index);
		void								setAutoIndex(std::string &autoIndex);
		void								setUploadPath(std::string &uploadPath);
		void								setRedirection(std::pair<std::string, std::string> &redirection);
};

#endif