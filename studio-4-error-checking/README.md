# CSCI 3500: Studio 4 - Error Checking

Any non-trivial program requires you to call functions provided by
either the operating system or system libraries. These functions almost
always provide a return value, indicating whether an operation succeeded
or failed. Checking these return values are vital to producing robust
code, and greatly simplify debugging.

*Checking return values appropriately will be a requirement for lab 2
and all subsequent labs!*

In this studio, you will:

1.  Look up return values in a function\'s man page
2.  Check return values and handle errors appropriately
3.  Print appropriate error messages with `perror()`

------------------------------------------------------------------------

Please complete the required exercises below, as well as any optional
enrichment exercises that you wish to complete.

As you work through these exercises, please record your answers in a
text file. When finished, submit your work via the Git repository.

------------------------------------------------------------------------

### Required Exercises

1.  Create a new file called `open.c`. Write a short program that
    accepts a single command line argument. This argument should be
    interpreted as a file name that your program attempts to open. Once
    the file is opened, your program should print the contents of the
    file to standard output. For example, the command
    \"`./open mars.txt`\" should open the file `mars.txt` and print it
    to the console. Use the `open()` function documented at
    `man 2 open`.

    *Hint: start with your program from studio 2, called `copy.c`.
    Modify this program to open a file, and then modify the `read()`
    statement to read from your file descriptor rather than standard
    input. Alternately, start with your program from Lab 1.*

    As the answer to this exercise, copy and paste your working code.

2.  What happens when your argument specifies a file that doesn\'t
    exist?

3.  Create a text file called `perm.txt` and fill it with some short
    message. Execute the command \"`chmod -rw perm.txt`\". This command
    removes read and write permissions from the file. Now try to execute
    your program on this file- what happens?

4.  Look at the man page for `open()`. There is a
    section called \"RETURN VALUE\". What value does `open()` return in
    the event of an error?

5.  If you haven\'t already, insert an `if` statement directly after
    your call to `open()` to check for errors. Your error detection
    should check the criteria in the man page exactly. For example, if
    the reported error value for `open()` is -1, then check with the
    conditional \"`return_value == -1`\" rather than something like
    \"`return_value < 0`\". As the answer to this exercise, copy and
    paste the conditional you check.

6.  The function `open()` also sets a special variable called `errno`.
    Many system calls and library functions will set this variable when
    they execute. If an error occurs, this variable tells the system
    what happened. You can print a helpful error message with the
    function `perror()`.

    Insert a call to `perror()` inside your if statement from the last
    exercise. The only argument to this function is a short message that
    should describe the circumstances of the call. For example,
    `"Error opening file"` or `"Error reading input"`.

    As the answer to this exercise, copy and paste your call to
    `perror()`.

7.  Finally, your program should exit gracefully when an error occurs.
    Insert a call to `return` or `exit()` with a negative value inside
    your `if` statement, but after `perror()`. Leave this answer blank.

8.  Now, what happens when the argument to your program specifies a file
    that doesn\'t exist?

9. What happens when the argument specifies the file without read or
    write permissions- `perm.txt`?

10. Look at the following manual pages and determine what value(s) are
    returned in the event of an error, and whether these functions set
    `errno`:
    -   man 2 open
    -   man 2 close
    -   man 2 read
    -   man 2 write
    -   man 3 printf

11. Give two reasons why error checking in this manner simplifies
    software development.

### Optional Enrichment Exercises

1.  The special variable `errno` is documented at `man errno`. There are
    a huge number of possible error statuses. Some of these are generic
    (e.g. insufficient permissions), while others are very specific
    (e.g. network host is unreachable).

    Having detailed error reporting presents the possibility that your
    programs can detect an error and attempt to self-correct, rather
    than quitting or crashing. For example, a common error status is
    `EAGAIN` or `EBUSY`, both of which indicate that an OS resource is
    currently unavailable. Rather than quitting, your program could
    simply wait and try again later.

    Browse through `man errno` and think about how you might handle some
    of the errors that can arise.

------------------------------------------------------------------------
