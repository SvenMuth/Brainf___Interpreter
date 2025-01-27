//
// Created by sven on 25.01.25.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "list.h"

#define TAPE_SIZE                 30000 // Fixed tape size for list
#define STEP_SIZE                 1000  //Defines in which step the size of file_data_size is increased

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

#define ERROR_PRINT(args ...)     fprintf(stderr, args)


typedef struct char_array
{
    char* instructions;
    int length;
} char_array_t;

typedef struct file_data
{
    int* instructions;
    int size;
    int length;

    const int step;
    int multiplicator;
} file_data_t;

char_array_t int_to_char_array(file_data_t file_data_int_array);

void calculate_size(file_data_t* file_data);
void allocate_space(file_data_t* file_data);
void realloc_space(file_data_t* file_data);
char_array_t read_file_in_array(FILE* fp);
void clear_char_buffer(char* buffer);

void log_execution(char instruction, const char* message, const int value);

node_t* init_tape();
bool process_instruction(char instruction, node_t** current_pos, bool is_jump_active);

void move_right(node_t** current_pos);
void move_left(node_t** current_pos);
void read(node_t* current_pos);
void add(node_t* current_pos);
void subtract(node_t* current_pos);

bool jump_if_zero(node_t* current_pos, bool is_jump_active);
bool jump_if_not_zero(node_t* current_pos, bool is_jump_active);

#endif //INTERPRETER_H
