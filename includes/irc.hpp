#pragma once

#include <iostream>
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

# define ENDL				"." << std::endl
# define buf(c,err,str,cmd)	(putInBuf(c,err,str,cmd))

# define USERLEN 15 //maximo tamanho para username

# define ERR_NEEDMOREPARAMS 	" :Not enough parameters"
# define ERR_ALREADYREGISTERED 	" :You may not reregister"


typedef struct s_channel t_channel;

typedef struct s_client{
	int fd;
	std::string buffer;
	std::vector< std::string > args;

	std::string	nickname;
	//They MUST NOT start with a character listed as a channel type, channel membership prefix, or prefix listed in the IRCv3 multi-prefix Extension.
	/*	supostamente nao pode comecar com $ mas tb da erro se tiver no meio
	n pode ter ':' a nao ser no inicio*/
	std::string	realname;
	std::string	username;
	std::vector< t_channel > channels;
	
} t_client;

/*A user may be joined to several channels at once, but a limit may be imposed by the server*/

typedef struct s_channel{
	std::vector< t_client > operators;
	std::vector< t_client > clients;
	std::string				name;
	//may not contain any spaces, a control G / BELL ('^G', 0x07), or a comma
	std::string				topic;
	/*he topic is a line shown to all users when they join the channel, and all users in the channel are notified when the topic of a channel is changed
	channel ceases to exist when the last client leaves it.*/
} t_channel;

class Server
{
	private:
		std::map< std::string, void(Server::*)( t_client & )>	commands;
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

	int		clientRead( t_client & );
	void	parseCommand( t_client & , std::string );

	void	list( t_client & );
	void	join( t_client & );
	void	nick( t_client & );
	void	quit( t_client & );
	void	mode( t_client & );
	void	user( t_client & );
	void	cap( t_client & );
	void	privmsg( t_client & );

	int		checkChannelNameExists( std::string );
	int		checkClientNickExists( std::string );
	int		sendMsgToUser( std::string, std::string );

	t_channel	*getChannel( std::string );
	void		addUserToChannel( t_client &, t_channel * );
};

void	clientWrite( t_client & );
void	putInBuf( t_client &, int, std::string , std::string );
int		isClientInChannel( t_client &, t_channel * );
int		sendMsgToChannel( t_channel &, std::string , std::string );

class FileException : public std::exception{
	private:
		std::string msg;

	public:
		FileException( const char * );
		~FileException( void ) throw();
		virtual const char* what() const throw();
};
