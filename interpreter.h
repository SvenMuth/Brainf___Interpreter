//
// Created by sven on 25.01.25.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

//#define TAPE_SIZE                 6000000000 //For executing Mandelbrot.bf (Interpreter to slow currently)
#define TAPE_SIZE                 30000 //Fixed tape size for list
#define STEP_SIZE                 1000  //Defines in which step the size of file_data_size is increased
#define BUFFER_SIZE               1000  //Output buffer

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

typedef enum RUNNING_MODE
{
    DEBUG,
    NO_LOG,
    DEFAULT,
}RUNNING_MODE;

static const char* MODE_STR[] = {
    [DEBUG] = "-DEBUG",
    [NO_LOG] = "-NO_LOG"
};

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
    u_int8_t* tape;
    long pos_tape;
    long tape_length;

    bool is_jump_if_zero;
    bool is_jump_if_not_zero;

    char* orders;
    int pos_orders;
    int orders_length;
}data_t;


void int_to_char_array(int_array_t file_data, data_t* exec_data);

void calculate_size(int_array_t* file_data);
void allocate_space(int_array_t* file_data);
void realloc_space(int_array_t* file_data);
void read_file_in_array(FILE* fp, data_t* exec_data);
void set_array_zero(const data_t* data);

void log_execution(char instruction, const char* message,
    long value, RUNNING_MODE running_mode);

bool process_instruction(data_t* data, bool is_jump_active,
    RUNNING_MODE running_mode);

void move_right(data_t* data);
void move_left(data_t* data);
void read(const data_t* data);
void add(const data_t* data);
void subtract(const data_t* data);

bool jump_if_zero(const data_t* data, bool is_jump_active);
bool jump_if_not_zero(const data_t* data, bool is_jump_active);

FILE* open_file(const char* filename);

void print_log_header(RUNNING_MODE running_mode);
void initialize_exec_data(data_t* data);

bool run_jump_if_zero(const data_t* data);
void run_jump_if_not_zero(data_t* data);
void reset_is_jump(data_t* data, bool status_jump_execution);
void set_is_jump(data_t* data, bool status_jump_execution);

#endif //INTERPRETER_H












