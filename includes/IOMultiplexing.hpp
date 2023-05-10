/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOMultiplexing.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 14:48:24 by mruizzo           #+#    #+#             */
/*   Updated: 2023/05/10 15:01:43 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IOMULTIPLEXING_HPP
#define IOMULTIPLEXING_HPP

#include "Config.hpp"

class IOMultiplexing
{
private:
	/* data */
public:
	IOMultiplexing(/* args */);
	~IOMultiplexing();
	void SetupServers(Config &conf);
};



#endif