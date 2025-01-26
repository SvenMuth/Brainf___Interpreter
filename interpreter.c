//
// Created by sven on 25.01.25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"


void calculate_size(file_data_t* file_data)
{
    file_data->size = file_data->step  * file_data->multiplicator;
    file_data->multiplicator++;
}

void allocate_space(file_data_t* file_data)
{
    calculate_size(file_data);

    file_data->data = malloc(sizeof(int) * file_data->size);
    if (file_data->data == nullptr)
    {
        ERROR_PRINT("Memory allocation failed!");
        exit(EXIT_FAILURE);
    }
}

void realloc_space(file_data_t* file_data)
{
    calculate_size(file_data);

    file_data->data = realloc(file_data->data,sizeof(int) * file_data->size);
    if (file_data->data == nullptr)
    {
        ERROR_PRINT("Realloc failed!");
        exit(EXIT_FAILURE);
    }
}

file_data_t read_file_in_array(FILE* fp)
{
    file_data_t file_data = {
        .index = 0,
        .multiplicator = 1,
        .step = STEP_SIZE,
    };
    allocate_space(&file_data);

    int c;
    while ((c = fgetc(fp)) != EOF)
    {
        if (c == NEW_LINE || c == SPACE_KEY)
        {
            continue;
        }

        //Add everything to buffer
        file_data.data[file_data.index] = c;
        file_data.index++;

        if ((file_data.index + 1) % STEP_SIZE == 0)
        {
            realloc_space(&file_data);
        }
    }

    return file_data;
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
        printf("(%d) \t \'%c\' %-18s [%d -> \'%c\']\n",
            counter, instruction, message, value, (char)value);
    }
    else
    {
        printf("(%d) \t \'%c\' %-18s %d\n",
            counter, instruction, message, value);
    }

    counter++;
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

bool process_instruction(const char instruction, node_t** current_pos, const bool is_jump_active)
{
    switch (instruction)
    {
    case TOKEN_MOVE_RIGHT:
        move_right(current_pos);
        log_execution(TOKEN_MOVE_RIGHT,
            "[moved right]", (*current_pos)->index);
        break;

    case TOKEN_MOVE_LEFT:
        move_left(current_pos);
        log_execution(TOKEN_MOVE_LEFT,
            "[moved left]", (*current_pos)->index);
        break;

    case TOKEN_DISPLAY:
        log_execution(TOKEN_DISPLAY,
            "[output]", (*current_pos)->value);
        break;

    case TOKEN_READ:
        read(*current_pos);
        log_execution(TOKEN_READ,
            "[read in]", (*current_pos)->value);
        break;

    case TOKEN_ADD_ONE:
        add(*current_pos);
        log_execution(TOKEN_ADD_ONE,
            "[add]", (*current_pos)->value);
        break;

    case TOKEN_SUBTRACT_ONE:
        subtract(*current_pos);
        log_execution(TOKEN_SUBTRACT_ONE,
            "[subtract]", (*current_pos)->value);
        break;

    case TOKEN_JUMP_IF_ZERO:
        const bool result_if_zero = jump_if_zero(current_pos, is_jump_active);
        if (result_if_zero)
        {
            log_execution(TOKEN_JUMP_IF_ZERO,
            "[jump start]", (*current_pos)->index);
            return true;
        }
        if (!result_if_zero && is_jump_active)
        {
            log_execution(TOKEN_JUMP_IF_ZERO,
            "[r jump end]", (*current_pos)->index);
        }

        break;

    case TOKEN_JUMP_IF_NOT_ZERO:
        const bool result_if_not_zero = jump_if_not_zero(current_pos, is_jump_active);
        if (result_if_not_zero)
        {
            log_execution(TOKEN_JUMP_IF_NOT_ZERO,
            "[r jump start]", (*current_pos)->index);
            return true;
        }
        if (!result_if_not_zero && is_jump_active)
        {
            log_execution(TOKEN_JUMP_IF_NOT_ZERO,
            "[jump end]", (*current_pos)->index);
        }
        break;

    default:
        ERROR_PRINT("Invalid instruction occurred! -> \'%d\'", instruction);
        exit(EXIT_FAILURE);
        break;
    }

    return false;
}

void clear_stdin()
{
    while (true)
    {
        int c = getchar();
        if (c == EOF || c == '\n')
        {
            break;
        }
    }
}

void move_right(node_t** current_pos)
{
    if ((*current_pos)->next == nullptr)
    {
        ERROR_PRINT("Current node is nullptr!");
        exit(EXIT_FAILURE);
    }
    *current_pos = (*current_pos)->next;
}

void move_left(node_t** current_pos)
{
    if ((*current_pos)->prev == nullptr)
    {
        ERROR_PRINT("Current node is nullptr!");
        exit(EXIT_FAILURE);
    }
    *current_pos = (*current_pos)->prev;
}

void read(node_t* current_pos)
{
    printf("Input value: ");
    current_pos->value = getchar() - '0';

    clear_stdin();
}

void add(node_t* current_pos)
{
    current_pos->value += 1;
}

void subtract(node_t* current_pos)
{
    current_pos->value -= 1;
}

bool jump_if_zero(node_t** current_pos, const bool is_jump_active)
{
    if (is_jump_active)
    {
        return false;
    }

    if ((*current_pos)->value == 0)
    {
        return true;
    }
    return false;
}

bool jump_if_not_zero(node_t** current_pos, const bool is_jump_active)
{
    if (is_jump_active)
    {
        return false;
    }

    if ((*current_pos)->value != 0)
    {
        return true;
    }
    return false;
}














