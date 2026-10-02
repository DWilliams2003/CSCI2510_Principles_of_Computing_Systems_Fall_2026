# CSCI 2510: Studio - Hello, world!

*\"It\'s a dangerous business, Frodo, going out of your door. You step
into the road, and if you don\'t keep your feet, there is no knowing
where you might be swept off to.\"*

-- Bilbo, *The Fellowship of the Ring*, Book 1, Chapter 3

Let\'s get started with two simple \"Hello, world!\" programs that
demonstrate the difference between doing standard output with the C
standard library versus Linux system calls.

In this studio, you will:

1.  Use the built-in Linux manual (man pages) to look up certain
    functions
2.  Write a \"Hello, world!\" program using the C standard library
3.  Write a \"Hello, world!\" program using Linux system calls
4.  Write a \"Hello, world!\" program using Linux shell 

------------------------------------------------------------------------

Please complete the required exercises below, as well as any optional
enrichment exercises that you wish to complete.

As you work through these exercises, please record your answers in a
text file. When finished, submit your code and answers file to this git
respository.

------------------------------------------------------------------------

### Required Exercises

1.  First we need a computing environment, and I will assume you are using
    your own laptop as our default environment.

    If you want to use the department Linux resources, then you can
    login locally to a Linux
    machine in the classroom or lab, or you can remotely connect to the
    department server at *hopper.slu.edu*. In general you will only ever need
    Linux terminal access for these exercises, though you\'re welcome to use a
    GUI desktop.

2.  Your first task is to write a \"Hello, world!\" program using the
    function `fprintf()` in the C standard library. To do so, pull up
    the function\'s documentation by typing \"`man fprintf`\" in the
    Linux terminal. This page tells you everything you need to know in
    order to use `fprintf()` in a program.
    (If you are not using the Linux environment, you can also type
    "man fprintf" into a search engine and you will find the same
    materials posted online.) As the answer to this
    exercise, give the `#include` header file(s) needed for this
    function (look right beneath the \"SYNOPSIS\" heading).

3.  Now look at the function signatures for both `fprintf()` and the
    `printf()`. What is the difference? (Signature is a name for the
    argument list.)

4.  Look at the first paragraph under the \"DESCRIPTION\" heading. What
    is the described difference between the `printf()` and `fprintf()`
    functions?

5.  Let\'s put our newfound knowledge to use! Quit out of the man page
    by pressing \"q\" and create a text file called
    \"hello\_fprintf.c\". Finish the following empty program so that it
    prints a message of your choice.

    `// Your name
     // The date
     // A short description of your program
     // Include file goes here

     int main( int argc, char* argv[] ){
       //Make a call to fprintf() here
       return 0;
     }`

    The first argument of `fprintf()` must specify an I/O stream. In
    this case you should use \"`stdout`\". This is a special stream that
    prints to the Linux console, but this function works for a variety
    of stream-oriented operations, such as writing to a file or sending
    data over a network. The second argument should be a string literal
    that gives your message, and should be terminated with \"`\n`\" (the
    newline character). For example, your string literal might look
    like:

    `"Hello, world!\n"`

    You can compile your program with the command:\
    `gcc hello_fprintf.c -o hello_fprintf`

    Run your program by typing \"`./hello_fprintf`\". If your program
    runs correctly then copy and paste your terminal output as the
    answer to this exercise.

7.  Great! Now we want to accomplish the same thing but using a system
    call directly, as opposed to the C library. The particular system
    call we want to use is called `write()`. However, if you give the
    command \"`man write`\" you won\'t arrive at the correct
    documentation because there are multiple pages that might apply to
    such a common term. Instead, give the command \"`man man`\" to pull
    up the documentation for the manual system.

    As the answer to this exercise, give the *section number* for system
    calls.

8.  Now, use the answer to the last exercise to look up the
    documentation for the `write()` system call. The syntax you should
    use is:

    `man <section number> write`

    For example, the command \"`man 3 fprintf`\" explicitly states to look
    in the standard library section of the manual. As it so happens, this
    command takes us to the documentation page we saw previously.

    As the answer to this
    exercise, give the header file which must be included to use the
    `write()` system call (again, look beneath the \"SYNOPSIS\"
    statement).

9.  Make a copy of your program called `hello_write.c`. The Linux
    terminal command to copy a file is `cp`. In this case, you should
    execute the command:

    `cp hello_fprintf.c hello_write.c`

    Leave the answer for this exercise blank.

11. The `fprintf()` function doesn\'t actually implement the code that
    allows your program to print to the terminal, it relies on the
    operating system to do that. The OS provides a ***system call***
    called `write()` that gives this functionality, and `fprintf()`
    calls this function on your behalf. However, you can call `write()`
    directly, which is what we will do now.

    Modify your program to use `write()` instead of `fprintf()`. There
    are two differences between the functions. The first is that you
    need to use a *file descriptor* instead of a *file stream*. In other
    words, replace the built-in variable \"`stdout`\" with the built-in
    variable \"`STDOUT_FILENO`\". Both variables refer to the same
    \"thing\" but in different ways.

    The second difference is that `write()` is a byte-oriented function
    rather than a string-oriented function. The `fprintf()` function was
    able to figure out how many characters to write on its own, but you
    will need to tell `write()` how many characters to output. 
    When counting how many characters is in your buffer, don\'t
    forget the newline character `'\n'`. Note that the whole newline
    character escape sequence (`'\n'`) counts as one character. For
    example, the string "dog\n" has four characters.
    
    Once you are finished, copy and paste your new program as the answer to
    this exercise.

10. Try changing the third argument of `write()` to be much larger than
    your character buffer (say, 1000 or 10000). What do you think happens
    when you do this? Try running the code- does anything usual show up?
    If so, Why do you think this is?

11. The function `fprintf()` is provided by the *C standard library* and
    is guaranteed to exist for any standards-compliant C language
    implementation- even on other operating systems. The `write()`
    system call is not guaranteed to exist as it is provided by the
    operating system itself. Many system calls (but not all) have C
    library versions.

    Thinking as a software developer, speculate a situation when you
    would want to use a C library function and another situation when
    you would want to use an OS system call.

12. Now, create a hello.sh to print \"Hello World!\". 

### Optional Enrichment Exercises

1.  No optional exercises

------------------------------------------------------------------------
