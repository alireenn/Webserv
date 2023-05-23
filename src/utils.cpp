/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccantale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 16:46:15 by ccantale          #+#    #+#             */
/*   Updated: 2023/05/23 16:47:49 by ccantale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.h"

bool utils::skipEmptyLines(std::ifstream &file)
{	
	while (file.peek() == '\n' || file.peek() == '\t' || file.peek() == ' ')
		file.get();
	return (1);
}
