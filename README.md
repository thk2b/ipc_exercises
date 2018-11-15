# ipc_exercises
Simple programs to demonstrate inter process communication

## compilation
run `make`

## exec_cmd

usage: `"<executable> [ <arg> [...]]"`
Executes the executable with arguments.

## exec_redirection

usage: `"<exec> [<arg> [...]] <filename>"`
Executes the executable with arguments and redirects the child's output to the file.

## exec_pipe

usage: `"<exec> [<arg> [...]] | <exec> [<arg> [...]]"`
Executes both executables with arguments, redirecting the first's `stdout` to the second's `stdin`.
