#include "../includes/Config.hpp"

Config::Config()
{
}

Config::Config(char **env): _env(env)
{
}

Config::~Config()
{
	for (size_t i = 0; i < _servers.size(); i++)
		delete _servers[i];
}

void Config::setConfig(char *filePath)
{
	this->_filePath = filePath;
	this->_configfile.open(_filePath.c_str());

	if (!_configfile.is_open() || utils::isEmpty(_filePath))
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
		amc::lerr << "Error: Server name is empty\n";
	while (!name.empty())
	{
		servernames.push_back(name);
		name = nextToken(line);
	}
	server.setServerNames(servernames);
}

void Config::parseLocation(std::string &line, Server server)
{
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
				curlyBruh--;
			else if (token == "autoindex")
			{
				std::string autoindex = nextToken(line);
				tmploc.setAutoIndex(autoindex);
			}
			else if (token == "index")
			{
				std::string index = nextToken(line);
				tmploc.setIndex(index);
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
					if (!utils::check_methods)
					{
						amc::lerr << "Error: Method not valid\n";
						continue ;
					}
					else
					{
						allow_methods.push_back(method);
						method = nextToken(line);
					}
				}
			}
			else if (token == "upload_path")
			{
				std::string upload_path = nextToken(line);
				tmploc.setUploadPath(upload_path);
			}
			else if (token == "client_max_body_size")
			{
				std::string client_max_body_size = nextToken(line);
				tmploc.setClientMaxBodySize(client_max_body_size);
			}

		} while (token != "");
	}
}

void	Config::parse(void)
{
	std::string	line;
	Server		tempServer;
	int			curlyBruh = 0;
	t_status	status = NO_SERVER_YET;

	while (std::getline(_configfile, line))
	{
		std::string	token;

		utils::skipEmptyLines(_configfile);
		do
		{
			utils::skipEmptyLines(_configfile);
			token = nextToken(line);
			std::cout << "token: " << token << std::endl;
			std::cout << "status: " << status << std::endl;
			if ((status == NO_SERVER_YET && token != "server")
				|| (status == CONFIGURING_SERVER && token == "server"))
			{
				amc::lerr << "Error: Config file no bueno\n";
				std::cerr << line << " " << token << std::endl;
				while (std::getline(_configfile, line))
					amc::lwar << line << std::endl;
				std::cerr << "Mmmh..." << RESET << std::endl;
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
					amc::lerr << "Error: Config file BRUH\n";
					return ;
				}
				else if (curlyBruh == 0)
				{
					/* here we push the configured new server */
					_servers.push_back(new Server(tempServer));
					status = NO_SERVER_YET;
					token = nextToken(line);
				}
			}
			else if (token == "listen")
			{
				tempServer.setPort(findPort(line));
			}
			else if (token == "server name")
			{
				serverName(line, tempServer);
			}
			else if (token == "cgi")
			{
				std::string path = nextToken(line);
				std::string extension = nextToken(line);

				if (path.empty() || extension.empty())
					amc::lerr << "Error: cgi: path or extension not found\n";
				else if (extension[0] != '.' || extension.length() < 2)
					amc::lerr << "Error: cgi: extension not valid\n";
				else
					tempServer.setCgi(std::pair<std::string, std::string>(path, extension));
			}
			else if (token == "error_page")
			{
				std::string code = nextToken(line);
				std::string path = nextToken(line);
				std::vector<std::pair<std::string, std::string> > pages;
				
				if (code.empty() || path.empty())
					amc::lerr << "Error: error_page: code or path not found\n";
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
					amc::lerr << "Error: upload_path: path not found\n";
				else
					tempServer.setUploadPath(path);
			}
			else if (token == "location")
				parseLocation(line, tempServer);
		} while (!token.empty());
	}

	if (curlyBruh != 0)
		amc::lerr << "cazzo fai fratm impara a scrivere coglione\n";

	std::cout << "Numero server: " << _servers.size() << std::endl;
}

//void Config::parse()
//{
//	int 						start = 0;
//	int							n_servers = 0; //conta i blocchi server // ccantale::Questa secondo me non server
//	int							curlyBruh = 0;
//	std::string					line; //per leggere il file
//	
//	while (getline(_configfile, line))
//	{
//		utils::skipEmptyLines(_configfile);
//		std::string token;
//		while ((token = nextToken(line)) != "")
//		{
//			utils::skipEmptyLines(_configfile);
//			if (!start && token != "server") //il config deve partire con server
//			{
//				std::cerr << "Error: config File\n";
//				std::cout << start  <<  " " << curlyBruh  << " "<< token << std::endl;
//			}
//			else if (token == "server")
//			{
//				if (start == 1)
//					std::cerr << "bisogna buttare tutto fa cagare\n";
//				n_servers++;
//				start = 1;
//			}
//			if (token == "{")
//				curlyBruh++;
//			else if (token == "}")
//			{
//				curlyBruh--;
//				if (curlyBruh < 0)
//					std::cerr << "Error: curly phrases\n";
//				else if (curlyBruh == 0)
//					start = 0;
//			}
//			else if (token == "listen")
//			{
//				int	port = findPort(line);
//				_servers.push_back(new Server(port));
//			}
//			else if (token == "server_name")
//				serverName(line, *_servers.back());
//			else if (token == "cgi")
//			{
//				std::string path = nextToken(line);
//				std::string extension = nextToken(line);
//
//				if (path.empty() || extension.empty())
//					std::cerr << "Error: cgi: path or extension not found\n";
//				else if (extension[0] != '.' || extension.length() < 2)
//					std::cerr << "Error: cgi: extension not valid\n";
//				else
//					_servers.back()->setCgi(std::pair<std::string, std::string>(path, extension));
//			}
//	
//			else if (token == "error_page")
//			{
//				std::string code = nextToken(line);
//				std::string path = nextToken(line);
//				if (code.empty() || path.empty())
//					std::cerr << "Error: error_page: code or path not found\n";
//				std::vector<std::pair<std::string, std::string> > pages;
//				while (!code.empty() && !path.empty())
//				{
//					pages.push_back(std::pair<std::string, std::string>(code, path));
//					code = nextToken(line);
//					path = nextToken(line);
//				}
//				//da definire ancora. dopo questo rimane upload_path e poi location
//				// pages.push_back(std::pair<std::string, std::string>(code, path));
//				_servers.back()->setErrorPages(pages);
//			// for (size_t i = 0; i < _servers.back()->getErrorPages().size(); i++)
//			// 	{
//			// 		std::cout << _servers.back()->getErrorPages()[i].first << " " << _servers.back()->getErrorPages()[i].second << std::endl;
//			// 	}
//			}
//			else if (token == "upload_path")
//			{
//				std::string path = nextToken(line);
//				if (path.empty())
//					std::cerr << "Error: upload_path: path not found\n";
//				else
//					_servers.back()->setUploadPath(path);
//			}
//			//else if (token == "location")
//				//location(line, *_servers.back()); //dio mio che palle
//		}
//	}
//	if (curlyBruh != 0)
//		std::cerr << "cazzo fai fratm impara a scrivere coglione\n";
//}

//la ref ordina i server per location. non e' detto che serva ma teniamolo presente
std::vector<Server *> &Config::getServers(void)
{
	return (this->_servers);
}
