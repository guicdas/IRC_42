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

# define RESET          "\033[0m"
# define RED            "\033[31m"
# define GREEN          "\033[32m"
# define MAGENTA        "\033[35m"
# define YELLOW         "\033[33m"
# define BLUE           "\033[34m"
# define CYAN           "\033[36m"
# define PRINT_COLOR(color, text) (std::cout << color << text << RESET << std::endl)
# define PRINT_ERROR(color, text) (std::cerr << color << text << RESET << std::endl)

# define USERLEN 15 //maximo tamanho para username

# define ERR_USERNOTINCHANNEL	" :They aren't on that channel"
# define ERR_CHANOPRIVSNEEDED	" :You're not a channel operator"
# define ERR_NOSUCHNICK			" :Nickname is already in use"
# define ERR_NOSUCHCHANNEL		" :No such channel"
# define ERR_USERONCHANNEL		" :User alredy on the channel"
# define ERR_NOTONCHANNEL		" :You're not on that channel"
# define ERR_NEEDMOREPARAMS 	" :Not enough parameters"
# define ERR_ALREADYREGISTERED 	" :You may not reregister"
# define ERR_PASSWDMISMATCH		" :Password incorrect"
# define ERR_USERSDONTMATCH		" :Cant change mode for other users"
# define ERR_ERRONEUSNICKNAME	" :Erroneous nickname"
# define ERR_NONICKNAMEGIVEN	" :No nickname given"
# define ERR_NORECIPIENT		" :No recipient given (command)"
# define ERR_NOTEXTTOSEND		" :No text to send"
//# define ERR_NOSUCHNICK			" :Cannot send to nick/channel"
# define ERR_NICKNAMEINUSE		" :Nickname is already in use"

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
		int				fd;
		std::string		nickname;
		std::string		realname;
		std::string		username;
		bool			registered;
		std::string		hostname;
		std::string		id;
	
	public:
		std::string					buffer;
		std::vector< Channel >		channels;
		std::vector< std::string >	args;

		Client( int	clientSocket );
		Client( Client const &c );
		Client	&operator=( Client const &c );
		~Client( void );

	std::string	getNick( void );
	std::string	getUser( void );
	std::string	getRealname( void );
	int			getFd( void );
	std::string getId( void );
	bool		getRegisterd( void );

	void		setNick( std::string );
	void		setUser( std::string );
	void		setRealname( std::string );
	void		setId( std::string );
	void		setRegisterd( bool );

	void 		resolveHostname( int );
	void 		setHostname(const std::string& hostname);
	void 		setId( void );

	void		verifyClientRegistered( void );
	void		verifyValidNick( void );
	std::string resolveHostname( int );
	std::string	createId( void );
};

class Server
{
	private:
		std::map< std::string, int(Server::*)( Client & )>	commands;
		std::string					password;
		unsigned int				port;
		struct sockaddr_in			serverAddr;
		int							serverSocket;
		fd_set						fdList;
		fd_set						fdWrite;
		fd_set						fdRead;
		fd_set						fdExcep;
		int							maxFds;

		std::vector< Client >		clients;
		std::vector< Channel >		channels;

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

	int		topic( Client &);
	int		invite( Client &);

	void	checkChannelNameExists( std::string );
	int		doesChannelNameExist( std::string arg );
	int		isClientInChannel( std::string nick, std::string channel );
	void	checkClientNickExists( std::string );
	int		sendMsgToUser( std::string, std::string );
	void	eraseClientFromAllChannels( Client & );

	Channel	*getChannel( std::string );
	Client	&getClient( std::string );
	void	addUserToChannel( Client &, Channel * );
	
	int		ChannelNameExists( std::string );
};

	int		isClientInChannel( Client &, Channel * );
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
