#include "includes/Webserv.hpp"


int main(int argc, char** argv, char** env)
{
	Config conf;
	IOMultiplexing io;

	signal(SIGPIPE, SIG_IGN);//La chiamata a signal(SIGPIPE, SIG_IGN) 
	//imposta il gestore dei segnali per il segnale SIGPIPE in modo da ignorare il segnale quando viene ricevuto,
	//anziché gestirlo con il comportamento predefinito del sistema operativo, che di solito è terminare il processo.
	if (argc != 1 && argc != 2)
    {
        std::cout << "./webserv || ./webserv <path>" << std::endl;
        return (1);
    }
	else if (argc == 1)
	{
		conf.setConfigPath("./config.d/default.conf");
		conf.setEnv(env);
		io.SetupServers(conf);
	}
	else if (argc == 2)
	{
		conf.setConfigPath(argv[1]);
		conf.setEnv(env);
		io.SetupServers(conf);
	}
	return 0;
}