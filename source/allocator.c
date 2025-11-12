/*
    Name: Kamron Swingle
    Course: CPSC 380 - Operating Systems
    Email: swingle@chapman.edu
    Assignment: Assignment 5 - Contigious Memory Allicator
    File: allocator.c
    School: Chapman University
*/

#include "allocator.h"

MemoryBlock *head = NULL; // globally defining the head of the linked list, as there will only be 1
int total_memory = 0; // total memory allocated to find averages

int main(int argc, char *argv[]) {
    if (argc != 2) { // making sure user supplied enough arguments
        fprintf(stderr, "Error: Usage: %s <memory_size>\n", argv[0]);
        return 1;
    }
    
    int max_size = atoi(argv[1]); // define the max size from argument 1
    initialize_memory(max_size); // initializing the memory (setting up the linked list)
    
    char command[100]; // entire command for parsing
    char command_type[10]; // type of command passed (e.g. F, W, B etc.)

    while (1) { // main loop for CLI
        printf("allocator>");

        if (fgets(command, sizeof(command), stdin) == NULL) { // if the user doesn't say anything and hits enter just break
            break;
        }

        sscanf(command, "%s", command_type); // breaking up the command and the command type

        if (strcmp(command_type, "X") == 0) { // quitting, just break the loop
            break;
        }
        else if (strcmp(command_type, "RQ") == 0) { // if the user types RQ, parse through and check if they properly used the command
            char process[10];
            int size;
            char strategy;

            if (sscanf(command, "RQ %s %d %c", process, &size, &strategy) != 3) {
                printf("Error, usage: RQ <process> <size> <F|B|W>\n");
                continue;
            }

            if (strategy != 'F' && strategy != 'B' && strategy != 'W') {
                printf("Error: strategy must be F, B, or W\n");
                continue;
            }

            request_memory(process, size, strategy); // if everything is right, request the memory (make a new item in the list)
        }
        else if (strcmp(command_type, "RL") == 0) {
            char process[10];

            if (sscanf(command, "RL %s", process) != 1) {
                printf("Error, Usage: RL <process>\n");
                continue;
            }

            release_memory(process); // deleting an item in the list (also merges adjacent holes to avoid swiss cheese empty blocks in memory)
        }
        else if (strcmp(command_type, "C") == 0) {
            compact_memory(max_size); // compaction
        }
        else if (strcmp(command_type, "STAT") == 0) {
            char flag[10] = "";
            sscanf(command, "STAT %s", flag);
            
            if (strcmp(flag, "-v") == 0) { // if the user types STAT -v, pass in 1, used as a flag for verbose
                display_status(max_size, 1);
            }
            else {
                display_status(max_size, 0); // otherwise pass in 0, user did not type verbose
            }
        }
        else if (strcmp(command_type, "SIM") == 0) {
            char filename[100];
            if (sscanf(command, "SIM %s", filename) == 1) {
                simulate_from_file(filename, max_size);
            }
            else {
                printf("Error: Usage SIM <filename>\n");
                continue;
            }
        }
        else {
            printf("Error: unknown command.\n"); // if the user typed something unknown, print an error, but continue
        }
    }

    free_all_memory(head); // when done, clean up all memory (delete the entire list)
    return 0; // program completed successfully
}

void initialize_memory(int size) {
    total_memory = size;

    head = (MemoryBlock*)malloc(sizeof(MemoryBlock)); // initialize the linked list with random values by the size of the memory block
    if (head == NULL) {
        fprintf(stderr, "Error creating memory block.\n");
        exit(1);
        // no need to free memory here, because head is null anyways
    }

    // default initial values, process is set to null terminator, as we treat it as an empty string
    head->start = 0;
    head->size = size;
    head->process[0] = '\0';
    head->is_allocated = 0;
    head->next = NULL;
}

void request_memory(char *process, int size, char strategy) {
    if (process_exists(process)) { // this makes sure a user is not able to create a process with a duplicate name
        printf("Error: Process %s already exists, choose different process name.\n", process);
        return;
    }

    MemoryBlock* chosen_hole = NULL;
    
    // each type of strategy calls the corresponding helper function, find the fit
    if (strategy == 'F') {
        chosen_hole = find_first_fit(size);
    }
    else if (strategy == 'B') {
        chosen_hole = find_best_fit(size);
    }
    else if (strategy == 'W') {
        chosen_hole = find_worst_fit(size);
    }

    if (chosen_hole == NULL) {
        printf("Error: insufficient memory\n"); // somewhat un-needed, but makes sure the hole can be found
        return;
    }

    allocate_in_hole(chosen_hole, process, size); // now add it to the linked list
}

