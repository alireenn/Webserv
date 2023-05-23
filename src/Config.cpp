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

void findPort(std::string &line, Server server)
{
	std::string port = nextToken(line);
	if (port.empty())
		std::cerr << "Error: Listen: port not found\n";
	else
	{
		if (my_stoi(port) >= 0 && my_stoi(port) <= 65536)
			std::cerr << "Error: Listen: port not valid\n";
		else
			server.setPort(my_stoi(port));
	}
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
	int							n_servers = 0; //conta i blocchi server // ccantale::Questa secondo me non server
	int							curlyBruh = 0;
	std::string					line; //per leggere il file
	
	while (getline(_configfile, line))
	{
		SkipEmptyLines(_configfile);
		std::string token;
		while ((token = nextToken(line)) != "")
		{
			SkipEmptyLines(_configfile);
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
			// else if (token == "listen")
				// findPort(line, server); //questo funziona se usiamo setter e getter
		}
	}
	if (curlyBruh != 0)
		std::cerr << "cazzo fai fratm impara a scrivere coglione\n";
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
