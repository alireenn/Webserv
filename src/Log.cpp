/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccantale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 15:51:54 by ccantale          #+#    #+#             */
/*   Updated: 2023/05/24 17:28:13 by ccantale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"

void	log::init(void)
{
	logbook = std::ofstream(LOGPATH, std::ifstream::trunc);
}

std::string log::timestamp(void)
{
	auto		currentTime = std::chrono::system_clock::now();
	std::time_t	currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
	
	return (std::ctime(&currentTime_t));
}

void	log::cout(std::string msg)
{
	logbook << timestamp() << msg << std::endl;
}

void	log::cerr(std::string msg)
{
	logbook << "\nError:\n" << timestamp() << msg << std::endl;
}

void	log::close(void)
{
	logbook.close();
}