void release_memory(char *process) { // function just to delete a single item from a linked list
    MemoryBlock* block = find_process(process);

    if (block == NULL) { // make sure the process exists
        printf("Error: Process %s not found\n", process);
        return;
    }

    block->is_allocated = 0; // setting values to 0
    block->process[0] = '\0';

    merge_adjacent_holes(block); // ensure there are no tiny holes next to adjacent processes

    printf("Successfully released memory for process %s\n", process);
}

void allocate_in_hole(MemoryBlock* hole, char* process, int size) {
    if (hole->size == size) { // if the hole fits, make the hole allocated, and copy the data to the new hole
        hole->is_allocated = 1;
        strcpy(hole->process, process);
    }
    else { // otherwise it doesn't fit perfectly, so store the process, but create a new hole with the remaining info
        hole->is_allocated = 1;
        strcpy(hole->process, process);

        MemoryBlock* remaining = create_node(hole->start + size, hole->size - size, 0, NULL);
        insert_block_after(hole, remaining);
        hole->size = size;
    }
}

void compact_memory(int max_size) {
    MemoryBlock* current = head;
    int has_allocated = 0;

    while (current != NULL) { // just making sure the memory is populated, no need to continue if nothing on the list
        if (current->is_allocated) {
            has_allocated = 1;
            break;
        }
        current = current->next; // iterating through the linked list
    }

    if (!has_allocated) { // quitting if nothing to compact
        printf("No allocated blocks to compact.\n");
        return;
    }

    // Assigning old linked list head and new one that will be copied to
    MemoryBlock* old_head = head; // assigning the old head 
    MemoryBlock* new_head = NULL; // defining a new head, will be assigned at the end 
    MemoryBlock* new_tail = NULL; // tail to help iterate through the list and find the previous block
    
    int current_address = 0;
    int total_allocated = 0;
    
    current = old_head;
    while (current != NULL) { // iterating through original list
        if (current->is_allocated) {
            MemoryBlock* new_block = (MemoryBlock*)malloc(sizeof(MemoryBlock)); // creating a new list
            if (new_block == NULL) {
                fprintf(stderr, "Error: Unable to allocate memory during compaction\n"); // make sure we can create a new list, check if it was made
                free_all_memory(old_head);
                exit(1); // if it fails we have to completely exit the program
            }
           
            // copying over everything from old list
            new_block->start = current_address;
            new_block->size = current->size;
            new_block->is_allocated = 1;
            strncpy(new_block->process, current->process, sizeof(new_block->process) - 1);
            new_block->process[sizeof(new_block->process) - 1] = '\0';
            new_block->next = NULL;
            
            if (new_head == NULL) {
                new_head = new_block;
                new_tail = new_block;
            } else {
                new_tail->next = new_block;
                new_tail = new_block;
            }
            
            current_address += current->size;
            total_allocated += current->size;
        }
        current = current->next;
    }
    
    if (total_allocated < max_size) {
        MemoryBlock* free_block = (MemoryBlock*)malloc(sizeof(MemoryBlock)); // creating a new node for the hole
        if (free_block == NULL) {
            fprintf(stderr, "Error: Unable to allocate memory for free block\n");
            free_all_memory(head);
            exit(1);
        }
        
        free_block->start = current_address; // hole starts right after the last allocated process 
        free_block->size = max_size - total_allocated; // the hole size is the remaining space
        free_block->is_allocated = 0; // default initialization
        free_block->process[0] = '\0';
        free_block->next = NULL;
        
        if (new_head == NULL) { // if all the memory was free, the new head is the free block 
            new_head = free_block;
        } else { // otherwise  add the hole after the allocated block
            new_tail->next = free_block;
        }
    }
    
    current = old_head;
    while (current != NULL) {
        MemoryBlock* temp = current;
        current = current->next;
        free(temp); // deleting old list
    }
    
    head = new_head; // reassigning global head to new linked list
    
    printf("Memory compacted successfully\n");
}

