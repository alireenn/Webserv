/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 10:29:59 by mruizzo           #+#    #+#             */
/*   Updated: 2023/05/23 12:28:34 by ccantale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Server.hpp"

class Config
{
private:

	std::vector<Server> _Servers; //vector di server, la vera classe da instanziare al momento.
	std::string _filePath;
	std::ifstream _configfile;
	char **_env;
	
public:
	Config();
	Config(char **env);
	~Config();

	void setConfig(char *filePath);
	void setEnv(char **env);
	char **getEnv(void) const;
	void parse();                                   // Questa forse dovrebbe essere statica interna al .cpp della classe, non pubblica, tanto chi la richiama
	bool SkipEmptyLines(std::ifstream &file);       // E pure questa, oppure a parte, in un namespace di utils
	std::vector<Server> &getServers(void);                                                 //
};                                                                                             //
                                                                                               //
/*                                       <///////////////////////////////////////////////////////
** Es.
**
** namespace StringUtils
** {
**	bool skipEmptyLines(std::ecceteraeccetera...
**	...
**	...
** }
**
*/

#endif	
