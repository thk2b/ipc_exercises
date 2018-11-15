#include "lib.h"
#include <printf.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

static int	exec_fork_child(char *path, char **args)
{
	extern char	**environ;

	execve(path, args, environ);
	return (error("execve"));
}

static int	exec_fork_parent(pid_t child_pid)
{
	int	status;

	status = 0;
	signal(SIGINT, SIG_IGN);
	if (waitpid(child_pid, &status, 0) == -1)
		return (error("wait_pid"));
	signal(SIGINT, SIG_DFL);
	return (status);
}

static int	exec_fork(char *path, char **args)
{
	pid_t		pid;

	if (validate_executable(path))
		return (1);
	pid = fork();
	if (pid == -1)
		error("fork");
	if (pid == 0)
		_exit(exec_fork_child(path, args));
	return (exec_fork_parent(pid));
}

int			main(int ac, char **av)
{
	if (ac < 2)
		return (usage(av[0], "<executable> [ <arg> [...]]", 1));
	return (exec_fork(av[1], av + 1));
}
