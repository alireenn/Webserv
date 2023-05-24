/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccantale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 16:52:58 by ccantale          #+#    #+#             */
/*   Updated: 2023/05/24 18:44:22 by ccantale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_HPP
# define LOG_HPP

# define LOGPATH "Logbook/log"

# define cout	print()

# include <fstream>
# include <chrono>

namespace log
{
	static std::ofstream	logbook;

	void			init(void);
	std::string		timestamp(void);
	std::ofstream		&print(void);
	void			close(void);
}

#endif
