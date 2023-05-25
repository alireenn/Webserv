/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccantale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 16:52:58 by ccantale          #+#    #+#             */
/*   Updated: 2023/05/25 15:03:17 by ccantale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_HPP
# define LOG_HPP

# define LOGPATH	"Logbook/log"
# define FOLDERPATH	"./Logbook"

# define RESET		"\033[0m"
# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"

# include <fstream>
# include <sys/stat.h>
# include <ctime>
# include <cstdio>
# include <string>

namespace amc
{
	static std::ofstream	logbook;

	void			init(void);
	std::string		getTimeAndDate(void);
	std::ofstream		&print(std::string color);
	void			close(void);
}

# define lout		print(GREEN)
# define lerr		print(RED)
# define lwar		print(YELLOW)
# define timestamp	amc::getTimeAndDate()

#endif
