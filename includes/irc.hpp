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
# define ERR_PASSWDMISMATCH		" :Password incorrect"
# define ERR_USERSDONTMATCH		" :Cant change mode for other users"

class Client;

class Channel
{

	public:
		std::vector< Client > operators;
		std::vector< Client > clients;
		std::string				name;
		//may not contain any spaces, a control G / BELL ('^G', 0x07), or a comma
		std::string				topic;
		/*he topic is a line shown to all users when they join the channel, and all users in the channel are notified when the topic of a channel is changed
		channel ceases to exist when the last client leaves it.*/


};

class Client
{
	private:
		int							fd;
		std::string					nickname;
		std::string					realname;
		std::string					username;
		bool						registered;
	
	public:
		std::vector< Channel >		channels;
		std::vector< std::string >	args;
		std::string					buffer;

		Client( int	clientSocket );
		Client( Client const &c );
		Client	&operator=( Client const &c );
		~Client( void );

	std::string	getNick( void );
	int			getFd( void );
	void		setUser( std::string );
	void		setRealname( std::string );

	void		verifyClientRegistered( void );
	void		verifyValidNick( void );
};

class Server
{
	private:
		std::map< std::string, int(Server::*)( Client & )>	commands;
		std::vector< Channel >	channels;
		std::vector< Client >		clients;
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

	int			clientRead( Client & );
	int			parseCommand( Client &client, std::string s );

	int		list( Client & );
	int		join( Client & );
	int		nick( Client & );
	int		quit( Client & );
	int		mode( Client & );
	int		user( Client & );
	int		part( Client & );
	int		kick( Client & );
	int		pass( Client & );
	int		cap( Client & );
	int		who( Client & );
	int		privmsg( Client & );

	void	checkChannelNameExists( std::string );
	void	checkClientNickExists( std::string );
	int		sendMsgToUser( std::string, std::string );
	void	eraseClientFromAllChannels( Client & );

	Channel	*getChannel( std::string );
	Client	&getClient( std::string );
	void		addUserToChannel( Client &, Channel * );
};

void	clientWrite( Client & );
void	putInBuf( Client &, int, std::string , std::string );

	void	listChannelMembers( Client &, Channel * );
	int		sendMsgToChannel( Channel &, std::string, std::string );
	void	eraseClientFromChannel( Client &, Channel * );

	void	checkClientInChannel( Client &, Channel * );
	void	checkClientOp( Client &, Channel * );

class FileException : public std::exception{
	private:
		std::string msg;

	public:
		FileException( const char * );
		~FileException( void ) throw();
		virtual const char* what() const throw();
};
