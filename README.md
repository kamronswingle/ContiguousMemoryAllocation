# README

Name: Kamron Swingle

Course: CPSC 380 - Operating Systems

Email: swingle@chapman.edu

Assignment: Assignment 5 - Contiguous Memory Allicator

Required Files: allocator.c allicator.h

School: Chapman University

*Help recieved from freind outside of school. Name: Ian McQuerrey*

## Sources:

Memory Managment in OS:

https://www.geeksforgeeks.org/operating-systems/memory-management-in-operating-system/

Allocation Techniques:

https://www.geeksforgeeks.org/operating-systems/memory-allocation-techniques-mapping-virtual-addresses-to-physical-addresses/

Defining linked list in C:

https://www.geeksforgeeks.org/c/linked-list-in-c/

Initializing and implementing LL:

https://stackoverflow.com/questions/982388/how-to-implement-a-linked-list-in-c

Types of fits:

https://www.geeksforgeeks.org/operating-systems/partition-allocation-methods-in-memory-management/

Dynamic allocation:

https://www.geeksforgeeks.org/c/dynamic-memory-allocation-in-c-using-malloc-calloc-free-and-realloc/

Atoi:

https://www.geeksforgeeks.org/c/c-atoi-function/

Parsing strings:

https://www.w3schools.com/c/c_user_input.php

Strcpy:

https://www.geeksforgeeks.org/cpp/cpp-strcpy/

File handling:

https://www.geeksforgeeks.org/c/basics-file-handling-c/

cstring:

https://cplusplus.com/reference/cstring/

sscanf vs. scanf

https://stackoverflow.com/questions/27054634/how-is-sscanf-different-from-scanf-in-the-following-statements

strcmp:

https://www.geeksforgeeks.org/c/strcmp-in-c/

Passing in strings in functions:

https://stackoverflow.com/questions/17131863/passing-string-to-a-function-in-c-with-or-without-pointers

Iterating through linked list:

https://stackoverflow.com/questions/11463514/using-loop-to-traverse-through-linked-list

Copying a linked list to another:

https://stackoverflow.com/questions/4984071/how-do-you-copy-a-linked-list-into-another-list

## Compilation Instructions:

gcc allocator.c -o allocator

## Sample Usage:

./allocator 1048576

## Sample Output:

allocator>SIM test.txt
Starting simulation from file: test.txt

allocator> RQ P0 100000 F

allocator> RQ P1 200000 F

allocator> RQ P2 150000 F

allocator> STAT
Allocated memory:
Process P0: Start = 0.00 KB, End = 97.66 KB, Size = 97.66 KB
Process P1: Start = 97.66 KB, End = 292.97 KB, Size = 195.31 KB
Process P2: Start = 292.97 KB, End = 439.45 KB, Size = 146.48 KB

Free memory:
Hole 1: Start = 439.45 KB, End = 1024.00 KB, Size = 584.55 KB

Summary:
Total allocated: 439.45 KB
Total free: 584.55 KB
Largest hole: 584.55 KB
External fragmentation: 0.0% (1-largest free block/total free memory)
Average hole size: 584.55 KB

allocator> RL P1
Successfully released memory for process P1

allocator> STAT
Allocated memory:
Process P0: Start = 0.00 KB, End = 97.66 KB, Size = 97.66 KB
Process P2: Start = 292.97 KB, End = 439.45 KB, Size = 146.48 KB

Free memory:
Hole 1: Start = 97.66 KB, End = 292.97 KB, Size = 195.31 KB
Hole 2: Start = 439.45 KB, End = 1024.00 KB, Size = 584.55 KB

Summary:
Total allocated: 244.14 KB
Total free: 779.86 KB
Largest hole: 584.55 KB
External fragmentation: 25.0% (1-largest free block/total free memory)
Average hole size: 389.93 KB

allocator> C
Memory compacted successfully

allocator> STAT -v
Allocated memory:
Process P0: Start = 0.00 KB, End = 97.66 KB, Size = 97.66 KB
Process P2: Start = 97.66 KB, End = 244.14 KB, Size = 146.48 KB

Free memory:
Hole 1: Start = 244.14 KB, End = 1024.00 KB, Size = 779.86 KB

Summary:
Total allocated: 244.14 KB
Total free: 779.86 KB
Largest hole: 779.86 KB
External fragmentation: 0.0% (1-largest free block/total free memory)
Average hole size: 779.86 KB

\[\#\#\#\#\#\#\#\#\#\#\#\#\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\.\]

^0                                                                                                     ^1048576

allocator> X
Exiting simulation
Simulation complete

*Note: If you are reading this in plaintext, ignore the backslashes in the sample output, it is just for .md formatting on github*
