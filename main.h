//
// Created by sven on 24.01.25.
//

#ifndef MAIN_H
#define MAIN_H

#define TOKEN_MOVE_RIGHT          '>'
#define TOKEN_MOVE_LEFT           '<'
#define TOKEN_DISPLAY             '.'
#define TOKEN_READ                ','
#define TOKEN_ADD_ONE             '+'
#define TOKEN_SUBTRACT_ONE        '-'
#define TOKEN_JUMP_IF_ZERO        '['
#define TOKEN_JUMP_IF_NOT_ZERO    ']'
#define SPACE_KEY                 ' '

#include "list.h"

void log_program(const char* message);
void process_instructions(const char instruction, node_t** current_pos);
void clear_stdin();

void move_right(node_t** current_pos);
void move_left(node_t** current_pos);
void display(const node_t* current_pos);
void read(node_t* current_pos);
void add(node_t* current_pos);
void subtract(node_t* current_pos);

void jump_if_zero(node_t** current_pos);
void jump_if_not_zero(node_t** current_pos);

#endif //MAIN_H
