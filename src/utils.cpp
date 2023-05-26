/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccantale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 16:46:15 by ccantale          #+#    #+#             */
/*   Updated: 2023/05/25 16:22:52 by ccantale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.h"

bool	utils::skipEmptyLines(std::ifstream &file)
{	
	while (file.peek() == '\n' || file.peek() == '\t' || file.peek() == ' ')
		file.get();
	return (1);
}

bool	utils::isKey(char c)
{
	return (c == '_' || c == ':' || c == '/' || c == '\\'
				|| c == '.' || c == '-' || isalnum(c));
}

bool	utils::isEmpty(std::string File)
{
	std::ifstream file(File.c_str());
	return file.peek() == std::ifstream::traits_type::eof();
}

int	utils::myStoi(const std::string& str)
{
    int result = 0;
    int sign = 1;
    size_t i = 0;

    // Gestione del segno
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    // Calcolo del valore numerico
    for (; i < str.length(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        } else {
            break;
        }
    }
    return result * sign;
}


int utils::check_methods(std::string token)
{
    if (token != "GET" && token != "DELETE" && token != "POST")
        return (0);
    return (1);
}

// int utils::check_size(std::string token)
// {
//     if ()
// }