# CSCI 2510: Studio 6 - Parsing String Input

Parsing input in C is initially more difficult than in other, higher
level, languages. However, the C library includes a wide variety of
functions that make input handling straightforward after you\'ve spent a
little time in practice.

In this studio, you will:

1.  Get user input with `fgets()`
2.  Parse the input string with `strtok()`
3.  Pack input data into your own `char* argv[]` structure
4.  Execute programs with `execvp()`

------------------------------------------------------------------------

Please complete the required exercises below, as well as any optional
enrichment exercises that you wish to complete.

As you work through these exercises, please record your answers in a
text file. When finished, submit your work via the Git repository.


------------------------------------------------------------------------

### Required Exercises

1.  Write a short program that:

    1.  Prompts the user to enter input
    2.  Gets user input using the function `fgets()`
    3.  Prints the user input back to the console with
        `printf("%s\n", input_string)`

    See the man page for `fgets()` for more detail. You will need to
    declare a sufficiently large `char buffer[]` to store user input,
    and you should read from the standard input stream (\'`stdin`\').

    Copy and paste the output of your program once it runs correctly.

2.  Now we want to parse the input string and divide it into individual
    *tokens*. For this we can use the function `strtok()`, which parses
    a string from left to right and returns one token each time you call
    the function. Pull up the `strtok()` manual page and read the first
    paragraph. What difference is there between the initial call to
    `strtok()` and subsequent calls that parse the same string?

3.  The *delimiter* character tells the `strtok()` function which
    character marks the boundary of tokens. What delimiter character
    should you use for regular text input?

4.  Make a call to `strtok()` so that your program parses the first
    token from the user input. Print this first token on a new line.

    Copy and paste your program output once this works correctly.

5.  Now use an appropriate loop so that your program parses and prints
    each token in the user input on a separate line. Copy and paste your
    program output once this is done.

6.  One strange artifact you might notice is an extra newline that is
    printed after the last token. This is because your input string
    automatically contains a new line character at its end. For example,
    if you enter:

    `This is an input string.`

    The actual string recieved by your program is:

    `"This is an input string.\n"`

    We want to *strip* this trailing newline character, as it will cause
    problems for us later. There are a variety of approaches to doing
    this, such as using the `strlen()` or `strchr()` functions, or even
    the `strtok()` function itself. Modify your program to strip the
    newline character after your call to `fgets()`, but before you start
    parsing. Try a Google search for \"strip newline character in C\" if
    you would like some further inspiration.

    As the answer to this exercise, copy and paste your newline
    stripping code.

7.  Lastly, we want to re-purpose this studio program as a springboard
    to executing arbitrary programs for Lab 2. Rather than just
    tokenizing input for the fun of it, we will now modify the studio so
    that it interprets our input as a program to execute. For example,
    if the user inputs the string `"ls -l"` then your program should
    execute the command ` ls -l`. We will use the function `execvp()`.

    The `execvp()` function takes two arguments. First, a `char*` that
    contains the command we want to execute (e.g. `"ls"` from above).
    Second, a `char* argv[]` structure.

    Declare the following variables and leave this answer blank.

    `int max_args = 15;`\
    `int max_argv_size = max_args + 2; //one for argv[0], one for null terminator`\
    `char* cmd;`\
    `char* my_argv[max_argv_size];`

8.  Filling out the `cmd` variable is easy. This will always be the
    first token returned from `strtok()`. Note that `strtok()` returns a
    `char*` and this is exactly the type we have declared for `cmd`.

    Copy and paste your assignment statement as the answer to this
    exercise.

9. Filling out `my_argv` is slightly trickier. First, observe that
    `my_argv` is simply an array of `char*` pointers. Second, recall
    that the regular `argv` variable always has the name of the command
    as the first element (`argv[0]`), and is always terminated by a NULL
    character.

    Start by assigning \"`my_argv[0] = cmd;`\". Then, parse the rest of
    the user input string and assign each subsequent token to each
    subsequent position. For example, if the result of `strtok()` is
    stored as such:

    `res = strtok(NULL, " ");`

    Then assign each token thusly, for an appropriate value of `i`:

    `my_argv[i] = res;`

    IMPORTANT: After there are no more tokens to parse, assign the next
    element of `my_argv` the NULL character:

    `my_argv[after_last] = '\0';`

    Copy and paste your `my_argv[]` code as the answer.

10. Lastly, after you have constructed `cmd` and `my_argv`, insert the
    following call to `execvp()`:

    `execvp( cmd, my_argv );`

    If the two variables are constructed correctly, your program should
    read the user input, parse it as though it were a new program to
    execute, and then execute that program with `execvp()`

    Copy and paste a working output as the answer to this question.

### Optional Enrichment Exercises

1.  No optional exercises

------------------------------------------------------------------------
