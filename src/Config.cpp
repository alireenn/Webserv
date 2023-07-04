#include "../includes/Config.hpp"

Config::Config()
{
}

Config::Config(char **env): _env(env)
{
}

Config::~Config()
{
}

void Config::setConfig(char *filePath)
{
	this->_filePath = filePath;
	this->_Configfile.open(_filePath.c_str());

	if (!_Configfile.is_open() || utils::isEmpty(_filePath))
		std::cerr << "Your file can't be opened or is empty\n";
	else
		parse();
}

void Config::setEnv(char **env)
{
	this->_env = env;
}

char **Config::getEnv(void) const
{
	return (this->_env);
}

static std::string nextToken(std::string &fLine)
{
	int		i = 0;
	std::string	ret = "";

	while (fLine[i])
	{
		if (fLine[i] == ' ' || fLine[i] == '\t')
			i++;
		if (fLine[i] == '{' || fLine[i] == '}') 
		{
			ret += fLine[i];
			i++;
			break ;
		}
		if (utils::isKey(fLine[i]))
		{
			while (utils::isKey(fLine[i]))
			{
				ret += fLine[i];
				i++;
			}
			break ;
		}
		i++;
	}
	//estraggo da line i token
	fLine = fLine.substr(i, fLine.length() - i);
	return (ret);
}

int findPort(std::string &line)
{
	std::string port = nextToken(line);
	if (port.empty())
	{
		std::cerr << "Error: Listen: port not found\n";
		return (0);
	}
	else
	{
		if (utils::myStoi(port) <= 0 && utils::myStoi(port) >= 65536)
		{
			std::cerr << "Error: Listen: port not valid\n";
			return (0);
		}
		else
			return(utils::myStoi(port));
	}
}

/*
**	1) leggo il file una riga alla volta
**	2) estraggo i token
**	3) quando leggo la porta, creo un nuovo server e lo pusho nel vector di server
      vector_di_server.push_back(*(new Server(porta)));
**	4) aggiungo le info al server tramite i setter
*/

void serverName(std::string &line, Server &server)
{
	std::vector<std::string>	servernames;
	std::string name = nextToken(line);

	if (name.empty())
	{
		std::cout << "Error: Server name is empty\n";
	}
	while (!name.empty())
	{
		std::cout << "ci passo " << name << std::endl;
		servernames.push_back(name);
		name = nextToken(line);
	}
	server.setServerNames(servernames);
}

void Config::parseLocation(std::string &line, Server &server)
{
	(void)server;
	Location tmploc;
	int curlyBruh = 0;
	std::string token = "";
	std::string locpath = nextToken(line);

	if (locpath.empty())
		std::cerr << "Error: Location path is empty\n";
	else
		tmploc.setLocationPath(locpath);
	while (utils::skipEmptyLines(this->_Configfile) && getline(this->_Configfile, line))
	{
		token = nextToken(line);
		utils::skipEmptyLines(this->_Configfile);

		do
		{
			if (token == "{")
				curlyBruh++;
			else if (token == "}")
			{
				curlyBruh--;
				if (curlyBruh <0)
				{
					std::cout << "Error: Config file BRUH\n";
					return ;
				}
				else if (curlyBruh == 0)
				{
					server.getLocations().push_back(tmploc);
					return ;
				}
			}
			else if (token == "autoindex")
			{
				std::string autoindex = nextToken(line);
				tmploc.setAutoIndex(autoindex);
			}
			else if (token == "index")
			{
				std::vector<std::string> index;
				std::string indexname = nextToken(line);
				while (!indexname.empty())
				{
					index.push_back(indexname);
					indexname = nextToken(line);
					tmploc.setIndex(index);
				}
			}
			else if (token == "root")
			{
				std::string root = nextToken(line);
				tmploc.setRoot(root);
			}
			else if (token == "allow_methods")
			{
				std::vector<std::string> allow_methods;
				std::string method = nextToken(line);
				while (!method.empty())
				{
					if (!utils::check_methods(method))
					{
						std::cout << "Error: Method not valid\n";
						continue ;
					}
					else
					{
						allow_methods.push_back(method);
						method = nextToken(line);
					}
				}
				tmploc.setAllowedMethods(allow_methods);
			}
			else if (token == "upload_path")
			{
				std::string upload_path = nextToken(line);
				tmploc.setUploadPath(upload_path);
			}
			else if (token == "client_max_body_size")
			{
				std::string client_max_body_size = nextToken(line);
				if (client_max_body_size.empty())
					std::cout << "Error: Size not valid\n";
				else
				{
					if (client_max_body_size[client_max_body_size.length() - 1] == 'k')
					{
						client_max_body_size = client_max_body_size.substr(0, client_max_body_size.length() - 1);
						client_max_body_size = ft_toString(utils::myStoi(client_max_body_size) * 1024);
					}
					else if (client_max_body_size[client_max_body_size.length() - 1] == 'm')
					{
						client_max_body_size = client_max_body_size.substr(0, client_max_body_size.length() - 1);
						client_max_body_size = ft_toString(utils::myStoi(client_max_body_size) * 1024 * 1024);
					}
					std::cout << "client_max_body_size: " << client_max_body_size << std::endl;
					tmploc.setClientMaxBodySize(client_max_body_size);
				}
			}
			if (token == "redirection")
			{
				std::string code = nextToken(line);
				std::string url = nextToken(line);
				std::pair<std::string, std::string> redirection = std::make_pair(code, url);
				tmploc.setRedirection(redirection);
			}
		token = nextToken(line);
		} while (token != "");
	}
	if (curlyBruh != 0)
		std::cout << "Error: Location curly brackets not balanced\n";
}

