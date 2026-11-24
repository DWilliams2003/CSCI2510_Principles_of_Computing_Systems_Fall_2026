# CSCI 2510: Lab `deadlock` - Deadlock Detection via Graph Cycles

Deadlock is a hazard inherent to concurrent and parallel systems.
Whenever two or more actors hold resources, and there is the possibility that
they may in turn wait for other resources, there is a risk of deadlock.
Threads locking and unlocking mutex variables, processes getting exclusive access
to hardware devices, and network applications waiting on remote servers to
respond to requests are just a few examples of where deadlock can occur.

One defense is the use of a deadlock detector. With such a detector, a system
could discover the occurence of deadlock and take corrective action after
it has occured. Or, a system could preemptively simulate the effects 
of resource acquisition and avoid taking actions that would lead to deadlock.

In this lab, you will:

1.  Parse formatted input with `scanf()`
2.  Represent graphs with an adjacency matrix
3.  Model resource acquisition in a system with a Resource Allocation
    Graph (RAG)
4.  Detect cycles in the RAG through recursive graph search

------------------------------------------------------------------------

### Parameters

This is a solo assignment. Please do not collaborate with other students
on this assignment.

------------------------------------------------------------------------

### Usage

`./deadlock < test_file`

Returns -1 when deadlock is detected and prints out the list of
processes and resources that participate in the deadlock cycle.

Returns 0 when no deadlock is detected by the end of input.

### Description

`deadlock` should model a set of interacting processes and resources.
In this model, processes may attempt to acquire a resource, and they
may release a resource. As these actions take place, a resource
allocation graph (RAG) should be maintained to model the state of the
system. After each action, a cycle-detection algorithm looks for
cycles in the RAG. A cycle indicates deadlock, and the program terminates
with exit code -1. If all input is consumed and no cycle is detected,
then the program terminates with with exit code 0.  

Input to the program is a sequence of resource transactions indicating
a process attempting to acquire or release a resource. Each transaction
is given on a separate line of input, in the format:

`<process number> <a or r> <resource number>`

For example, if process 0 wants to acquire resource 2, this would be
coded as:

`0 a 2`

Or, if process 11 wanted to release resource 12, this would be:

`11 r 12`

`deadlock` should continue reading from standard input until
it fails or there is no more input to read. This allows us to model
a system through a series of transactions. For example, the `test1`
file contains the basic deadlock example we looked at in class:

    0 a 0
    1 a 1
    0 a 1
    1 a 0

In this sequence, process 0 acquires resource 0, process 1 acquires
resource 1, then process 0 attempts to acquire resource 1, and
finally process 1 attempts to acquire resource 0. At this point,
a cycle is formed, and the program returns -1.

When the program detects deadlock, it should print out the processes
and resources that are participating in the cycle:

    ./deadlock < test1
    RESOURCE 1 -> PROCESS 1
    PROCESS 0 -> RESOURCE 1
    RESOURCE 0 -> PROCESS 0
    PROCESS 1 -> RESOURCE 0

However, the following sequence would not deadlock, because process
0 releases its hold on resource 0 before the final acquire action:

    0 a 0
    1 a 1
    0 a 1
    0 r 0
    1 a 0

If the program detects a cycle before the end of input, it should
stop processing and return immediately with -1. In our model, a 
blocked process cannot release a resource after deadlock has already 
occured. 

In our model, resources can only be held by a single process at a time,
but processes may be waiting on multiple resources. (This is justified
by the observation that a process may contain multiple threads or
sub-processes, each of which may be waiting on a resource. Conversely,
there are models of deadlock where resources may be held by multiple
processes, but we chose not to consider those here.) The semantics of
our two actions are thus:

* When a process attempts to acquire a resource, the program checks
  whether the resource is already held by another process. If it is,
  then the acquring process is said to wait on the resource. If it is
  not, the process is said to acquire and hold the resource. Note that
  it is possible to self-deadlock. This occurs when a process attempts 
  to acquire the same resource twice. For example, the following sequence
  self-deadlocks on the third instruction:

        0 a 0
        1 a 0
        0 a 0
        0 a 1

