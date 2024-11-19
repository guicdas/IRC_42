#pragma once

#include <iostream>
#include <exception>
#include <cstring>
#include <stdlib.h>
//#include <sys/socket.h>		//socket
#include <arpa/inet.h>		//inet_ntoa
#include <netinet/in.h>		//sockaddr_in
#include <unistd.h>			//close
#include <cerrno>			//errno
//#include <csignal>  		//signal
#include <sys/fcntl.h>
# include <sys/select.h>	//select
#include <netdb.h>			//getproto

#include "fd.hpp"

#define _XOPEN_SOURCE_EXTENDED 1	//Special behavior for C++: you must use the _XOPEN_SOURCE_EXTENDED 1 feature test macro.
#ifndef BUF_SIZE
# define BUF_SIZE 4096
#endif

typedef struct s_env t_env;

typedef enum {
	FREE = 0,
	SERVER = 1,
	CLIENT = 2
} fdType;

class Server{
	private:
		struct sockaddr_in	serverAddr;
		struct sockaddr_in	clientAddr;
		std::string			password;
		unsigned int		port;
		int					serverSocket;
		int					clientSocket;
		fd_set				fdWrite;
		fd_set				fdRead;
		fd_set				fdExcep;

	public:
		Server( void );
		Server( char ** );
		Server( Server const & );
		Server	&operator=( Server const & );
		~Server( void );

	void	createServerSocket( t_env * );
	void	loop( t_env * );
	void	acceptClient( t_env * );
	void	clientWrite( t_env * );
	void	clientRead( t_env * );
};

typedef struct s_env{
	t_fd *fds;
	int	maxFd;
} t_env;

class FileException : public std::exception{
	private:
		std::string msg;

	public:
		FileException( const char * );
		~FileException( void ) throw();
		virtual const char* what() const throw();
};
