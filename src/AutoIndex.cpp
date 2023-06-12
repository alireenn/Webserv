/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 15:36:48 by mruizzo           #+#    #+#             */
/*   Updated: 2023/06/12 14:30:55 by ccantale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/AutoIndex.hpp"

static std::string getSize(long long st_size) // dimensione in byte
{	// restituisce una stringa formattata 
	//che rappresenta la dimensione in unità di misura più comuni
	std::string ret;
	std::stringstream stream; //manipolare e formattare le stringhe come se fossero uno stream di input/output

	if (st_size < 1048576) // n di byte di un kilobyte
	{
		stream << std::fixed << std::setprecision(2) << (long double)(st_size) / 1024;
		ret = stream.str();
		return (ret + "K");
	}
	else if (st_size < 524288000)
	{
		stream << std::fixed << std::setprecision(2) << (long double)(st_size) / 1048576;
		ret = stream.str();
		return (ret + "M");
	}
	else
	{
		stream << std::fixed << std::setprecision(2) << (long double)(st_size) / 1073741824;
		ret = stream.str();
		return (ret + "M");
	}
}

static t_dir showDirContent(std::string path)
{
	DIR *dir = opendir(path.c_str());
	t_dir dirContent;

	if (dir)
	{
		struct dirent *dirStat;//è una struttura dati in C che viene utilizzata per rappresentare le informazioni di una voce all'interno di una directory in un sistema operativo Unix
		while ((dirStat = readdir(dir)))
		{
			if (dirStat->d_type != DT_DIR && dirStat->d_name[0] != '.')//non deve essere ne una directory ne un file nascosto
			{
				struct stat fileStat;
				stat((std::string(path) + "/" + std::string(dirStat->d_name)).c_str(), &fileStat);
				
				t_spair NameSize;
				NameSize.first = std::string(dirStat->d_name);
				NameSize.second = getSize(fileStat.st_size).c_str();

				t_dir::value_type PairType;
				PairType.first = NameSize;
				PairType.second = 2; // 2 = file

				dirContent.push_back(PairType);
			}
			else if (dirStat->d_type == DT_DIR && dirStat->d_name[0] != '.')
			{
				struct stat fileStat;
				stat((std::string(path) + "/" + std::string(dirStat->d_name)).c_str(), &fileStat);
				
				t_spair NameSize;
				NameSize.first = dirStat->d_name;
				NameSize.second = "-";

				t_dir::value_type PairType;
				PairType.first = NameSize;
				PairType.second = 1; // 1 = directory

				dirContent.push_back(PairType);
			}
		}
		closedir(dir);
	}
	return (dirContent);
}

std::string generateAutoIndex(std::string path)
{
	std::string autoIndex = "";
	if (!path.empty())
	{
		t_dir dir = showDirContent(path);
		autoIndex += "<!DOCTYPE html>\n<html>\n<head>\n<title>Index of " + path.substr(1) + "</title>\n</head>";
		autoIndex += "<body>\n<h1>Index of " + path.substr(1) + "</h1>\n";
		autoIndex += "<table>\n<tr>\n<th>Name</th>\n<th>Size</th>\n</tr>\n";
		for (t_dir::iterator it = dir.begin(); it != dir.end(); it++)
		{
			autoIndex += "<tr>\n<td><a href=\"" + path + "/" + it->first.first + "\">" + it->first.first + "</a></td>\n";
			autoIndex += "<td>" + it->first.second + "</td>\n</tr>\n";
		}
		autoIndex += "</table>\n</body>\n</html>";
	}
	return (autoIndex);
}
