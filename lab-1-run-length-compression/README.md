# CSCI 2510: Lab - Run Length Compression

System calls are the fundamental API provided by an operating system to
application programs. Programmers do not usually use system calls
directly, and instead prefer to use standard libraries that are
portable. For Lab 1, we will be using Linux system calls to explore the
low-level nature of how input and output is done. The `read()` and
`write()` system calls are the basic mechanisms for doing file input and
output in Linux. All other file I/O routines, such as `printf()` in the
C Standard Library, C++\'s stream operators (`<<` and `>>`) or Python\'s
file methods (`file.read()` and `file.write()`) are built upon these
C-interface system calls at some level.

To make sure we never take these high-level interfaces for granted,
we\'re going to work with Linux system calls directly. For Lab 1 we will
write a program that compresses and decompresses files.

In this lab, you will:

1.  Use the `open()`, `close()`, `read()`, and `write()` system calls to
    do file I/O
2.  Implement a simple lossless data compression method called
    [run-length
    encoding](https://en.wikipedia.org/wiki/Run-length_encoding).
3.  Perform proper Linux/Unix style error checking on all functions that
    may return an error

------------------------------------------------------------------------

### Lab Format

This assignment should be completed as an individual assignment.

------------------------------------------------------------------------

### Program name

`rle` - compress or decompress a file with run-length encoding

### Usage

`rle <input file> <output file> <compression length> <mode>`

`input file`: the file to compress/decompress

`output file`: the result of the operation

`compression length`: the base size of candidate runs

`mode`: specifies whether to compress or decompress- if mode=0, then
compress the input file, if mode=1 then decompress the input file

### Description

`rle` implements run-length encoding for the compression of files. You
must use the `read()` and `write()` system calls to read the input file and
write to the output file.

Run-length encoding is a compression technique that identifies \"runs\"
of repeated characters and represents these compactly. The length of
each run is counted, and the base is stored along with the number of
repetitions of that basis. For example, the string `AAABBBBBB` consists
of nine bytes, but it could be instead represented as `3A6B`, where
\"A\" and \"B\" are the basis of each run and the numbers give how many
times each base is repeated. This represents the 9-byte source as a
compressed 4-byte string, a data size ratio of 55.6%.

The base does not need to be length-1, and this is what the third
program parameter above specifies. For example, the string
`ABABABCDCDCDCD` compresses very poorly with length-1 encoding to
`1A1B1A1B1A1B1C1D1C1D1C1D1C1D`, which is an expansion from 14 bytes to
28 bytes. However, if we allow our base to be length-2 then we can
represent the above as `3AB4CD`, which is a reduction from 14 bytes to 6
bytes.

Each run should be encoded as a *binary* one-byte counter that is
followed by a pattern. The use of binary in the previous sentence implies
that the counter ranges between 1-255 repetitions. The counter explicitly
should not store the decimal ASCII encoding of the characters `'0'`-`'9'`. 

Thus, each run can be a maximum of 255 repetitions. Longer runs would
overflow the one-byte counter (recall
that an 8-bit unsigned integer can store values from 0-255). For
example, if you had the character \"A\" 300 times in a row, then
with a length-1 encoding your program should produce \"255A45A\" rather
than \"300A\".

One can imagine many versions of RLE that are optimizations of the above
principles. For this assignment I ask you to implement the simplest
algorithm that mimics the behavior given above. In particular, the algorithm
you implement should be as follows. Given a
run-length parameter K, and starting at the beginning of the input
file:

1.  Read in the first K bytes of the file, and set a counter to one
2.  Scan the next set of K bytes in the file, if these bytes equal the last set of K
    bytes then increment the counter by one
3.  Repeat the previous step until a new pattern is found
4.  When a new pattern is found, output the value of the counter along with the original pattern
5.  Reset the counter to one and repeat the above process for the new
    pattern by going back to step 2

Decompressing files is much easier:

1.  Read in the first byte of the file as an integer N
2.  Read in the next K bytes of the file as a length-K base pattern
3.  Output that pattern N times
4.  Repeat the above process

`rle` detects the following errors and quits gracefully:

-   Not enough / too many command line arguments (and prints a usage
    statement)
-   Compression length less than 1
-   A mode value other than 0 or 1
-   Input file does not exist or is otherwise not openable
-   Any errors returned from `open()`, `close()`, `read()`, or
    `write()` - use the function `perror()` to print useful error
    messages

Upon encountering any error, print a useful message and `exit()` with a
negative status code.

If no error is encountered then the program should not produce any
output to standard output, and should only modify the output file.

### Test Input Files

- test1
- test2
- test3
- test4
- 1mil\_random
- 1mil\_weak\_bias
- 1mil\_strong\_bias
- slu\_logo.bmp

These files are provided for you in your repository. 

### Sample Outputs

You can use the `xxd` program to inspect your program output. Many text
editors do not handle non-printable characters gracefully, but `xxd`
prints the underlying binary data in hexadecimal. For example, the
`test1` test case contains the sequence
`AAAABBBBBBBBCCCCCCCCCCCCAAAAAAAAAAAAAAAA` followed by a new-line
character. The `xxd` program generates the following output:

    xxd test1
    0000000: 4141 4141 4242 4242 4242 4242 4343 4343  AAAABBBBBBBBCCCC
    0000010: 4343 4343 4343 4343 4141 4141 4141 4141  CCCCCCCCAAAAAAAA
    0000020: 4141 4141 4141 4141 0a                   AAAAAAAA.

The left column gives the location of the displayed data within the
file. The middle section displays sixteen bytes of hexadecimal data
(remember that one hex number describes four bits, so two hex digits
describes one byte). The right section displays the printable-text
equivalent of the hexadecimal data. Looking from left to right we see
byte 0x41 repeated four times (ASCII code for \'A\'), then 0x42 repeated
eight times (for \'B\'), then 0x43 repeated twelve times (for \'C\') and
then 0x41 repeated sixteen times (for \'A\' again). The last byte in the
file, 0x0a, is the New Line character. Since the newline isn\'t a printable
character, it is represnted with a period in the right hand side
display.

Thus, compressing the first file acording to the specifications above
gives the following outputs:

    ./rle test1 compressed 1 0
    xxd compressed
    0000000: 0441 0842 0c43 1041 010a                 .A.B.C.A..

Notice we have the value 4 followed by the code for \'A\', then the
value 8 followed by the code for \'B\', then the value 12 (hexadecimal
code 0x0c) followed by the code for \'C\', and the value 16 followed by
the code for \'A\' again. Continuing:

    ./rle test1 compressed 2 0
    xxd compressed
    0000000: 0241 4104 4242 0643 4308 4141 010a       .AA.BB.CC.AA..

    ./rle test1 compressed 4 0
    xxd compressed
    0000000: 0141 4141 4102 4242 4242 0343 4343 4304  .AAAA.BBBB.CCCC.
    0000010: 4141 4141 010a                           AAAA..

    ./rle test2 compressed 1 0
    xxd compressed
    0000000: 0142 0341 0242 0141 0242 0143 010a 0141  .B.A.B.A.B.C...A
    0000010: 0142 0241 0142 0141 0142 0341 010a 0541  .B.A.B.A.B.A...A
    0000020: 0143 0441 010a                           .C.A..

    ./rle test3 compressed 1 0
    xxd compressed
    0000000: ff41 2d41 010a                           .A-A..

### Hints

1.  Make sure you master the basics of the `read()` and `write` system
    calls with studios 1 and 2 before attempting this lab assignment.
2.  Read the manual page for `open()` carefully. The output file may not
    exist, or if it does exist you should overwrite it starting at the
    beginning. Thus, you should specify the O\_CREAT and O\_TRUNC flags,
    and because you specify O\_CREAT you should also specify (at a
    minimum) the S\_IRUSR and S\_IWUSR file permissions.
3.  Four test files are provided for you to experiment with. Each of
    them test a different aspect of your program- make sure your program
    works with all of them! If you compress and then decompress the same
    file you should have identical contents before and after. Feel free
    to make your own test files as well.
4.  Look at the man pages for all the functions you use. All of them
    will give the possible return values as well as how errors are
    specified.
5.  You can use the `diff` program to compare before/after files and it
    highlight any differences. This is an easy way to detect whether or
    not a decompressed file is identical to the original source, especially
    when the files are too large to inspect visually!
6.  You can use the `ls -l` command to see how many bytes are in each
    file. You can also use the `wc` program with the `-c` argument. For example,
    `wc -c test1` should return 41 bytes.
8.  The `read()` system call returns how many bytes it has read. This is
    useful info- you might ask the `read()` function to read in four
    bytes, but it might not be able to, such as if you\'re at the end of
    a file. This also tells you when there is no more input to be read:
    you can keep reading until `read()` returns a 0 (end of file) or -1
    (error). In particular, at the end of your file, your last pattern
    might have fewer bytes than every other pattern, and this should not
    break your compression or decompression algorithm. See the output
    of `./rle test1 compressed 2 0` from above as an example.

### Documentation

The following man pages may be useful:

-   `open` (2)
-   `close` (2)
-   `read` (2)
-   `write` (2)
-   `strncmp` (3)
-   `memcpy` (3)
-   `atoi` (3)
-   `strtol` (3)
-   `perror` (3)
-   `errno` (2)
-   `exit` (3)
-   `xxd` (1)
-   `diff` (1)
-   `wc` (1)

### Questions

1.  As the answer to the first exercise, list the name(s) of the people
    who worked together on this lab.

2.  This compression technique works well in some cases, and poorly in
    others. Give an example of each.

3.  The *compression ratio* can be computed as (size before
    compression)/(size after compression). For example if the original
    file size is 20,000 bytes and the compressed size is 15,000 bytes
    then the compression ratio is 1.33. There are three files above of
    size one million bytes each: 1mil\_random, 1mil\_weak\_bias, and
    1mil\_strong\_bias. Each of these files contains the numbers 0-4 in
    random order, but the random file contains all numbers distrbuted
    evenly, while the weak and strong bias files shift the distribution
    so that the lower number values occur more often. For example, zero
    occurs about 20% of the time in the random file, about 75% of the
    time in the weak bias file, and about 90% of the time in the strong
    bias file.

    Which file do you think will compress best? Compute the compression
    ratio for each file with a compression length of one.

4.  The Bash shell includes the nifty command `time`, which records how
    long a command runs. Record how long it takes to compress the file
    `test4`. The syntax in this case is
    \"`time ./rle test4 output 1 0`\". This will report three
    measurements: real, user, and sys. How long does your program take
    to run in real time?

5.  Try the above command again but with a compression length of ten,
    how long does your program take now? Which operations happen more or
    less frequently now? Formulate a hypothesis as to the difference.

6.  Indicate which, if any, extra credit exercises have you have
    attempted.

### Optional Enrichment Exercises

-   **3% Extra Credit**: Download the slu\_logo.bmp file above, this is
    an uncompressed image file. Compress the file, then uncompress it.
    Verify that you can still open the image in a photo viewer and that
    it is not distorted.

    The compression algorithm you have implemented is a form of lossless
    compression, meaning that the entire uncompressed file can be
    reconstructed and no data is lost. This approach works well for
    highly structured data with lots of repetition. This can happen
    naturally in a case like slu\_logo.bmp, where the whole image has
    just a few colors and lots of large same-color regions. Compute the
    compression ratio of your algorithm with slu\_logo.bmp. Then, open
    the file in a photo editor (such as Microsoft Paint, Photoshop, or
    GIMP) and save it as a .png file (lossless compression) and a .jpg
    file (lossy compression). Compute the compression ratio for each
    method.

-   **2% Extra Credit**: Lossy compression can be far
    more effective in some situations than lossless compression.
    Compression algorithm researchers are very skilled at removing data
    that is virtually imperceptible. The `uncompressed.bmp` file is a much
    more complex scene than the `slu_logo.bmp`, and lacks the structure
    that can be exploited with `slu_logo.bmp`. Use the `uncompressed.bmp` file
    and compute the compression ratio using
    your algorithm. Then, use a standard photo editor from before to
    generate a losslessly compressed .png file and a lossily compressed
    .jpg file and compute their compression ratios. Give all three compression
    ratios in your answer. Inspect the files
    carefully, are you able to discern a difference between the three?

------------------------------------------------------------------------

### Submission

Please submit your lab to Canvas. Your code should
be entirely contained in the file called `rle.c`, and your question
responses should be included in an appropriately named text file.
