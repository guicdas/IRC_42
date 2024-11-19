#pragma once

#include <stdlib.h>
#include <sys/resource.h>	//getrlimit
#include <iostream>
#ifndef BUF_SIZE
# define BUF_SIZE 4096
#endif

class Server;
typedef struct s_env t_env;

typedef struct	s_fd
{
	int		type;
	void	(Server::*clientRead)( t_env * );
	void	(Server::*clientWrite)( t_env * );
	char	readBuf[BUF_SIZE + 1];
	char	writeBuf[BUF_SIZE + 1];
} t_fd;

int		initFds( t_env * );
void	clean_fd( t_fd * );