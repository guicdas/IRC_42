#pragma once

#include <iostream>
#include <exception>
#include <cstring>
#include <stdlib.h>
#include <sys/socket.h>	//socket
#include <netinet/in.h>	//sockaddr_in
#include <unistd.h>		//close
#include <cerrno>		//errno

#define _XOPEN_SOURCE_EXTENDED 1	//Special behavior for C++: you must use the _XOPEN_SOURCE_EXTENDED 1 feature test macro.

class Chat{
	private:
		unsigned int	port;
		std::string		password;
		int				socketFd;
		sockaddr_in6	serverAddr;

	public:
		Chat( void );
		Chat( char ** );
		Chat( Chat const & );
		Chat	&operator=( Chat const & );
		~Chat( void );

	void	createServerSocket( void );
	void	prepareServerSocket( void );
	int		getServerFd( void );
	unsigned int	getServerPort( void );
};

class FileException : public std::exception{
	private:
		std::string msg;
	
	public:
		FileException( const char * );
		~FileException( void ) throw();
		virtual const char* what() const throw();
};

/*
getsockname, getprotobyname, gethostbyname, getaddrinfo, freeaddrinfo, 
connect, inet_addr, inet_ntoa, send,
signal, sigaction, lseek, fstat, fcntl,
poll (or equivalent)*/