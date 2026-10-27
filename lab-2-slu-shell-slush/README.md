# CSCI 2510: Lab - SLUSH - the SLU Shell 

Processes are the fundamental abstraction in any operating system-
a main purpose of the operating system is to "operate" the computer,
which means simplifying execution of
computer programs. A central part of this is the program that
allows a user to select and execute other programs: on Unix-based OSes
this is the *terminal* or *shell*. The shell is not part of the
operating system kernel, but it is a natural extension of it and in many
ways represents the operating system itself!

We never want to take programs like `bash` or `sh` for granted, so
we\'ll go through the work of building or own shell from scratch.

In this lab, you will:

1.  Spawn new processes with the `fork()` and `execvp()` system calls
2.  Wait for child processes with the `wait()` system call
3.  Perform complex input parsing in C
4.  Pipe data between processes
5.  Perform basic signal handling

------------------------------------------------------------------------

### Description

`slush` - SLU shell

`slush` is a very simple command-line interpreter. It uses a different
syntax than shells like `bash`, and has much less functionality. `slush`
executes in a loop in which it displays a prompt, reads in a command
line from standard input, and executes the command.

There are two types of commands: built in commands which are executed by
`slush` itself, and program execution commands which are carried out by
separate processes.

A built in command must appear on a line by itself. The only built in
command is:

`cd dir` - change current directory to `dir`

Program execution commands have the form:

`prog_n [args] ( ... prog_3 [args] ( prog_2 [args] ( prog_1 [args]`

This command runs the programs `prog_n`, \... , `prog_2`, `prog_1` (each
of which may have zero or more arguments) as separate processes in a
\"pipeline\". This means the standard output of each process is
connected to the standard input of the next.

The syntax of `slush` is backwards from shells you\'re used to, and is
intended to emphasize the functional nature of pipeline commands. As an
example, the command line:

`more ( sort ( ps aux `

should produce a paginated, sorted list of processes.

`slush` should catch \^C typed from the keyboard. If a command is
running, this should interrupt the command. If the user is entering a
line of input, `slush` should respond with a new prompt and a clean
input line.

`slush` will exit when it reads an end-of-file on input.

### Error Handling

If a program name does not exist or is not executable, `slush` should
print an error message of the form:

`prog1: Not found`

(To do this automatically, you can use `perror()` after the `exec()`
fails).

Syntax errors such as:

`more ( ( ps au`

`more ( ps au (`

should be handled with an appropriate message, such as:

`Invalid null command`

### Hints

1.  Work on parsing before you work on functionality. First write your
    program so that it reads input and generates correct tokens.
2.  Exiting on end-of-file (EOF) is VERY IMPORTANT because without it I
    cannot test your program. This is the only way to exit `slush`. You
    can create a EOF by typing \^D at the keyboard, or by running your
    program with noninteractive input. For example, `echo ls | slush`
    should run `slush`, execute the `ls` command, and quit `slush`. I
    suggest you read input using `fgets()`- the return value is
    false (0) on EOF.
3.  To break the user input into tokens, you could use the the
    `strtok()` function. For simplicity, you may restrict each command
    in a pipeline to at most 15 arguments. You may also limit the length
    of an input line to 256 characters. Violations of these limits may
    result in an error or simply be ignored, so long as `slush` does not
    crash.
4.  Turning the command into a pipeline of running processes is much
    easier if you write a recursive function. Start at the left end of
    the command string, and work right recursively. Processes should
    `fork()` off right to left as you return from the series of
    recursive calls.
5.  When a process has a pipe, it needs to `close()` the file descriptor
    for the end of the pipe it\'s not going to use (or else it will
    never terminate). Do this after each `fork()`. Both parent and child
    may need to close something. Count your children\... you\'ll need to
    `wait()` for them later.
6.  Save the \^C handling for last. \^C generates a `SIGINT` signal. You
    need to set up a signal handler to catch the signal. What happens if
    a signal is caught while reading a line of input? How about if a
    signal is caught while `wait()` is waiting? One more hint for \^C
    handling: typing \^\| will send a `SIGQUIT` and stop your process if
    you\'ve screwed up \^C.

### Documentation

The following man pages will be useful:

-   `execvp` (2)
-   `fork` (2)
-   `wait` (2)
-   `strtok` (3)
-   `strcmp` (3)
-   `chdir` (2)
-   `close` (2)
-   `dup2` (2)
-   `pipe` (2)
-   `signal` (2)
-   `sigaction` (2)

### Questions

1.  Describe any known bugs or ways that your submission deviates from
    the above specification.
2.  Indicate which, if any, extra credit exercises you have attempted.

### Optional Enrichment Exercises

-   **5% Extra Credit**: It\'s useful to know where you are in the
    directory tree. Modify your `slush` prompt to print the current
    directory. Don\'t print the entire directory path- just print
    everything past the user\'s home directory. For example, if I login
    and issue the command \'`cd dir1/dir2`\', then the whole directory
    path is actually:

    `/export/mathcs/home/faculty/dferry/dir1/dir2`

    That\'s much too long- I really just want to know the directory path
    within my home directory. That is, I want my shell prompt to look
    something like:

    `slush|dir1/dir2>`

    The manual pages `getcwd(3)` and `getenv(3)` might be helpful here,
    as well as the knowledge that there is an environment variable
    called `HOME` that stores the current user\'s home directory.

------------------------------------------------------------------------

### Submission

Your submission must include a makefile that will compile your program by
simply issuing the command `make`. You must also include a text file
with your answers to the required exercises.
