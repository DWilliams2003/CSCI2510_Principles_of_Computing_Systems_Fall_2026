# CSCI 2510: Studio 7 - Pipes

Pipes are a basic but fundamental method of inter-process communication.
They allow processes to communicate with each other as though they were
reading and writing to a shared file, when in fact they\'re accessing a
special mechanism provided by the operating system. Along with the
system call `dup2()` they permit the seamless redirection of program
input and output.

In this studio, you will:

1.  Create a pipe using the `pipe()` system call
2.  Redirect file streams using the `dup2()` system call
3.  Connect unrelated child processes through the use of pipes

------------------------------------------------------------------------

Please complete the required exercises below, as well as any optional
enrichment exercises that you wish to complete.

As you work through these exercises, please record your answers in a
text file. When finished, submit your work via the Git repository.

------------------------------------------------------------------------

### Required Exercises

1.  The **real** power of pipes lies in their ability to connect
    unrelated processes, *even if the original programmers didn\'t
    intend that*! In order to explore pipes today, we will need two
    simple programs.

    Program 1: Write a short program that:

    1.  Prints a string to standard output and then returns.

    *Hint! This was the goal of Studio 1!*

    Program 2: Write a short program that:

    1.  Inside an infinite loop:
    2.  Gets an input string from standard input using the function
        `fgets()`
    3.  If the return value of `fgets()` is NULL, then break
    4.  Prints the user input back to the console with
        `printf("Program 2 got: %s\n", input)`

    *Hint! This was also the starting point for Studio 6! (minus the
    infinte loop)*

    When you execute program 1, it should print a string and return.
    When you execute program 2, it should repeatedly receive user input
    and print the user input back to the console until you enter the end
    of file character (CTRL-D). Once finished, copy and paste these
    programs as the answer to this question.

2.  Now, create a new program called `pipe.c`. This program should
    `fork()` and `execvp()` two children. The first child should
    `execvp()` your Program 1, and the second child should `execvp()`
    your Program 2. Then the parent program should wait on both children
    with `waitpid()`.

    *Hint:* suppose your Program 1 is called `print`, the code to call
    `execvp()` correctly follows. Make sure you understand each part!

    `char* cmd = "./print"; char* myargv[] = {"print", '\0'}; int ret = execvp( cmd, myargv ); if( ret == -1 ) perror("Error exec'ing print");`

    If your code is working correctly, then your program `pipe.c` will
    never return. This is because the input program, Program 2, cannot
    recieve any input and will never terminate. You can kill these
    programs with CTRL-C. We will fix this problem eventually.

    Copy and paste your program output as the answer to this exercise.

3.  Now we will create a pipe to connect the two child processes. In the
    parent, before the first call to `fork()`, make a call to `pipe()`.
    This function accepts a single argument of type `int fd[2]`.

    Look at the manual page at `man 2 pipe`. Which element of `fd[2]`
    contains the read end of the pipe after `pipe()` successfully
    returns? Which elemnt contains the write end?

4.  Our goal now is to write the output of Program 1 into the input of
    Program 2. However, we don\'t want to have to modify Program 1 or
    Program 2. The solution is to *redirect* the `stdout` stream of
    Program 1 into the write end of the pipe, and then redirect the read
    end of the pipe into the `stdin` stream of Program 2. To do this we
    will use the system call `dup2()`.

    Open the manual page for this function with `man 2 dup2`.
    Unfortunately it can be confusing. Let\'s rephrase our objective in
    the language of this manual page. We want to\...

    1.  *close `STDOUT_FILENO` in Program 1, replacing it with the write
        end of the pipe*
    2.  *close `STDIN_FILENO` in Program 2, replacing it with the read
        end of the pipe*

    The function `dup2()` takes two arguments. Which argument is
    *closed* when the function succeeds?

5.  Now use the `dup2()` function in the manner described in the
    previous exercise. Insert the call to `dup2()` after forking the
    first child, but before executing your Program 1. Replace the
    `STDOUT_FILENO` descriptor with the write end of the pipe.
    Similarly, replace `STDIN_FILENO` with the read end of the pipe in
    your second child. Copy and paste your `dup2()` code.
6.  Now execute your `pipe.c` program. The output of your Program 1
    should be read in by your Program 2, and then re-printed with the
    Program 2 preface. For example, your output might read:

    `Program 2 got: Program 1 output line`

    Note that your program still shouldn\'t terminate. This is because
    your parent program (`pipe.c`) is `waitpid()`-ing on the second
    child, and the second child still does not terminate. Use the CTRL-C
    shortcut to kill your processes.

    Copy and paste your program output as the answer to this exercise.

7.  The reason your Program 2 does not terminate is because it will
    continually check the read end of your pipe for more data as long as
    the write end of the pipe is still open. Once all of the write ends
    of the pipe have been closed the pipe will deliver an end-of-file
    notification, letting the reader(s) know that no further data will
    be coming.

    Use the `close()` system call to tidy up your access to the pipe.
    Remember that three processes now have access: the parent and the
    two children. Your parent is not reading or writing the pipe, so it
    should close both the read and write ends. Your Program 1 child does
    not need to read from the pipe, so it should close the read end.
    Your Program 2 child does not need to write to the pipe, so it
    should close the write end.

    Once the above steps are complete, the `pipe.c` should execute and
    then terminate correctly. Copy and paste your output.

8.  If you think about it, you just achieved a pretty impressive feat.
    You just shared data between two programs who had no idea that each
    other existed. In fact, your programs have no idea where their data
    comes from, and no idea where it goes. They simply read and write to
    file streams, and the operating system can redirect these file
    streams at will. For all they know they could be acting on an actual
    file, a virtual file, a network socket, or any other suitable I/O
    channel, and the operating system seamlessly does all the heavy
    lifting in the background. Give two advantages to this stream
    ambiguity.

### Optional Enrichment Exercises

1.  No optional exercises

------------------------------------------------------------------------
