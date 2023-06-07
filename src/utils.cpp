/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccantale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 16:46:15 by ccantale          #+#    #+#             */
/*   Updated: 2023/06/07 13:36:05 by ccantale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.h"

int utils::last_slash(std::string tmp)
{
    int a = 0;
    int i = 0;

    a = i = tmp.find("/",0);
    while(i != -1)
    {
        a = i;
        i = tmp.find("/", i + 1);
    }
    return (a);
}

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
