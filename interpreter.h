//
// Created by sven on 25.01.25.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

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


typedef struct int_array
{
    int* instructions;
    int size;
    int length;

    const int step;
    int multiplicator;
}int_array_t;

typedef struct data
{
    int* tape;
    int index_tape;

    bool is_jump_if_zero;
    bool is_jump_if_not_zero;

    char* instructions;
    int index_instructions;
    int instructions_length;
    char current_instruction;
}data_t;

void int_to_char_array(int_array_t file_data, data_t* exec_data);

void calculate_size(int_array_t* file_data);
void allocate_space(int_array_t* file_data);
void realloc_space(int_array_t* file_data);
void read_file_in_array(FILE* fp, data_t* exec_data);
void set_array_zero(data_t* data);

void log_execution(char instruction, const char* message, int value);

bool process_instruction(data_t* data, bool is_jump_active);

void move_right(data_t* data);
void move_left(data_t* data);
void read(data_t* data);
void add(data_t* data);
void subtract(data_t* data);

bool jump_if_zero(data_t* data, bool is_jump_active);
bool jump_if_not_zero(data_t* data, bool is_jump_active);

#endif //INTERPRETER_H












