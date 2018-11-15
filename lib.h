#ifndef LIB_H
# define LIB_H
# include <printf.h>

int		error(char *msg);
int		error_ret(char *msg, int ret);
int		validate_executable(char *path);
int		usage(char *name, char *str, int ret);

#endif
