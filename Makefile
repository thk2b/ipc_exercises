CC = gcc
FLAGS = -Wall -Wextra -Werror -g

all: exec_cmd exec_pipe

lib:
	$(CC) $(FLAGS) lib.c -c

exec_cmd: lib
	$(CC) $(FLAGS) lib.o exec_cmd.c -o exec_cmd

exec_pipe: lib
	$(CC) $(FLAGS) lib.o exec_pipe.c -o exec_pipe

clean:
	rm -f lib.o

fclean: clean
	rm -f exec_cmd exec_pipe
