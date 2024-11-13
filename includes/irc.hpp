#pragma once

#include <iostream>
#include <exception>
#include <cstring>
#include <stdlib.h>
#include <sys/socket.h>	//socket
#include <netinet/in.h>	//sockaddr_in
#include <unistd.h>		//close
#include <cerrno>		//errno
#include <csignal>  	//signal
 #include <sys/fcntl.h>

#define _XOPEN_SOURCE_EXTENDED 1	//Special behavior for C++: you must use the _XOPEN_SOURCE_EXTENDED 1 feature test macro.

class Chat{
	private:
		struct sockaddr_in6	serverAddr;
		//pollfd			poll;
		unsigned int	port;
		std::string		password;
		int				socketFd;
		static bool		signal;

	public:
		Chat( void );
		Chat( char ** );
		Chat( Chat const & );
		Chat	&operator=( Chat const & );
		~Chat( void );

	unsigned int	getServerPort( void );
	void			createServerSocket( void );
	int				getServerFd( void );
	static void		signalHandler( int );
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
connect, send, sigaction, lseek, fstat,
poll (or equivalent)*/