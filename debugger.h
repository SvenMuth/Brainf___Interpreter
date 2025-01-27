//
// Created by sven on 27.01.25.
//



#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "interpreter.h"

#define CLEAR_SCREEN()      printf("\033[H\033[J")

void debug(char_array_t file_data_as_char_array, int current_index, node_t* current_pos);

#endif //DEBUGGER_H





























