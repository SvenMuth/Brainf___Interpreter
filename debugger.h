//
// Created by sven on 27.01.25.
//



#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "interpreter.h"

#define CLEAR_SCREEN()      printf("\033[H\033[J")

typedef struct debug_info
{
    char* instructions;
    int instructions_length;
    int current_index;
    node_t* current_pos;
}debug_info_t;

void debug(debug_info_t debug_info);

void calculate_range_of_code_to_print(int* range_neg, int* range_pos, debug_info_t debug_info);
void print_debug_code(debug_info_t debug_info);
void print_status(debug_info_t debug_info, const long step);
void print_tape_section(node_t* current_pos);
void input_amount_of_instructions_to_skip(long* instructions_till_print_info, long* step);


#endif //DEBUGGER_H





























