/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mruizzo <mruizzo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:06:04 by mruizzo           #+#    #+#             */
/*   Updated: 2023/06/09 18:28:10 by mruizzo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
# define AUTOINDEX_HPP

# include <iostream>
# include <vector>
# include <sys/stat.h>
# include <dirent.h>
# include <sstream>
# include <string>
# include <iomanip>

typedef std::vector<std::pair<std::pair<std::string, std::string>, int> > t_dir;
typedef std::pair<std::string, std::string> t_spair;

std::string generateAutoIndex(std::string path);

#endif