#include "../includes/irc.hpp" 

static void validate_arguments(int ac, char **av){
	if (ac != 3)
		throw(FileException("Wrong number of arguments!"));
	if (std::strlen(av[1]) < 1 || std::strlen(av[2]) < 1)
		throw (FileException("Arguments cannot be empty!"));
	for (int i = 0; av[1][i]; i++){
		if (av[1][i] < '0' || av[1][i] > '9')
			throw (FileException("port must not contain letters!"));
	}
	int num = atoi(av[1]);
	if (num < 0 || num > 65535)
		throw (FileException("invalid port!"));
}

int main( int ac, char **av ){
	try
	{
		validate_arguments(ac, av);

		Server Server(av);
		Server.createServerSocket();
		Server.createCommandMap();
		Server.loop();
	}
	catch(const std::exception &e){
		std::cerr << e.what() << std::endl;
	}
}

FileException::FileException( const char* msg ){
	this->msg = msg;
	if (std::strcmp(strerror(errno), "Success") != 0)
	{
		this->msg += " ";
		this->msg += strerror(errno);
	}
}
FileException::~FileException( void ) throw() {}
const char * FileException::what( void ) const throw() {
	return (msg.c_str());
}

