#include "../includes/Config.hpp"
#include "Config.hpp"

Config::Config(/* args */)
{
}

Config::~Config()
{
}

void Config::setConfig(std::string filePath)
{
	this->_FilePath = filePath;
	this->_Configfile.open(_FilePath.c_str());

	if (!_Configfile.is_open() || !_Configfile)

}

void Config::setEnv(char **env)
{
	this->_env = env;
}

char **Config::getEnv(void) const
{
	return (this->_env);
}