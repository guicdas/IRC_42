#pragma once

#include <iostream>
#include <exception>
#include <cstring>
#include <sys/socket.h>	//socket
#include <netinet/in.h>	//sockaddr_in
#include <unistd.h>		//close
#include <cerrno>		//errno

#define _XOPEN_SOURCE_EXTENDED 1	//Special behavior for C++: To use this function with C++, you must use the _XOPEN_SOURCE_EXTENDED 1 feature test macro.

class Chat{
	private:

	public:
		Chat( void );
		Chat( Chat const & );
		Chat	&operator=( Chat const & );
		~Chat();

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
setsockopt, getsockname,
getprotobyname, gethostbyname, getaddrinfo, freeaddrinfo, 
connect, inet_addr, inet_ntoa, send,
signal, sigaction, lseek, fstat, fcntl,
poll (or equivalent)*/