#include "lib.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>

int		usage(char *name, char *str, int ret)
{
	printf("%s: %s\n", name, str);
	return (ret);
}

int		error(char *msg)
{
	if (msg)
		printf("%s: %s\n", msg, strerror(errno));
	else
		printf("%s\n", strerror(errno));
	return (errno);
}

int		validate_executable(char *path)
{
	if (access(path, X_OK))
		return (error(path));
	return (0);
}
