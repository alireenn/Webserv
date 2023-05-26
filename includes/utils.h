/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccantale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 16:43:15 by ccantale          #+#    #+#             */
/*   Updated: 2023/05/25 16:22:41 by ccantale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <fstream>

namespace utils
{
	bool	isKey(char c);
	bool	isEmpty(std::string File);
	int		myStoi(const std::string& str);
	int 	check_methods(std::string token);
	bool	skipEmptyLines(std::ifstream &file);
}

#endif
