/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccantale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 16:52:58 by ccantale          #+#    #+#             */
/*   Updated: 2023/05/24 17:13:52 by ccantale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_HPP
# define LOG_HPP

# define LOGPATH "Logbook/log"

# include <fstream>
# include <chrono>

namespace log
{
	static std::ofstream	logbook;

	void			init(void);
	std::string		timestamp(void);
	void			cout(std::string msg);
	void			cerr(std::string msg);
	void			close(void);
}

#endif
