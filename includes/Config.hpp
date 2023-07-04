/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 10:29:59 by mruizzo           #+#    #+#             */
/*   Updated: 2023/05/30 12:05:56 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "Server.hpp"
#include "utils.h"

std::string ft_toString(long long n);

class Config
{
private:

	std::vector<Server> _servers; //vector di server, la vera classe da instanziare al momento.
	std::string _filePath;
	std::ifstream _Configfile;
	char **_env;
	
public:
	Config();
	Config(char **env);
	~Config();

	void setConfig(char *filePath);
	void setEnv(char **env);
	char **getEnv(void) const;
	void parse();                                   // Questa forse dovrebbe essere statica interna al .cpp della classe, non pubblica, tanto chi la richiama
	std::vector<Server> &getServers(void);

	void parseLocation(std::string &line, Server &server);
};

#endif	
