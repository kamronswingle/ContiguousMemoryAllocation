/*
    Name: Kamron Swingle
    Course: CPSC 380 - Operating Systems
    Email: swingle@chapman.edu
    Assignment: Assignment 5 - Contigious Memory Allicator
    File: allocator.h
    School: Chapman University
*/


#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Memory block structure
typedef struct MemoryBlock {
    int start;
    int size;
    char process[10];
    int is_allocated;
    struct MemoryBlock* next;
} MemoryBlock;

// Global variables
extern MemoryBlock *head;
extern int total_memory;

// Initialization
void initialize_memory(int size);

// Memory operations
void request_memory(char *process, int size, char strategy);
void release_memory(char *process);
void compact_memory(int max_size);
void allocate_in_hole(MemoryBlock* hole, char* process, int size);

// Allocation strategies
MemoryBlock* find_first_fit(int size);
MemoryBlock* find_best_fit(int size);
MemoryBlock* find_worst_fit(int size);

// Display functions
void display_status(int max_size, int verbose);
void visualize_memory(int max_size);

// Simulation
void simulate_from_file(char* filename, int max_size);

// Utility functions
MemoryBlock* create_node(int start, int size, int is_allocated, const char* process);
MemoryBlock* find_process(const char *process);
int process_exists(char *process);
void merge_adjacent_holes(MemoryBlock* block);
void insert_block_after(MemoryBlock *current, MemoryBlock *new_block);
int count_holes();
void free_all_memory(MemoryBlock* head);

#endif // ALLOCATOR_H
