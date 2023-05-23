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

bool isEmpty(std::string File)
{
	std::ifstream file(File.c_str());
	return file.peek() == std::ifstream::traits_type::eof();
}

void Config::setConfig(char *filePath)
{
	this->_FilePath = filePath;
	this->_Configfile.open(_FilePath.c_str());

	if (!_Configfile.is_open() || isEmpty(_FilePath))
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

bool isKey(char c)
{
	return (c == '_' || c == ':' || c == '/' || c == '\\' || c == '.' || c == '-' || isalnum(c));
}

std::string nextToken(std::string &fLine)
{
	int			i = 0;
	std::string ret = "";

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
				std::cout << ret << std::endl;
			}
			break ;
		}
		i++;
	}
	//estraggo da line i token
	fLine = fLine.substr(i, fLine.length() - i);
	return (ret);
}

/*
**	1) leggo il file una riga alla volta
**	2) estraggo i token
**	3) quando leggo la porta, creo un nuovo server e lo pusho nel vector di server
      vector_di_server.push_back(*(new Server(porta)));
**	4) aggiungo le info al server tramite i setter
*/

void Config::parse()
{
	int 						start = 0;
	// int							n_servers = 0; //conta i blocchi server
	std::string					line; //per leggere il file
	std::vector<std::string>	pg; //conta le parentesi graffe
	

	while (getline(_Configfile, line))
	{
		SkipEmptyLines(_Configfile);
		std::string token;
		while ((token = nextToken(line)) != "")
		{
			SkipEmptyLines(_Configfile);
			if (!start && token != "server") //il config deve partire con server
				std::cerr << "Error: config File\n";
			else
				start = 1;
			if (token == "{")
				pg.push_back(token);
			else if (token == "}")
			{
				if (pg.size() != 0)
					pg.pop_back();
				else
					std::cerr << "Error: curly phrases\n";
			}
			else if (token == "server")
			{
				token = nextToken(line);
				std::cout << token << std::endl;
				if ((token = nextToken(line)) != "listen")
				{
					std::cerr << "Error: Listen not found on first line\n";
					return ;
				}
				else
				{
					std::cout << line << std::endl;
				}
			}
		}
	}
}

bool Config::SkipEmptyLines(std::ifstream &file)
{	
	while (file.peek() == '\n' || file.peek() == '\t' || file.peek() == ' ')
		file.get();
	return (1);
}

//la ref ordina i server per location. non e' detto che serva ma teniamolo presente
std::vector<Server> &Config::getServers(void)
{
	return (this->_Servers);
}