/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 13:04:09 by mruizzo           #+#    #+#             */
/*   Updated: 2023/05/29 11:19:00 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Webserv.hpp"


int main(int argc, char** argv, char** env)
{
	Config conf(env);
	IOMultiplexing io;

	amc::init();

	signal(SIGPIPE, SIG_IGN);//La chiamata a signal(SIGPIPE, SIG_IGN) 
	//imposta il gestore dei segnali per il segnale SIGPIPE in modo da ignorare il segnale quando viene ricevuto,
	//anziché gestirlo con il comportamento predefinito del sistema operativo, che di solito è terminare il processo.
	
	
	if (argc != 1 && argc != 2)
    {
		std::cout << "./webserv || ./webserv <path>" << std::endl;
        return (1);
    }
	else if (argc == 1)
		conf.setConfig((char *)"./config.d/default.conf");
	else if (argc == 2)
		conf.setConfig(argv[1]);
	
		
	io.StartServers(conf);
	
	
	return 0;
}
