//
// Created by sven on 25.01.25.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#define TOKEN_MOVE_RIGHT          '>'   //62
#define TOKEN_MOVE_LEFT           '<'   //60
#define TOKEN_DISPLAY             '.'   //46
#define TOKEN_READ                ','   //44
#define TOKEN_ADD_ONE             '+'   //43
#define TOKEN_SUBTRACT_ONE        '-'   //45
#define TOKEN_JUMP_IF_ZERO        '['   //91
#define TOKEN_JUMP_IF_NOT_ZERO    ']'   //93
#define SPACE_KEY                 ' '   //32
#define NEW_LINE                  '\n'  //10

#include "list.h"


void log_program(const char instruction, const char* message, const int value);

bool process_instruction(const char instruction, node_t** current_pos);
void clear_stdin();

void move_right(node_t** current_pos);
void move_left(node_t** current_pos);
void read(node_t* current_pos);
void add(node_t* current_pos);
void subtract(node_t* current_pos);

bool jump_if_zero(node_t** current_pos);
bool jump_if_not_zero(node_t** current_pos);

#endif //INTERPRETER_H
