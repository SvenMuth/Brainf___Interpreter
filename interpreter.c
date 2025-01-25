//
// Created by sven on 25.01.25.
//

#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"


void log_execution(const char instruction, const char* message, const int value)
{
    //TODO: Implement better format specifiers
    static int counter = 1;
    if (instruction == TOKEN_ADD_ONE)
    {
        printf("(%d) \t \'%c\' %-10s [%d -> %d]\n",
            counter, instruction, message, (value - 1), value);
    }
    else if (instruction == TOKEN_SUBTRACT_ONE)
    {
        printf("(%d) \t \'%c\' %-10s [%d -> %d]\n",
            counter, instruction, message, (value + 1), value);
    }
    else
    {
        printf("(%d) \t \'%c\' %-10s %d\n",
            counter, instruction, message, value);
    }

    counter++;
}

bool process_instruction(const char instruction, node_t** current_pos)
{
    switch (instruction)
    {
    case TOKEN_MOVE_RIGHT:
        move_right(current_pos);
        log_execution(TOKEN_MOVE_RIGHT,
            "array pos", (*current_pos)->index);
        break;

    case TOKEN_MOVE_LEFT:
        move_left(current_pos);
        log_execution(TOKEN_MOVE_LEFT,
            "array pos", (*current_pos)->index);
        break;

    case TOKEN_DISPLAY:
        log_execution(TOKEN_DISPLAY,
            "output", (*current_pos)->value);
        break;

    case TOKEN_READ:
        read(*current_pos);
        log_execution(TOKEN_READ,
            "read in", (*current_pos)->value);
        break;

    case TOKEN_ADD_ONE:
        add(*current_pos);
        log_execution(TOKEN_ADD_ONE,
            "add", (*current_pos)->value);
        break;

    case TOKEN_SUBTRACT_ONE:
        subtract(*current_pos);
        log_execution(TOKEN_SUBTRACT_ONE,
            "subtract", (*current_pos)->value);
        break;

    case TOKEN_JUMP_IF_ZERO:
        if (!jump_if_zero(current_pos)) return false;

        log_execution(TOKEN_JUMP_IF_ZERO,
            "jump to", (*current_pos)->index);
        break;

    case TOKEN_JUMP_IF_NOT_ZERO:
        if (!jump_if_not_zero(current_pos)) return false;

        log_execution(TOKEN_JUMP_IF_NOT_ZERO,
            "jump to", (*current_pos)->index);
        break;

    default:
        fprintf(stderr, "Invalid instruction occurred! -> \'%c\'", instruction);
        exit(EXIT_FAILURE);
        break;
    }

    return true;
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
        node_t* tmp = create_new_node(0);
        insert_after_node(*current_pos, tmp);
    }
    *current_pos = (*current_pos)->next;
}

void move_left(node_t** current_pos)
{
    if ((*current_pos)->prev == nullptr)
    {
        node_t* tmp = create_new_node(0);
        insert_at_head(current_pos, tmp);
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

bool jump_if_zero(node_t** current_pos)
{
    if ((*current_pos)->value == 0)
    {
        node_t* tmp = *current_pos;
        while (tmp != nullptr)
        {
            if (tmp->value == TOKEN_JUMP_IF_NOT_ZERO)
            {
                break;
            }
            tmp = tmp->next;

            if (tmp == nullptr)
            {
                return false;
            }
        }
        *current_pos = tmp;
    }

    return true;
}

bool jump_if_not_zero(node_t** current_pos)
{
    if ((*current_pos)->value != 0)
    {
        node_t* tmp = *current_pos;
        while (tmp != nullptr)
        {
            if (tmp->value == TOKEN_JUMP_IF_ZERO)
            {
                break;
            }
            tmp = tmp->prev;

            if (tmp == nullptr)
            {
                return false;
            }
        }
        *current_pos = tmp;
    }
    return true;
}
