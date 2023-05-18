/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 10:29:59 by mruizzo           #+#    #+#             */
/*   Updated: 2023/05/10 10:37:33 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "Server.hpp"

class Config
{
private:

	std::vector<Server &> _Servers; //vector di server, la vera classe da instanziare al momento.
    std::string _FilePath;
    std::ifstream _Configfile;
	char **_env;
	
public:
	Config();
	~Config();

	void setConfig(std::string filePath);
	void setEnv(char **env);
	char **getEnv(void) const;
	void parse();
	bool SkipEmptyLines(std::ifstream &file);

};

#endif	