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
}

void Config::setEnv(char **env)
{
	this->_env = env;
}

char **Config::getEnv(void) const
{
	return (this->_env);
}