MemoryBlock* find_first_fit(int size) {
    MemoryBlock* current = head;
    while (current != NULL) {
        if (!current->is_allocated && current->size >= size) { // if the current is not allocated and we are inside the list
            return current; // this is a fit, return the first one found
        }
        current = current->next;
    }
    return NULL;
}

MemoryBlock* find_best_fit(int size) {
    MemoryBlock* current = head;
    MemoryBlock* best = NULL;

    while (current != NULL) {
        if (!current->is_allocated && current->size >= size) { // if the current is not allocated, and we are inside the list
            if (best == NULL || current->size < best->size) { // if the best is unassigned, or the current size is less than the best size
                best = current; // this is the best fit
            }
        }
        current = current->next;
    }
    return best;
}

MemoryBlock* find_worst_fit(int size) {
    MemoryBlock* current = head;
    MemoryBlock* worst = NULL;

    while (current != NULL) {
        if (!current->is_allocated && current->size >= size) {
            if (worst == NULL || current->size > worst->size) { // very similar to worst fit, except, if the current size is larger than the worst size
                worst = current; // this is the worst fit, return it
            }
        }
        current = current->next;
    }
    return worst;
}

void display_status(int max_size, int verbose) { // self explanitory, printing the memory block either verbose or not
    MemoryBlock* current = head;
    
    size_t total_allocated = 0;
    double total_free = 0;
    size_t largest_hole = 0;
    int hole_count = 0;
    
    printf("Allocated memory:\n");
    current = head;
    while (current != NULL) {
        if (current->is_allocated) {
            printf("Process %s: Start = %.2f KB, End = %.2f KB, Size = %.2f KB\n",
                   current->process,
                   current->start / 1024.0,
                   (current->start + current->size) / 1024.0,
                   current->size / 1024.0);
            total_allocated += current->size;
        }
        current = current->next;
    }
    
    printf("\nFree memory:\n");
    current = head;
    int hole_num = 1;
    while (current != NULL) {
        if (!current->is_allocated) {
            printf("Hole %d: Start = %.2f KB, End = %.2f KB, Size = %.2f KB\n",
                   hole_num++,
                   current->start / 1024.0,
                   (current->start + current->size) / 1024.0,
                   current->size / 1024.0);
            total_free += current->size;
            hole_count++;
            if (current->size > largest_hole) {
                largest_hole = current->size;
            }
        }
        current = current->next;
    }
    
    printf("\nSummary:\n");
    printf("Total allocated: %.2f KB\n", total_allocated / 1024.0);
    printf("Total free: %.2f KB\n", total_free / 1024.0);
    printf("Largest hole: %.2f KB\n", largest_hole / 1024.0);
    
    if (total_free > 0) {
        double frag = (1.0 - ((double)largest_hole / (double)total_free)) * 100.0;
        printf("External fragmentation: %.1f%% (1-largest free block/total free memory)\n", frag);
    } else {
        printf("External fragmentation: 0.0%% (1-largest free block/total free memory)\n");
    }
    
    if (hole_count > 0) {
        printf("Average hole size: %.2f KB\n", (total_free / hole_count) / 1024.0);
    } else {
        printf("Average hole size: 0.00 KB\n");
    }
    
    if (verbose) {
        printf("\n");
        visualize_memory(max_size);
    }
}

void visualize_memory(int max_size) { // if the user used the verbose flag call this, draw a simulated memory block of size 50
    char visual[51];
    visual[50] = '\0';
    
    for (int i = 0; i < 50; i++) {
        visual[i] = '.';
    }
    
    double bytes_per_char = (double)max_size / 50.0;
    
    for (int i = 0; i < 50; i++) {
        int position_address = (int)(i * bytes_per_char);
        
        MemoryBlock* current = head;
        while (current != NULL) { // iterate through the list and either draw it assigned (#) or empty (.)
            if (position_address >= current->start && 
                position_address < current->start + current->size) {
                
                if (current->is_allocated) {
                    visual[i] = '#';
                } else {
                    visual[i] = '.';
                }
                break;
            }
            current = current->next;
        }
    }
    
    printf("[%s]\n", visual);
    printf("^0");
    
    for (int i = 0; i < 48; i++) {
        printf(" ");
    }
    printf("^%d\n", max_size);
}

