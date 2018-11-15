#include "lib.h"
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#define USAGE "<exec> [<arg> [...]] | <exec> [<arg> [...]]"

// static int	exec_pipe_cmd(int fds[], char **cmd)
// {

// }

static int	exec_pipe_child(int writes, int fd[], char **cmd)
{
	extern char	**environ;

	if (writes)	/* map child process's stdout to the the pipe's write end */
	{
		if (dup2(fd[1], 1) == -1)
			return (error("exec pipe write child"));
		if (close(fd[0]))
			return (error("close"));
	}
	else		/* map child process's stdin to the the pipe's read end */
	{
		if (dup2(fd[0], 0) == -1)
			return (error("exec pipe read child"));
		if (close(fd[1]))
			return (error("close"));
	}
	execve(cmd[0], cmd, environ);
	return (error("execve"));
}

static int	exec_pipe(char **cmd1, char **cmd2)
{
	int		fd[2];
	int		status[2];
	pid_t	pid[2];

	if (pipe(fd))
		return (error("pipe"));
	pid[0] = fork();
	if (pid[0] == -1)
		return (error("fork cmd1"));
	if (pid[0] == 0)
		_exit(exec_pipe_child(1, fd, cmd1));
	pid[1] = fork();
	if (pid[1] == -1)
		return (error("fork cmd2"));
	if (pid[1] == 0)
		_exit(exec_pipe_child(0, fd, cmd2));
	if (close(fd[0]) || close(fd[1]))
		return (error("close pipe from parent"));
	if (waitpid(-1, status, 0) == -1)
		return (error("waitpid 1"));
	if (waitpid(-1, status + 1, 0) == -1)
		return (error("waitpid 2"));
	return (status[0] || status[1]);
}

static int	get_cmds(char **av, char ***c1, char ***c2)
{
	int	i;

	i = 0;
	while (av[i])
	{
		if (strcmp(av[i], "|") == 0)
		{
			av[i] = NULL;
			i++;
			break ;
		}
		i++;
	}
	if (av[i] == NULL)
		return (1);
	*c1 = av;
	*c2 = av + i;
	return (0);
}

int			main(int ac, char **av)
{
	char	**cmd1;
	char	**cmd2;

	cmd1 = NULL;
	cmd2 = NULL;
	if (ac < 2)
		return (usage(av[0], USAGE, 0));
	if (get_cmds(av + 1, &cmd1, &cmd2))
		return (usage(av[0], USAGE, 1));
	return (exec_pipe(cmd1, cmd2));
}
