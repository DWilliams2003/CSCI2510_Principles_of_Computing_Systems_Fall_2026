# CSCI 3500: Studio 3 - Pointers

One tricky part of writing good C code is using pointers correctly.
Understanding how pointers work, how to use them to index arrays, how
they relate to strings, and how to reference and dereference data
correctly is vital.

In this studio, you will:

1.  Learn about pointers and strings in C
2.  Use pointers to index into an array
3.  Write some basic string manipulation functions

------------------------------------------------------------------------

Please complete the required exercises below, as well as any optional
enrichment exercises that you wish to complete.

As you work through these exercises, please record your answers in a
text file. When finished, submit your work via Canvas. 

------------------------------------------------------------------------

### Required Exercises

1.  Let\'s start by building a minimal program. Look at the programs
    from the previous studios, and write out a main function. Inside
    your function you should only have the statement \"`return 0;`\"
    This is a valid C program, though it doesn\'t do much. When it runs
    it will immediately quit and return zero to the terminal.

    (You don\'t see the zero, but any command you run through the
    terminal returns some value and you can get that value if you want.
    By convention in Linux/Unix a return value of zero indicates that
    the program ran successfully, while any non-zero value indicates an
    error. These return values are especially useful for scripting a set
    of commands in sequence, as they tell you whether or not each step
    was successful and allow you to react appropriately. Typing the
    command \"`echo $?`\" will show the return value of the last command
    executed. You can try this now if you want- remember the program
    \"`cat`\" prints a file to the terminal, so if you use `cat` with a
    real file that exists you should get a return value of zero, while
    if you use the command \"`cat FileDoesNotExist`\" you should get
    some non-zero return value. You can also try returning different
    values from your program as well. But I digress- this is not the
    purpose of this studio, but still good for any Linux programmer to
    know.)

    Make sure your code compiles successfully. Remember we use the GCC
    compiler to build C programs. If you have named your program
    `pointers.c` then you can use a command such as
    `gcc -Wall pointers.c -o pointers` to build your code. You can leave
    this answer blank.

2.  Now back to pointers. Pointers are an essential part of programming
    in C, and one of the biggest things we use them for is accessing
    arrays. We can have arrays of any data type: chars, ints, floats,
    doubles, and so on. Copy the following line into your `main()`
    function.

    `char *messagePtr = "HELLOWORLD!"`

    Let\'s break down this statement. There are several important
    pieces.

    1.  *messagePtr* - This is the name of a variable we have just
        defined.
    2.  *The asterisk* - Declares that `messagePtr` is a pointer.
    3.  *char* - This declares that `messagePtr` is a pointer to a
        `char`. Remember that a `char` is a one-byte data type.
    4.  *\"HELLOWORLD!\"* - This is the string that the variable
        `messagePtr` points to, called a *string literal*.

    Next, let\'s print this string just to make sure everything is
    working correctly. Observe that the `%s` format specifier tells
    `printf()` to print a string.

    `printf("%s\n", messagePtr);`

    You will also need to include `stdio.h` at the top of your file.

    You can leave the answer to this question blank.

3.  We can access the elements of a string by *dereferencing* the string
    pointer. A pointer points to data in memory, and dereferencing that
    pointer gives us the *value* of the data. The simplest way to
    dereference is through the use of the square bracket *index
    notation*. The code `messagePtr[0]` gives you the first character
    \"H\", the code `messagePtr[1]` gives you the second character
    \"E\", etc.

    Print out each character of the string using a for-loop with index
    notation. You can print a character as such:
    \"`printf("%c\n", char_to_print)`. Your output should look like:

        H
        E
        L
        L
        O
        W
        O
        R
        L
        D
        !

    As the answer to this question, copy-paste your for-loop.

4.  The *dereference operator* in C is the asterisk (`*`) and is also
    important when using pointers. Just like indexing a pointer, the
    dereference operator obtains the *value* of the data that is pointed
    to.

    If the pointer `messagePtr` is a pointer to a character, what
    character does it point to? In other words, what do you think is the
    value of the dereference operation `*messagePtr`?

5.  Check your answer to the last exercise by dereferencing `messagePtr`
    and printing it out. The dereference operator is the asterisk when
    placed to the left of a pointer. You can print out a single
    character like so:

    `printf("%c\n", *pointer_to_string);`

    What was printed?