void simulate_from_file(char *filename, int max_size) { // very similar to main function, but this time automated through a file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file '%s'\n", filename);
        return;
    }
    
    printf("Starting simulation from file: %s\n\n", filename);
    
    char line[256];
    int line_number = 0;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            continue;
        }
        
        printf("allocator> %s\n", line);
        
        char command_type[10];
        sscanf(line, "%s", command_type);
        
        if (strcmp(command_type, "RQ") == 0) {
            char process[10];
            int size;
            char strategy;
            
            if (sscanf(line, "RQ %s %d %c", process, &size, &strategy) == 3) {
                request_memory(process, size, strategy);
            } else {
                printf("Error on line %d: Invalid RQ command format\n", line_number);
            }
        }
        else if (strcmp(command_type, "RL") == 0) {
            char process[10];
            
            if (sscanf(line, "RL %s", process) == 1) {
                release_memory(process);
            } else {
                printf("Error on line %d: Invalid RL command format\n", line_number);
            }
        }
        else if (strcmp(command_type, "C") == 0) {
            compact_memory(max_size);
        }
        else if (strcmp(command_type, "STAT") == 0) {
            char flag[10] = "";
            sscanf(line, "STAT %s", flag);
            
            if (strcmp(flag, "-v") == 0) {
                display_status(max_size, 1);
            } else {
                display_status(max_size, 0);
            }
        }
        else if (strcmp(command_type, "X") == 0) {
            printf("Exiting simulation\n");
            break;
        }
        else {
            printf("Error on line %d: Unknown command '%s'\n", line_number, command_type);
        }
        
        printf("\n");
    }
    
    fclose(file);
    printf("Simulation complete\n");
}

MemoryBlock* create_node(int start, int size, int is_allocated, const char* process) { // simple function to create a node in a linked list
    MemoryBlock* new_node = (MemoryBlock*)malloc(sizeof(MemoryBlock));

    if (new_node == NULL) {
        fprintf(stderr, "Error creating new memory block.\n");
        free_all_memory(head);
        exit(1);
    }

    new_node->start = start;
    new_node->size = size;
    new_node->is_allocated = is_allocated;

    if (process != NULL && is_allocated) {
        strncpy(new_node->process, process, sizeof(new_node->process) - 1);
        new_node->process[sizeof(new_node->process) - 1] = '\0';
    }
    else {
        new_node->process[0] = '\0'; 
    }

    new_node->next = NULL;

    return new_node;
}

MemoryBlock* find_process(const char *process) { // finding a process at an index, return the index, if not found, return null
    MemoryBlock* current = head;
    while (current != NULL) {
        if (current->is_allocated && strcmp(process, current->process) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int process_exists(char *process) { // check if a process exists, if does return 1, if nont return 0
    MemoryBlock* current = head;
    while (current != NULL) {
        if (current->is_allocated && strcmp(current->process, process) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void merge_adjacent_holes(MemoryBlock* block) { // makes sure there are no tiny holes, merges small holes next to processes into large holes
    if (block->next != NULL && !block->next->is_allocated) {
        MemoryBlock* next_block = block->next;
        block->size += next_block->size;
        block->next = next_block->next;
        free(next_block);
    }
    
    MemoryBlock* prev = NULL;
    MemoryBlock* current = head;
    
    while (current != NULL && current != block) {
        prev = current;
        current = current->next;
    }
    
    if (prev != NULL && !prev->is_allocated) {
        prev->size += block->size;
        prev->next = block->next;
        free(block);
    }
}

void insert_block_after(MemoryBlock *current, MemoryBlock *new_block) { // insertion into a linked list
    new_block->next = current->next;
    current->next = new_block;
}

int count_holes() { // counting the number of holes in the memory block, used for averages
    int count = 0;
    MemoryBlock* current = head;
    while (current != NULL) {
        if (!current->is_allocated) {
            count++;
        }
        current = current->next;
    }
    return count;
}

void free_all_memory(MemoryBlock* head) { // complete deletion of entire list, used for cleanup
    MemoryBlock* current = head;
    MemoryBlock* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

