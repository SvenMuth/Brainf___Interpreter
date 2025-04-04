//
// Created by sven on 27.01.25.
//


#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "interpreter.h"

#define CLEAR_SCREEN()      printf("\033[H\033[J")


void debug(const data_t* data);

void calculate_range_of_code_to_print(int* range_neg, int* range_pos, const data_t* data);
void print_debug_code(const data_t* data);
void print_status(const data_t* data, long step);
void print_tape_section(const data_t* data);
void input_amount_of_instructions_to_skip(long* instructions_till_print_info, long* step);


#endif //DEBUGGER_H





























