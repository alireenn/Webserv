#include "../includes/Config.hpp"

static int my_stoi(const std::string& str) {
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

bool isEmpty(std::string File)
{
	std::ifstream file(File.c_str());
	return file.peek() == std::ifstream::traits_type::eof();
}

void Config::setConfig(char *filePath)
{
	this->_filePath = filePath;
	this->_configfile.open(_filePath.c_str());

	if (!_configfile.is_open() || isEmpty(_filePath))
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

static bool isKey(char c)
{
	return (c == '_' || c == ':' || c == '/' || c == '\\'
				|| c == '.' || c == '-' || isalnum(c));
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
		if (isKey(fLine[i]))
		{
			while (isKey(fLine[i]))
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
		if (my_stoi(port) <= 0 && my_stoi(port) >= 65536)
		{
			std::cerr << "Error: Listen: port not valid\n";
			return (0);
		}
		else
			return(my_stoi(port));
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
		std::cerr << "Error: Server name is empty\n";
	while (!name.empty())
	{
		servernames.push_back(name);
		name = nextToken(line);
	}
	server.setServerNames(servernames);
}

void Config::parse()
{
	int 						start = 0;
	int							n_servers = 0; //conta i blocchi server // ccantale::Questa secondo me non server
	int							curlyBruh = 0;
	std::string					line; //per leggere il file
	
	while (getline(_configfile, line))
	{
		utils::skipEmptyLines(_configfile);
		std::string token;
		while ((token = nextToken(line)) != "")
		{
			utils::skipEmptyLines(_configfile);
			if (!start && token != "server") //il config deve partire con server
			{
				std::cerr << "Error: config File\n";
				std::cout << start  <<  " " << curlyBruh  << " "<< token << std::endl;
			}
			else if (token == "server")
			{
				if (start == 1)
					std::cerr << "bisogna buttare tutto fa cagare\n";
				n_servers++;
				start = 1;
			}
			if (token == "{")
				curlyBruh++;
			else if (token == "}")
			{
				curlyBruh--;
				if (curlyBruh < 0)
					std::cerr << "Error: curly phrases\n";
				else if (curlyBruh == 0)
					start = 0;
			}
			else if (token == "listen")
			{
				int	port = findPort(line);
				_servers.push_back(new Server(port));
			}
			else if (token == "server_name")
				serverName(line, *_servers.back());
			else if (token == "cgi")
			{
				std::string path = nextToken(line);
				std::string extension = nextToken(line);

				if (path.empty() || extension.empty())
					std::cerr << "Error: cgi: path or extension not found\n";
				else if (extension[0] != '.' || extension.length() < 2)
					std::cerr << "Error: cgi: extension not valid\n";
				else
					_servers.back()->setCgi(std::pair<std::string, std::string>(path, extension));
				// for (size_t i = 0; i < _servers.back()->getcgi().size(); i++)
				// {
				// 	std::cout << _servers.back()->getcgi()[i].first << " " << _servers.back()->getcgi()[i].second << std::endl;
				// }
			}
			else if (token == "error_page")
			{
				std::string code = nextToken(line);
				std::string path = nextToken(line);
				if (code.empty() || path.empty())
					std::cerr << "Error: error_page: code or path not found\n";
				std::vector<std::pair<std::string, std::string> > pages;
				//da definire ancora. dopo questo rimane upload_path
			}
		}
	}
	if (curlyBruh != 0)
		std::cerr << "cazzo fai fratm impara a scrivere coglione\n";
}

//la ref ordina i server per location. non e' detto che serva ma teniamolo presente
std::vector<Server *> &Config::getServers(void)
{
	return (this->_servers);
}