* When a process releases a resource, the process no longer holds it.
  However, other processes may already be waiting for the resource.
  In `deadlock`, a released resource is assigned to the lowest-numbered
  waiting process. Note that this should be considered an acquisition,
  and we should then check for deadlock. For example, the following
  sequence results in deadlock on the last instruction, because
  process 0 acquires resource 0 and completes the RAG cycle:

        2 a 0
        0 a 0
        1 a 0
        1 a 1
        0 a 1
        2 r 0

Processes only release a resource that they are already holding. If a
process attempts to release a resource it does not hold, then nothing
should happen. 

### Implementation

You may assume a maximum of 15 processes and 15 resources. 

You should implement `deadlock` using the RAG formulation. Remember that a RAG contains a set of processes and a
set of nodes, and edges denote holding/waiting among the processes.
If we have a process P and a resource R, then the edge P -> R indicates
that P is waiting on R. The edge R -> P indicates that P has acquired
and is holding R. 

Graphs are traditionally represented in a machine through either an
adjacency list or an adjacency matrix. Given that C does not have built
in support for lists, I strongly suggest an adjacency matrix approach. In
my solution, I have an adjacency matrix where rows represent processes and
columns represent resources. The entries of the matrix are 1 where an
edge exists from the process to the resource, and zero everywhere else.
For example, the following matrix indicates that process 0 is waiting
on resource 3, process 1 is waiting on resource 1, and process 2 is
waiting on both resources 2 and 3:

    0 0 0 1
    0 1 0 0
    0 0 1 1

This represents all edges P -> R where a process is waiting. 

To represent those edges R -> P, i.e. resource holding, I suggest using an
array. Remember that each resource can only be held by a single process
at a time, so a matrix would be would be mostly unused. For example, the
following vector could mean that resource 1 is held by process 2, resource
2 is held by process 0, and resources 0 and 3 are not currently held:

    -1
    2
    0
    -1

Implementing graph search is the hardest part of this lab, but we can
make a few assumptions that make the computation easier:

1. We start with an empty graph, which is deadlock-free. Our program stops
   when it finds deadlock. Thus, we can assume that every graph is
   deadlock-free before we perform an acquire action.  

2. We add exactly one edge after each acquire, so if there is a cycle
   in our graph, then the edge we just added must participate in that
   cycle. 

3. The instruction `<p> a <r>` either results in the edge p -> r, or the
   edge r -> p. In either case, if a cycle exists, it means there exists
   a path starting at p that eventually ends up back at p.

Thus, for a given instruction `<p> a <r>`, deadlock happens exactly when
we are able to find a path back to the process node p.

Note that a release instruction `<p> r <r>` may cause some other process
to acquire the resource r. In this case, we start searching at that
other process and see whether we're able to find a path back to that
other process.

You can use either breadth-first or depth-first search for cycle detection.
A recursive implementation is the easiest way to write these algorithms in C. 


### Hints

You will need to accept formatted input for this assignment. I suggest
using the `scanf()` function from the C standard library. 

C doesn't support matrices exactly, but does support an array of arrays.
The following declaration will look and act like a matrix:

`int processes_to_resources [NUM_PROCS][NUM_RES]`

Be careful to keep track of whether the first or second index refers to
processes and resources.

Remember that each resource will have at most one outgoing edge. Processes
may have multiple outgoing edges. If you structure your data as I suggested
above, it is probably easiest to have two functions: `search_process()` and
`search_resource()` to reflect that process edges are stored in a matrix and
resource edges are stored in an array. 

There are never edges between two resources or two processes. Thus, in my
code, I start at the starting process, and then alternate between calling 
`search_resource()` and `search_process()`. 

Write lots of little helper functions to make sure you're interacting with
your data structures in a consistent and sane manner. A few of the functions
my solution has are:

* void acquire( int process, int resource )
* int release( int resource )
* int start\_search( int start\_process )
* int search\_resource( int visited[NUM\_PROCS], int start, int current )
* int search\_process( int visited[NUM\_PROCS], int start, int current )
* void init\_matrix()


### Useful man pages

* `scanf(3)` 
