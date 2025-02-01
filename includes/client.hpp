#pragma once

#include "irc.hpp"

class Channel;

class Client
{
	private:
		int							fd;
		std::string					nickname;
		std::string					realname;
		std::string					username;
		std::string					id;
		bool						registered;

	public:
		std::string					buffer;
		std::vector< Channel >		channels;
		std::vector< std::string >	args;

		Client( int );
		Client( Client const & );
		Client	&operator=( Client const & );
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

	void		verifyClientRegistered( void );
	void		verifyValidNick( void );
	std::string resolveHostname( int );
	std::string	createId( void );

};