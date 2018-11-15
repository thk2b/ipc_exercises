#include "lib.h"
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#define USAGE "<exec> [<arg> [...]] | <exec> [<arg> [...]]"

static int	select_pipe_end(int selected, int fd[])
{
	if (selected > 1 || selected < 0)
		return (-1);
	if (dup2(fd[selected], selected) == -1)
		return (1);
	if (close(fd[!selected]))
		return (1);
	return (0);
}

static int	exec_pipe_cmd(int selected, int fd[], char **cmd)
{
	pid_t	pid;
	extern char	**environ;

	pid = fork();
	if (pid == -1)
		return (error_ret("fork", 0));
	if (pid == 0)
	{
		if (select_pipe_end(selected, fd))
			return (error("select pipe end"));
		_exit(execve(cmd[0], cmd, environ));
	}
	return (pid);
}

static int	exec_pipe(char **cmd1, char **cmd2)
{
	int		fd[2];
	int		status[2];
	pid_t	pid[2];

	if (validate_executable(cmd1[0]) || validate_executable(cmd2[0]))
		return (1);
	if (pipe(fd))
		return (error("pipe"));
	if ((pid[0] = exec_pipe_cmd(1, fd, cmd1)) == 0)
		return (1);
	if ((pid[1] = exec_pipe_cmd(0, fd, cmd2)) == 0)
		return (1);
	if (close(fd[0]) || close(fd[1]))
		return (error("close pipe from parent"));
	signal(SIGINT, SIG_IGN);
	if (waitpid(pid[0], status, WUNTRACED) == -1)
		return (error("waitpid 1"));
	if (waitpid(pid[1], status + 1, WUNTRACED) == -1)
		return (error("waitpid 2"));
	signal(SIGINT, SIG_DFL);
	printf("[%d]: %d | [%d]: %d\n", pid[0], status[0], pid[1], status[1]);
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
