#include "../includes/Config.hpp"

Config::Config(/* args */)
{
}

Config::~Config()
{
}

void Config::setEnv(char **env)
{
	this->_env = env;
}

char **Config::getEnv(void) const
{
	return (this->_env);
}