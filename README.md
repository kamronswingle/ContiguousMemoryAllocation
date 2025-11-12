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

gcc allocator.c memory_operations.c allocator_strategies.c display.c simulation.c utilities.c -o allocator

## Sample Usage:

./allocator 1048576
