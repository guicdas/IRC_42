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

# define ERR_USERNOTINCHANNEL	" :They aren't on that channel"
# define ERR_CHANOPRIVSNEEDED	" :You're not a channel operator"
# define ERR_NOSUCHNICK			" :Nickname is already in use"
# define ERR_NOSUCHCHANNEL		" :No such channel"
# define ERR_NOTONCHANNEL		" :You're not on that channel"
# define ERR_NEEDMOREPARAMS 	" :Not enough parameters"
# define ERR_ALREADYREGISTERED 	" :You may not reregister"


typedef struct s_channel t_channel;

typedef struct s_client{
	int fd;
	std::string buffer;
	std::vector< std::string > args;

	std::string	nickname;
	/*	n pode ter ':' a nao ser no inicio*/
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
		std::map< std::string, int(Server::*)( t_client & )>	commands;
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
	void	iterateClients( void );

	int		clientRead( t_client & );
	int		parseCommand( t_client & , std::string );

	int		list( t_client & );
	int		join( t_client & );
	int		nick( t_client & );
	int		quit( t_client & );
	int		mode( t_client & );
	int		user( t_client & );
	int		part( t_client & );
	int		kick( t_client & );
	int		cap( t_client & );
	int		who( t_client & );
	int		privmsg( t_client & );

	int		checkChannelNameExists( std::string );
	int		checkClientNickExists( std::string );
	int		sendMsgToUser( std::string, std::string );
	void	eraseClientFromAllChannels( t_client & );

	t_channel	*getChannel( std::string );
	t_client	&getClient( std::string );
	void		addUserToChannel( t_client &, t_channel * );
};

void	clientWrite( t_client & );
void	putInBuf( t_client &, int, std::string , std::string );
int		isClientInChannel( t_client &, t_channel * );
int		sendMsgToChannel( t_channel &, std::string , std::string );
void	eraseClientFromChannel( t_client &, t_channel * );
int		isClientOp( t_client &, t_channel * );
void	listChannelMembers( t_client &, t_channel * );

class FileException : public std::exception{
	private:
		std::string msg;

	public:
		FileException( const char * );
		~FileException( void ) throw();
		virtual const char* what() const throw();
};