void	Config::parse(void)
{
	std::string	line;
	Server		tempServer;
	int			curlyBruh = 0;
	t_status	status = NO_SERVER_YET;

	while (std::getline(_Configfile, line))
	{
		std::string	token;

		utils::skipEmptyLines(_Configfile);
		do
		{
			utils::skipEmptyLines(_Configfile);
			token = nextToken(line);
			if ((status == NO_SERVER_YET && token != "server")
				|| (status == CONFIGURING_SERVER && token == "server"))
			{
				std::cout << "Error: Config file no bueno\n";
				std::cerr << line << " " << token << std::endl;
				while (std::getline(_Configfile, line))
					std::cout << line << std::endl;
				std::cerr << "Mmmh..."<< std::endl;
				return ;
			}
			else
				status = CONFIGURING_SERVER;

			if (token == "{")
			{
				curlyBruh++;
			}
			else if (token == "}")
			{
				curlyBruh--;
				if (curlyBruh < 0)
				{
					std::cout << "Error: Config file BRUH\n";
					return ;
				}
				else if (curlyBruh == 0)
				{
					/* here we push the configured new server */
					tempServer.setEnv(_env);
					_servers.push_back(tempServer);
					///qui temp_andrebbe svuotato
					tempServer.reset();
					status = NO_SERVER_YET;
					token = nextToken(line);
				}
			}
			else if (token == "listen")
			{
				tempServer.setPort(findPort(line));
			}
			else if (token == "server_name")
			{
				serverName(line, tempServer);
			}
			else if (token == "cgi")
			{
				std::string path = nextToken(line);
				std::string extension = nextToken(line);

				if (path.empty() || extension.empty())
					std::cout << "Error: cgi: path or extension not found\n";
				else if (extension[0] != '.' || extension.length() < 2)
					std::cout << "Error: cgi: extension not valid\n";
				else
					tempServer.setCgi(std::pair<std::string, std::string>(path, extension));
			}
			else if (token == "error_page")
			{
				std::string code = nextToken(line);
				std::string path = nextToken(line);
				std::vector<std::pair<std::string, std::string> > pages;
				
				if (code.empty() || path.empty())
					std::cout << "Error: error_page: code or path not found\n";
				while (!code.empty() && !path.empty())
				{
					pages.push_back(std::pair<std::string, std::string>(code, path));
					code = nextToken(line);
					path = nextToken(line);
				}
				tempServer.setErrorPages(pages);
			}
			else if (token == "upload_path")
			{
				std::string path = nextToken(line);
				if (path.empty())
					std::cout << "Error: upload_path: path not found\n";
				else
					tempServer.setUploadPath(path);
			}
			else if (token == "location")
			{
				parseLocation(line, tempServer);
			}
		} while (!token.empty());
	}

	if (curlyBruh != 0)
		std::cout << "cazzo fai fratm impara a scrivere coglione\n";


	std::cout << "Numero server: " << _servers.size() << std::endl;

	//stampa dei server tramite <<
	// std::cout << "\n\n\nStampa dei server tramite <<\n";
	// for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
	// 	std::cout << *it << std::endl;
	
}

static bool compare_location(Location &a,  Location &b)
{
    return (a.getLocationPath().length()) < (b.getLocationPath().length());
}

std::vector<Server> &Config::getServers(void)
{
	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
    {
        std::sort((it)->getLocations().begin(), (it)->getLocations().end(), compare_location);
    }
	return (this->_servers);
}
