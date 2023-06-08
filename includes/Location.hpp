/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 13:16:58 by mruizzo           #+#    #+#             */
/*   Updated: 2023/06/08 13:02:19 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <vector>

class Location
{
	private:
		std::string							_root;
		std::vector<std::string>			_index;
		std::string							_autoIndex;	
		std::string							_uploadPath;
		std::pair<std::string, std::string>	_redirection;
		std::string							_locationPath;	
		std::vector<std::string>			_allowedMethods;
		std::string							_clientMaxBodySize;
	public:
		Location();
		Location(Location const &obj);
		~Location();

		std::string							&getRoot();
		std::vector<std::string>			&getIndex();
		std::string							&getAutoIndex();
		std::string							&getUploadPath();
		std::pair<std::string, std::string>	&getRedirection();
		std::string							&getLocationPath();
		std::vector<std::string>			&getAllowedMethods();
		std::string							&getClientMaxBodySize();

		void								setRoot(std::string &root);
		void								setIndex(std::vector<std::string> &index);
		void								setAutoIndex(std::string &autoIndex);
		void								setUploadPath(std::string &uploadPath);
		void								setLocationPath(std::string &locationPath);
		void								setClientMaxBodySize(std::string &clientMaxBodySize);
		void								setAllowedMethods(std::vector<std::string> &allowedMethods);
		void								setRedirection(std::pair<std::string, std::string> &redirection);
};
//operatore di overloading <<
		// std::ostream						&operator<<(std::ostream &out, Location &location);


#endif