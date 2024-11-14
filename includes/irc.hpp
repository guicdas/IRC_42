#pragma once

#include <iostream>
#include <exception>
#include <cstring>
#include <stdlib.h>
#include <sys/socket.h>		//socket
#include <arpa/inet.h>		//inet_ntoa
#include <netinet/in.h>		//sockaddr_in
#include <unistd.h>			//close
#include <cerrno>			//errno
#include <csignal>  		//signal
#include <sys/fcntl.h>
# include <sys/select.h>	//select
#include <sys/resource.h>	//getrlimit
#include <netdb.h>			//getproto

#define _XOPEN_SOURCE_EXTENDED 1	//Special behavior for C++: you must use the _XOPEN_SOURCE_EXTENDED 1 feature test macro.
# define BUF_SIZE	4096


class Server{
	private:
		struct sockaddr_in	serverAddr;
		struct sockaddr_in	clientAddr;
		int					serverSocket;
		int					clientSocket;
		std::string			password;
		unsigned int		port;
		fd_set				fd_write;
		fd_set				fd_read;
		t_fd				*fds;
		int					maxfd;
		static bool			signal;

	public:
		Server( void );
		Server( char ** );
		Server( Server const & );
		Server	&operator=( Server const & );
		~Server( void );

	void			createServerSocket( void );
	void			loop( void );

	static void		signalHandler( int );
};

class FileException : public std::exception{
	private:
		std::string msg;
		std::string errno_s;
	
	public:
		FileException( const char * );
		~FileException( void ) throw();
		virtual const char* what() const throw();
};


typedef struct	s_fd
{
  int	type;
  void	(*fct_read)();
  void	(*fct_write)();
  char	buf_read[BUF_SIZE + 1];
  char	buf_write[BUF_SIZE + 1];
}		t_fd;
/*
getsockname, getprotobyname, gethostbyname, getaddrinfo, freeaddrinfo, 
connect, send, lseek, fstat,*/