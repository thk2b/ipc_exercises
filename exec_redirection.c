#include "lib.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#define USAGE "<exec> [<arg> [...]] <filename>"
#define CREAT_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IRUSR)

static int	exec_child_redirect(int fd, char **cmd)
{
	extern char	**environ;

	if (dup2(fd, 1) == -1)
		return (error("dup2"));
	execve(cmd[0], cmd, environ);
	return (error("execve"));
}

static int	exec_redirect(char **cmd, char *filename)
{
	int		fd;
	int		status;
	pid_t	pid;

	if ((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, CREAT_PERMS)) == -1)
		return (error("open"));
	pid = fork();
	if (pid == -1)
		return (error("fork"));
	if (pid == 0)
		_exit(exec_child_redirect(fd, cmd));
	close(fd);
	status = 0;
	signal(SIGINT, SIG_IGN);
	if (waitpid(pid, &status, WUNTRACED) == -1)
		return (error("waitpid"));
	signal(SIGINT, SIG_DFL);
	return (status);
}

int			main(int ac, char **av)
{
	char	*filename;

	if (ac < 3)
		return (usage(av[0], USAGE, 0));
	filename = strdup(av[ac - 1]);
	av[ac - 1] = NULL;
	return (exec_redirect(av + 1, filename));
}
