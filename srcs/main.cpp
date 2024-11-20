#include "../includes/irc.hpp" 

int main( int ac, char **av ){
	try
	{
		if (ac != 3)
			throw(FileException("Wrong number of arguments!"));

		Server	Server(av);
		Server.createServerSocket();
		Server.loop();
	}
	catch(const std::exception &e){std::cerr << e.what() << std::endl;}
}