/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 15:36:48 by mruizzo           #+#    #+#             */
/*   Updated: 2023/06/09 18:33:42 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/AutoIndex.hpp"

static string getSize(long long st_size)
{
	
}

static t_dir showDirContent(std::string path)
{
	DIR *dir = opendir(path.c_str());
	t_dir dirContent;

	if (dir)
	{
		struct dirent *dirStat;//è una struttura dati in C che viene utilizzata per rappresentare le informazioni di una voce all'interno di una directory in un sistema operativo Unix
		while ((dirStat = readdir(dir)) != NULL)
		{
			if (dir->d_type != DT_DIR && dir->d_name[0] != '.')//non deve essere ne una directory ne un file nascosto
			{
				struct stat fileStat;
				stat((std::string(path) + "/" + std::string(dirStat->d_name)).c_str(), &fileStat);
				
				t_spair NameSize;
				NameSize.first = std::string(dirStat->d_name);
				NameSize.second = getSize(fileStat.st_size);
			}
		}
	}
}

std::string generateAutoIndex(std::string path)
{
	std::string autoIndex = "";
	if (!path.empty())
	{
		t_dir dir = showDirContent(path);
		autoIndex += "<!DOCTYPE html>\n<html>\n<head>\n<title>Index of " + path.substr(1) + "</title>\n</head>";
		autoIndex += "<body>\n<h1>Index of " + path.substr(1) + "</h1>\n";

	}
	return (autoIndex);
}