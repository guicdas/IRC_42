#include "../includes/fd.hpp"
#include "../includes/irc.hpp"

void	clean_fd( t_fd *fd )
{
	fd->type = 0;
	fd->clientRead = NULL;
	fd->clientWrite = NULL;
}

int	initFds(t_env *e)
{
	int	i = 0;

	struct rlimit	rlp;
	if (getrlimit(RLIMIT_NOFILE, &rlp) == -1)
		return (0);
	e->maxFd = rlp.rlim_cur;
	e->fds = (t_fd *) malloc(sizeof(*e->fds) * e->maxFd);
	if (e->fds == 0)
		return (0);
	while (i < e->maxFd)
	{
		clean_fd(&e->fds[i]);
		i++;
	}
	return (1);
}