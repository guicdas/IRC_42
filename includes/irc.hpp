#pragma once

#include <iostream>
#include <iomanip>			// getline
#include <sstream>			// stringstream
#include <exception>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <arpa/inet.h>		// inet_ntoa
#include <netinet/in.h>		// sockaddr_in
#include <unistd.h>			// close
#include <cerrno>			// errno
#include <sys/fcntl.h>
#include <sys/select.h>		// select
#include <netdb.h>			// getproto
#include <vector>
#include <map>
#include <iterator>			// advance, distance

//#define _XOPEN_SOURCE_EXTENDED 1	//Special behavior for C++: you must use the _XOPEN_SOURCE_EXTENDED 1 feature test macro.
#define ENDL "." << std::endl

typedef struct s_client{
	int fd;
	std::string buffer;
	std::string	name;
	std::vector< std::string > args;
} t_client;
/*A user may be joined to several channels at once, but a limit may be imposed by the server as to how many channels a client can be in at one time
channel ceases to exist when the last client leaves it.*/

typedef struct s_channel{
	std::vector< t_client > operators;
	std::vector< t_client > clients;
	std::string				name;
	//may not contain any spaces, a control G / BELL ('^G', 0x07), or a comma
	std::string				topic;
	/*he topic is a line shown to all users when they join the channel, and all users in the channel are notified when the topic of a channel is changed*/
} t_channel;

class Server
{
	private:
		std::map< std::string, void(Server::*)( t_client *, std::vector<std::string> )>	commands;
		std::vector< t_channel >	channels;
		std::vector< t_client >		clients;
		struct sockaddr_in			serverAddr;
		std::string					password;
		unsigned int				port;
		int							serverSocket;
		fd_set						fdList;
		fd_set						fdWrite;
		fd_set						fdRead;
		fd_set						fdExcep;
		int							maxFds;

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

	int		clientRead( t_client * );
	void	parseCommand( t_client *client, std::string s );

	void	clientWrite( t_client * );

	void	list( t_client * , std::vector< std::string > );
	void	join( t_client * , std::vector< std::string > );
	void	nick( t_client * , std::vector< std::string > );
	void	quit( t_client * , std::vector< std::string > );
};

std::vector<std::string>	ircSplit(std::string str, char c);

class FileException : public std::exception{
	private:
		std::string msg;

	public:
		FileException( const char * );
		~FileException( void ) throw();
		virtual const char* what() const throw();
};
