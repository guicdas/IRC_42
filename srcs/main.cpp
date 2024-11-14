#include "../includes/irc.hpp"

int main( int ac, char **av ){
	try
	{
		if (ac != 3)
			throw(FileException("Wrong number of arguments!"));
		
		Server Server(av);

		std::signal(SIGINT, Server::signalHandler);
		std::signal(SIGQUIT, Server::signalHandler);
		Server.createServerSocket();
		std::cout << "Server listening on port " << Server.getServerPort() << std::endl;
		Server.acceptClient();
		Server.loop();
		//if (close(Server.getServerPort()) == -1)
		//	throw (FileException("Error: closing sockets: "));
	}
	catch(const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}