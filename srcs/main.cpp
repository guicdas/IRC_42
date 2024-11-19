#include "../includes/fd.hpp"
#include "../includes/irc.hpp" 

int main( int ac, char **av ){
	try
	{
		t_env	e;

		if (ac != 3)
			throw(FileException("Wrong number of arguments!"));
		if (initFds(&e) == 0)
			throw (FileException("Error: initializing fds"));

		Server	Server(av);
		Server.createServerSocket(&e);
		Server.loop(&e);
	}
	catch(const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}