#pragma once

#include <iostream>
#include <exception>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <arpa/inet.h>		//inet_ntoa
#include <netinet/in.h>		//sockaddr_in
#include <unistd.h>			//close
#include <cerrno>			//errno
#include <sys/fcntl.h>
# include <sys/select.h>	//select
#include <netdb.h>			//getproto
#include <vector>
#include <map>

//#define _XOPEN_SOURCE_EXTENDED 1	//Special behavior for C++: you must use the _XOPEN_SOURCE_EXTENDED 1 feature test macro.

typedef struct s_client{
	int fd;
	std::string buffer;
	std::string	name;
} t_client;

typedef struct s_channel{
	std::vector< t_client > moderators;
	std::vector< t_client > users;
} t_channel;

class Server
{
	private:
		std::map<std::string, void(Server::*)( t_client, std::string )>	commands;
		std::map<std::string, std::string>	channels;
		/*The channel is created implicitly when the first client joins it, and the channel ceases to exist when the last client leaves it.
		While the channel exists, any client can reference the channel using the name of the channel.*/
		struct sockaddr_in		serverAddr;
		std::vector<t_client>	clients;
		std::string				password;
		unsigned int			port;
		int						serverSocket;
		fd_set					fdList;
		fd_set					fdWrite;
		fd_set					fdRead;
		fd_set					fdExcep;
		int						maxFds;

	public:
		Server( void );
		Server( char ** );
		Server( Server const & );
		Server	&operator=( Server const & );
		~Server( void );

	void	createServerSocket( void );
	void	createCommandMap( void );
	void	loop( void );
	void	acceptClient( void );
	void	clientWrite( t_client );
	void	clientRead( t_client );

	void	list( t_client, std::string );
	void	join( t_client, std::string );
	void	nick( t_client, std::string );
};

class FileException : public std::exception{
	private:
		std::string msg;

	public:
		FileException( const char * );
		~FileException( void ) throw();
		virtual const char* what() const throw();
};
