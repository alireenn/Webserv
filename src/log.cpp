/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccantale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 15:51:54 by ccantale          #+#    #+#             */
/*   Updated: 2023/05/24 18:43:51 by ccantale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log.hpp"

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

std::ofstream	&log::print(void)
{
	logbook << timestamp();
	return (logbook);
}

void	log::close(void)
{
	logbook.close();
}
