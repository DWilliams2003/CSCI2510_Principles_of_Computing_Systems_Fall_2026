# CSCI 2510: Studio 5 - Fork and Exec

Executing processes is fundamentally the reason why operating systems
exist. Beyond that, it\'s an extremely common activity in modern
systems- every time you look at a file, compile a program, or execute a
terminal command you\'re probably creating at least one process, if not
more. Understanding the `fork()` and `exec()` mechanisms provided by the
operating system will give you insight to the process lifecycle in
Linux.

In this studio, you will:

1.  Get process IDs with `getpid()`
2.  Create new processes with `fork()`
3.  Wait for child processes with `wait()`
4.  Execute programs with `exec()`

------------------------------------------------------------------------

Please complete the required exercises below, as well as any optional
enrichment exercises that you wish to complete.

As you work through these exercises, please record your answers in a
text file. When finished, submit your work via the Git repository.

------------------------------------------------------------------------

### Required Exercises

1.  Every process in Linux (and other OSes) has a unique identifying
    number called a *Process Identifier* (or PID). This number is used
    whenever you need to programmatically reference or interact with a
    specific process in the system.

    Create a new program called `fork.c`. Use `printf()` and `getpid()`
    to print out the process\' PID. Run the program three times and
    copy-paste the output as the answer to this exercise.

2.  Now we will create a new process. Recall from lecture that this
    involves two different operations: *`fork()`-ing* and
    *`exec()`-ing*. The `fork()` step creates a new process (the
    \"container\" for executing a program), and the `exec()` step loads
    and executes a desired program within that new process.

    When `fork()` is called, the system creates a near-identical replica
    of the calling process. After the fork you will have two processes
    that both execute the same program. Insert the call \"`fork();`\"
    immediately after the start of `main()` in your program `fork.c`,
    and before the lines where you get the program PID and the print it
    out.

    What happens when you run your program now? Copy and paste the
    output.

3.  Look up the man page for `fork()`. This is a unique function in that
    you get *two* return values for each valid call to `fork()` (one in
    the parent, and another in the child). What are the return values
    listed?

4.  Use `fork()`\'s return values to modify your program so that the
    parent and child processes identify themselves as such when they
    print out their PIDs. Copy and paste your program output.

5.  Sometimes it is useful for a parent process to *wait* for one or
    more child processes to finish executing. Otherwise, you might get
    undesirable behavior, or behavior that doesn\'t make sense. The
    `sleep()` function causes a program to suspend for a specified
    length of time before continuting, and we can use it to pretend that
    the child process takes a long time to execute. Modify your program
    so the child process executes the following call before it prints
    out its PID:

    `sleep(3);`

    This will cause your child process to go to sleep for about three
    seconds before printing its output. Now run your program\... what
    happens? Does it behave the way you would expect?

6.  Because the child process takes a while to execute, we\'re in a
    situation where it might make sense for the parent process to
    explicitly wait for the child to finish. The `waitpid()` function
    will halt the parent until a desired child process returns. Modify
    your program so the parent process executes the following call
    before it prints out its PID:

    `waitpid( child_PID, NULL, 0 );`

    (The second and third parameters allow us some extra control over
    how this function operates, but we don\'t need them for now.)

    Execute your program again. How is the behavior different?

7.  Lastly, we can use the exec family of functions so that the child
    process can load and execute a new program on behalf of the parent.
    In particular we will use the `execvp()` function. (There are many
    forms of `exec()`, but this one is arguably the easiest to get
    started with, see the rest at `man 3 exec`.)

    Before we use `execvp()`, you should execute the command we want to
    run so that you know what to expect. We\'re going to have our child
    process execute the `ls` program in list-mode. Execute the command
    \"`ls -l`\" and copy-paste the output as the answer to this
    question.

8.  Now we will create a new, *static*, argv variable for the child.
    This is only useful when you know exactly what you\'re going to
    execute when you\'re writing your program, but we will explore more
    flexible methods in future studios. Create the following variables:

    `char* child_argv[] = {"ls", "-l", NULL};`\
    `char* child_prog = child_argv[0];`

    Notice that `child_argv` has the exact same layout as if you were to
    try and access `argv` in a regular program. Now, in the child
    process, insert the call:

    `execvp( child_prog, child_argv );`

    If everything is correct, you should see your child process execute
    the \"`ls -l`\" command, and then your parent process should wait
    and print out its PID. Copy and paste your program output as the
    answer to this exercise.

### Optional Enrichment Exercises

1.  Rearrange your call to the `execvp()` function so that you can call
    a different program with different arguments.

2.  Rearrange your `execvp()` call so that you can execute an arbitrary
    program, for example as a result of user input. This is a vital part
    of Lab 2, and we will explore it further in a future studio.

------------------------------------------------------------------------
