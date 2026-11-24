# CSCI 2510: Lab `crack` - Brute Force Password Cracking

Modern computers overwhelmingly use multi-core architectures. Exploiting
this hardware for parallel programming (creating a program that executes
simultaneously on more than one processor core) requires the use of
multiple threads. We will use one of the most basic threading
interfaces, Pthreads, to create a multi-threaded password cracking
program.

In this lab, you will:

1.  Use `crypt()` and `crypt_r()` to guess password hashes
2.  Iterate over aribtrary length strings with characters from \'a\' to
    \'z\'
3.  Create and wait for threads with `pthread_create()` and
    `pthread_join()`
4.  Divide work between multiple parallel threads

------------------------------------------------------------------------

### Parameters

This is a solo assignment. Please do not collaborate with other students
on this assignment.

------------------------------------------------------------------------

### Usage

`crack <threads> <keysize> <target>`

### Description

`crack` should attempt to find the password associated to the target DES
hash. It does this by trying all possible lowercase alphabetic (a-z)
passwords of length up to `keysize`. The program should run with
`threads` concurrent threads for speed.

Linux/Unix user passwords are never stored on the system. Instead, a
function called a hash is applied to the password. Then, the hashed
password is stored, traditionally in /etc/passwd or more recently in
/etc/shadow. The classic hash function on Unix systems is crypt(3). To
make things harder on crackers, crypt also uses a two character string
called a salt which it combines with the password to create the hash.
Schematically:

`password + salt => crypt() => hash`

The salt is visible in the hash as the first two characters. As an
example, a password `'apple'` and salt `'na'` become the hash
`'na3C5487Wz4zw'`.

The crack program should extract the salt from the first two characters
of target, then repeatedly call `crypt()` using all possible passwords
built of up to `keysize` lowercase alphabetic characters. For example,
if the given `keysize` is four then your program would iterate over all
strings `'aaaa', 'aaab', 'aaac', ..., 'zzzz'` looking for one that
hashes to the given input. If a match is not found, it would also look
through all strings of length less than `keysize`.

When a match to `target` is found, the program should print the cracked
password and exit immediately. If the entire space of passwords is
searched with no match, the program should exit with no output.

### Hints

Start by writing a short program that uses `crypt()` to encrypt a given
password and salt. You can also encrypt using the one line perl command:
`perl -e 'print crypt("apple","na")'`

Don\'t forget to compile with the -lpthread and -lcrypt options.

The maximum allowed keysize is 8 (since crypt only uses the first 8
characters of the password anyway).

You might want to write this first as a single threaded program. Just
remember that `crypt()` won\'t work with multiple threads - you need to
switch to `crypt_r()`.

You need to check passwords of length `keysize`, but don\'t forget you
also need to check the shorter ones. The simplest way is probably to
write a function that checks all passwords which are exactly a given
length, and then have main call it in a loop from 1 to `keysize`.

### Useful man pages

* `pthread_create(3)` 
* `pthread_join(3)` 
* `pthread_exit(3)` 
* `pthread_self(3)` 
* `pthread_mutex_init(3)` 
* `pthread_mutex_lock(3)` 
* `pthread_mutex_unlock(3)` 
* `crypt_r(3)` 
* `strcmp(3)` 
* `strcpy(3)` 
* `strncpy(3)`

### Questions

1.  As the answer to the first exercise, list your name.
2.  What four-letter password corresponds to the following target:
    \'`abccBcrPOxnLU`\'?
3.  What five-letter password corresponds to the following target:
    \'`xyxMB6gxuiBZg`\'?
4.  The following hash does not correspond to any password, so your
    program will exhaust the entire search space without finding any
    match. Use the `time` command to measure how long your program takes
    to search the entire space of five-letter passwords. Hash
    \'`nomatchingpass`\'
5.  Re-run the previous experiment but with two threads, four threads,
    eight threads, thirteen threads, and twenty-six threads. Record how
    long each run takes.
6.  Calculate the *hash rate* your program gets as a result of using
    multiple threads. The hash rate is defined to be the number of
    hashes computed divided by the execution time. Do this for each of
    the two, four, eight, thirteen, and twenty-six threads.
7.  Interpret your results. Does your program run twice as fast with two
    threads? Does it run eight times faster with eight threads? How does
    this relate to the way you have written your program?
8.  What was the maximum hash rate you recorded?
9.  Assuming each password only uses the 26 lowercase characters,
    compute the total number of hashes needed to exhaustively search the
    keyspace for keysizes of one through eight.
10. Using the last two answers, estimate how long it should take to
    exhaust each keyspace at your maximum rate? Describe the benefit of
    increasing key length.
11. Instead of just using lowercase characters, estimate how long it
    takes to exhaustively search a keyspace of size six when you are
    allowed to use lowercase and uppercase (52 characters), digits (62
    characters), and common keyboard symbols (93 characters). Use your
    maximum hash rate.
12. Describe any known bugs or ways that your submission deviates from
    the above specification.
13. Indicate which, if any, extra credit exercises you have attempted.

### Optional Enrichment Exercises

-   **2% Extra Credit**: Crack the following five-letter password
    hashes:

* `abA.g8pU2Iffo` 
* `cdfnIXMyMCpPg` 
* `efgC/gw8PDKhs`
* `ghLneTdBMxJP.` 
* `ijSsXTgIC7QRU` 
* `klr7dT7cAODsk` 
* `mn4iWfK0m76t6` 
* `opjPsgpXaahxM` 
* `qr6ncfvfqecME`

-   **2% Extra Credit**: Explain why password salts are useful, even if
    they\'re not secret.
-   **2% Extra Credit**: Add a fourth paramter to your program which, if
    present, causes your program to search all uppercase and lowercase
    characters, plus numbers and symbols. *Hint: look at an ASCII table.
    What range of decimal encodings includes all of the above?*