6.  Another way to use pointers is with *pointer arithmetic*. Suppose we
    have a regular string pointer called *ptr*, as seen above this
    points to the first character of the string. To access the next
    character we could add to this pointer as such:

    `ptr + 1  //same as saying ptr[1]`

    or we could access the fourth element of the string by adding:

    `ptr + 3  //accesses fourth element, same as ptr[3]`

    The index notation you just used is essentially pointer arithmetic
    (in fact the C standard defines index notation in terms of pointer
    arithmetic).

    Try printing the value of the next few bytes of the string using
    pointer arithmetic. To do so, add one, two, or three to the pointer
    before dereferencing. For example: `*(messagePtr + 1)`.

7.  Write a second for-loop to print the entire contents of the string
    using pointer arithmetic, one character at a time. As the answer to
    this question, copy and paste your new for-loop.

8.  It\'s easy to use pointers with strings when we know exactly what
    the string is. It\'s harder when we don\'t. For example, how did you
    figure out how many iterations of the for-loop you needed for the
    previous exercises? If you\'re like most of us, you just counted out
    the eleven characters in \"HELLOWORLD!\". So, how do we handle
    arbitrary strings?

    Recall that in a properly formatted C-style string the string
    pointer indicates the first character of the string and always ends
    with a *null-terminator* (ends with the character \'\\0\'). See the
    diagram below.

    ![A diagram of the string in memory. There are twelve consecutive
    bytes, starting with H, E, L, L, O and continuing. The eleventh byte
    is the exclamation point, and the twelfth byte is the NULL
    terminator.](pointers.PNG)

    We can always use the null-terminator to know when a string ends.
    For this exercise I want you to print the string using a while loop
    instead of a for-loop. Use the fact that the string ends with a null
    value as your loop condition. You can use the keyword `NULL` or the
    null character `'\0'` to compare against. As the answer to this
    question copy-paste your while loop.

9. It\'s also important to understand how pointers are treated when
    calling functions. Recall that the two basic ways to pass arguments
    to functions are *pass-by-value* and *pass-by-reference*. The C
    language does not support pass-by-reference, and everything is
    passed by value. So for example, you can\'t pass a string to a
    function.

    Suppose we want to write a function that uses strings- what can we
    do instead? The answer is to pass a pointer to the function- we can
    pass a pointer by value, and that pointer can point to a string.
    Copy the following function definition above your `main()` function:

        void printReverse( char* string ){

        }

    Starting with your code from the previous exercise, write a function
    that prints a string in reverse order. For example, printing the
    string pointed to by `messagePtr` should produce the output:

        !
        D
        L
        R
        O
        W
        O
        L
        L
        E
        H

    You should not make any assumptions about the string passed to the
    function. Use the fact that strings are always null-terminated to
    figure out how long the string is. (Do not use the C-standard
    library function strlen()- get the value yourself.) Your code should
    work equally well with `messagePtr` or with another string such as:

    `char *secondString = "Another string!";`

    As the answer to this exercise, copy-paste your function.

10. For our last exercise, we will write a second function that
    creates a reversed copy of an input string. Feel free to borrow code
    from any of the other exercises you\'ve done. Start with the
    following outline. This code should also go above your `main()`
    function.

        #include <stdlib.h>

        char* reverseString( char* input ){

        //1. First count how many characters are in the input string

        //This creates enough space to store the reversed string, plus one more byte
        //for the null terminator
        char* output = (char*)malloc( number_of_chars_in_input+1 );

        //2. Copy the input string to the output string in reverse order. There are
        //multiple ways to do this- consider using a counter, or consider using two
        //pointers. 

        //REMEMBER THAT YOUR OUTPUT STRING MUST END WITH A NULL TERMINATOR. This is not
        //provided for you automatically- you must put it there!

            return output; 
        }

    The above code bears some explanation. Look up the function
    `malloc()` in the manual pages. This function takes a number and
    returns a pointer to a region of memory with that many bytes. For
    example, `malloc(10)` returns a pointer to a region of memory with
    10 bytes that you can modify however you\'d like. This is necessary
    because we need someplace to *put* our reversed string. Then, the
    last line returns that pointer so it can be used.

    Verify your function works with the following code in your main
    function:

        char* reversedMessage = reverseString( messagePtr );
        printf("Reversed string: %s\n", reversedMessage);

    As the answer to this exercise, copy-paste your last function.

### Optional Enrichment Exercises

1.  None.

------------------------------------------------------------------------
