#include "../includes/Config.hpp"
#include "Config.hpp"

Config::Config(char **env)
{

}

Config::~Config()
{
}

bool isEmpty(std::string File)
{
	std::ifstream file(File);
	return file.peek() == std::ifstream::traits_type::eof();
}

void Config::setConfig(std::string filePath)
{
	this->_FilePath = filePath;
	this->_Configfile.open(_FilePath.c_str());

	if (!_Configfile.is_open() || !isEmpty(_FilePath))
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
		if ( fLine[i] == '{' || fLine[i] == '}') 
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

void Config::parse()
{
	int 						start = 0;
	int							n_servers = 0; //conta i blocchi server
	std::string					line; //per leggere il file
	std::vector<std::string>	pg; //conta le parentesi graffe

	while (getline(_Configfile, line))
	{
		SkipEmptyLines(_Configfile);
		std::string token;
		while ((token = nextToken(line)) != "")
		{
			SkipEmptyLines(_Configfile);
		}
	}
}

bool Config::SkipEmptyLines(std::ifstream &file)
{	
	while (file.peek() == '\n' || file.peek() == '\t' || file.peek() == ' ')
		file.get();
	return (1);
}
