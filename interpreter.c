//
// Created by sven on 25.01.25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"

#ifndef DEBUG
#include "colors.h"
#endif


void int_to_char_array(int_array_t file_data, data_t* exec_data)
{

    *exec_data = (data_t){
        .instructions = malloc(sizeof(char) * file_data.length),
        .instructions_length = file_data.length,
    };

    if (exec_data->instructions == nullptr)
    {
        ERROR_PRINT("Memory allocation failed!");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < file_data.length; i++)
    {
        exec_data->instructions[i] = (char)file_data.instructions[i];
    }
}

void calculate_size(int_array_t* file_data)
{
    file_data->size = file_data->step * file_data->multiplicator;
    file_data->multiplicator++;
}

void allocate_space(int_array_t* file_data)
{
    calculate_size(file_data);

    file_data->instructions = malloc(sizeof(int) * file_data->size);
    if (file_data->instructions == nullptr)
    {
        ERROR_PRINT("Memory allocation failed!");
        exit(EXIT_FAILURE);
    }
}

void realloc_space(int_array_t* file_data)
{
    calculate_size(file_data);

    file_data->instructions = realloc(file_data->instructions,sizeof(int) * file_data->size);
    if (file_data->instructions == nullptr)
    {
        ERROR_PRINT("Realloc failed!");
        exit(EXIT_FAILURE);
    }
}

void read_file_in_array(FILE* fp, data_t* exec_data)
{
    int_array_t file_data = {
        .length = 0,
        .multiplicator = 1,
        .step = STEP_SIZE,
    };
    allocate_space(&file_data);

    int c;
    while ((c = fgetc(fp)) != EOF)
    {
        // When writing directly to a char array the file pointer was getting corrupted
        file_data.instructions[file_data.length] = c;
        file_data.length++;

        if ((file_data.length + 1) % STEP_SIZE == 0)
        {
            realloc_space(&file_data);
        }
    }

    // That is why conversion is done here
    int_to_char_array(file_data, exec_data);
    free(file_data.instructions);
}

void clear_char_buffer(char* buffer)
{
    for (int i = 0; i < strlen(buffer); i++)
    {
        buffer[i] = 0;
    }
}

void log_execution(const char instruction, const char* message, const int value)
{
#if defined(DEBUG) || defined(NO_LOG)
    //Dont print log while debugging or no_log
#else
    //TODO: Implement better format specifiers
    static int counter = 1;
    if (instruction == TOKEN_ADD_ONE || instruction == TOKEN_MOVE_RIGHT)
    {
        printf("(%d) \t \'%c\' %-18s [%d -> %d]\n",
            counter, instruction, message, (value - 1), value);
    }
    else if (instruction == TOKEN_SUBTRACT_ONE || instruction == TOKEN_MOVE_LEFT)
    {
        printf("(%d) \t \'%c\' %-18s [%d -> %d]\n",
            counter, instruction, message, (value + 1), value);
    }
    else if (instruction == TOKEN_DISPLAY)
    {
        printf(HI_MAGENTA "(%d) \t \'%c\' %-18s [%d -> \'%c\']\n" RESET_COLOR,
            counter, instruction, message, value, (char)value);
    }
    else
    {
        printf("(%d) \t \'%c\' %-18s %d\n",
            counter, instruction, message, value);
    }

    counter++;
#endif
}

node_t* init_tape()
{
    node_t* head = nullptr;
    static int index = TAPE_SIZE;

    for (int i = 0; i <= TAPE_SIZE; i++)
    {
        node_t* tmp = create_new_node(0);
        tmp->index = index;
        index--;

        insert_at_head(&head, tmp);
    }

    //print_list(head);
    return head;
}

bool process_instruction(data_t* exec_data, const bool is_jump_active)
{
    switch (exec_data->current_instruction)
    {
    case TOKEN_MOVE_RIGHT:
        move_right(exec_data->current_pos);
        log_execution(TOKEN_MOVE_RIGHT,
            "[moved right]", (*exec_data->current_pos)->index);
        break;

    case TOKEN_MOVE_LEFT:
        move_left(exec_data->current_pos);
        log_execution(TOKEN_MOVE_LEFT,
            "[moved left]", (*exec_data->current_pos)->index);
        break;

    case TOKEN_DISPLAY:
        log_execution(TOKEN_DISPLAY,
            "[output]", (*exec_data->current_pos)->value);
        break;

    case TOKEN_READ:
        read(*exec_data->current_pos);
        log_execution(TOKEN_READ,
            "[read in]", (*exec_data->current_pos)->value);
        break;

    case TOKEN_ADD_ONE:
        add(*exec_data->current_pos);
        log_execution(TOKEN_ADD_ONE,
            "[add]", (*exec_data->current_pos)->value);
        break;

    case TOKEN_SUBTRACT_ONE:
        subtract(*exec_data->current_pos);
        log_execution(TOKEN_SUBTRACT_ONE,
            "[subtract]", (*exec_data->current_pos)->value);
        break;

    case TOKEN_JUMP_IF_ZERO:
        const bool result_if_zero = jump_if_zero(*exec_data->current_pos, is_jump_active);
        if (result_if_zero)
        {
            log_execution(TOKEN_JUMP_IF_ZERO,
            "[jump start]", (*exec_data->current_pos)->index);
            return true;
        }
        if (!result_if_zero && is_jump_active)
        {
            log_execution(TOKEN_JUMP_IF_ZERO,
            "[r jump end]", (*exec_data->current_pos)->index);
        }

        break;

    case TOKEN_JUMP_IF_NOT_ZERO:
        const bool result_if_not_zero = jump_if_not_zero(*exec_data->current_pos, is_jump_active);
        if (result_if_not_zero)
        {
            log_execution(TOKEN_JUMP_IF_NOT_ZERO,
            "[r jump start]", (*exec_data->current_pos)->index);
            return true;
        }
        if (!result_if_not_zero && is_jump_active)
        {
            log_execution(TOKEN_JUMP_IF_NOT_ZERO,
            "[jump end]", (*exec_data->current_pos)->index);
        }
        break;

    default:
        ERROR_PRINT("Invalid instruction occurred! -> \'%d\'", exec_data->current_instruction);
        exit(EXIT_FAILURE);
        break;
    }

    return false;
}

void move_right(node_t** current_pos)
{
    if ((*current_pos)->next == nullptr)
    {
        ERROR_PRINT("Next node is nullptr! [INDEX: %d]", (*current_pos)->index);
        exit(EXIT_FAILURE);
    }
    *current_pos = (*current_pos)->next;
}

void move_left(node_t** current_pos)
{
    if ((*current_pos)->prev == nullptr)
    {
        ERROR_PRINT("Previous node is nullptr! [INDEX: %d]", (*current_pos)->index);
        exit(EXIT_FAILURE);
    }
    *current_pos = (*current_pos)->prev;
}

void read(node_t* current_pos)
{
    printf("Input value: ");
    current_pos->value = getchar() - '0';

    fseek(stdin,0,SEEK_END);
}

void add(node_t* current_pos)
{
    current_pos->value += 1;
}

void subtract(node_t* current_pos)
{
    current_pos->value -= 1;
}

bool jump_if_zero(node_t* current_pos, const bool is_jump_active)
{
    if (is_jump_active)
    {
        return false;
    }

    if (current_pos->value == 0)
    {
        return true;
    }
    return false;
}

bool jump_if_not_zero(node_t* current_pos, const bool is_jump_active)
{
    if (is_jump_active)
    {
        return false;
    }

    if (current_pos->value != 0)
    {
        return true;
    }
    return false;
}














