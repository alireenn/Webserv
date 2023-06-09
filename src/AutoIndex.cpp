/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 15:36:48 by mruizzo           #+#    #+#             */
/*   Updated: 2023/06/09 16:25:55 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../includes/AutoIndex.hpp"

// t_dir showDirContent(std::string path)
// {
	
// }
#include <string>

std::string generateAutoIndex(std::string path)
{
	std::string autoIndex = "";
	if (!path.empty())
	{
		// t_dir dir = showDirContent(path);
		autoIndex += "<!DOCTYPE html>\n<html>\n<head>\n<title>Index of " + path.substr(1) + "</title>\n</head>";
		autoIndex += "<body>\n<h1>Index of " + path.substr(1) + "</h1>\n";
		autoIndex += "<table>\n<tr>\n<th>Name</th>\n<th>Last Modified</th>\n<th>Size</th>\n</tr>\n";
		// for (t_dir::iterator it = dir.begin(); it != dir.end(); it++)
		// {
		// 	autoIndex += "<tr>\n<td><a href=\"" + it->first.first + "\">" + it->first.first + "</a></td>\n";
		// 	autoIndex += "<td>" + it->first.second + "</td>\n";
		// 	autoIndex += "<td>" + std::to_string(it->second) + "</td>\n</tr>\n";
		// }
		autoIndex += "</table>\n</body>\n</html>";
	}
	return (autoIndex);
}