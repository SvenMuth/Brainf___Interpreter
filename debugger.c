//
// Created by sven on 27.01.25.
//

#include <stdio.h>
#include <stdlib.h>

#include "debugger.h"
#include "colors.h"

void debug(debug_info_t debug_info)
{
    // How many steps since start
    static long step = 0;

    // Skip instructions if number is set
    static long instructions_till_print_info = 0;
    if (instructions_till_print_info != 0)
    {
        instructions_till_print_info--;
        return;
    }
    step++;

    print_debug_code(debug_info);
    print_status(debug_info, step);
    print_tape_section(debug_info.current_pos);
    input_amount_of_instructions_to_skip(&instructions_till_print_info, &step);

    fseek(stdin,0,SEEK_END);
    CLEAR_SCREEN();
}

void calculate_range_of_code_to_print(int* range_negative, int* range_positive, debug_info_t debug_info)
{
    int range_neg = debug_info.current_index - 200;
    int range_pos = 0;

    if (range_neg < 0)
    {
        range_neg = 0;
        range_pos = 400;
    }
    else
    {
        range_pos = range_neg + 400;
    }

    if (range_pos > debug_info.instructions_length)
    {
        range_pos = debug_info.instructions_length;
        range_neg = range_pos - 400;
    }

    if (debug_info.instructions_length <= 400)
    {
        range_pos = debug_info.instructions_length;
        range_neg = 0;
    }

    *range_negative = range_neg;
    *range_positive = range_pos;
}

void print_debug_code(debug_info_t debug_info)
{
    printf("\n");
    printf(RED "\nDEBUG INFO\n\n" RESET_COLOR);

    // Calculate range for code output -> large file only 400 instruction shown each time
    int range_negative;
    int range_positive;
    calculate_range_of_code_to_print(&range_negative, &range_positive, debug_info);

    printf(YELLOW "%d\n" RESET_COLOR, range_negative);
    while (range_negative < range_positive)
    {
        if (debug_info.current_index == range_negative)
        {
            printf(HI_RED "%c" RESET_COLOR, debug_info.instructions[range_negative]);
            range_negative++;
            continue;
        }

        printf("%c", debug_info.instructions[range_negative]);
        range_negative++;
    }
    printf(YELLOW "\n%d" RESET_COLOR, range_positive);
}

void print_status(debug_info_t debug_info, const long step)
{
    printf(RED "\n\nCURRENT STATUS:\n" RESET_COLOR);
    printf(BLUE "STEP: " RESET_COLOR);
    printf("%li\n\n", step);

    printf(BLUE "INSTRUCTION: " RESET_COLOR);

    switch (debug_info.instructions[debug_info.current_index])
    {

    case TOKEN_MOVE_RIGHT: printf("MOVE RIGHT"); break;
    case TOKEN_MOVE_LEFT: printf("MOVE LEFT"); break;
    case TOKEN_DISPLAY: printf("DISPLAY"); break;
    case TOKEN_READ: printf("READ"); break;
    case TOKEN_ADD_ONE: printf("ADD"); break;
    case TOKEN_SUBTRACT_ONE: printf("SUB"); break;
    case TOKEN_JUMP_IF_ZERO: printf("JUMP IF ZERO"); break;
    case TOKEN_JUMP_IF_NOT_ZERO: printf("JUMP IF NOT ZERO"); break;
    default:
        ERROR_PRINT("Invalid instruction occurred! -> \'%d\'",
            debug_info.instructions[debug_info.current_index]);
        exit(EXIT_FAILURE);
        break;
    }

    printf(BLUE "\nINDEX: " RESET_COLOR);
    printf("%d", debug_info.current_pos->index);

    if (debug_info.instructions[debug_info.current_index] == TOKEN_DISPLAY)
    {
        printf(HI_MAGENTA "\tVALUE: %d -> \'%c\'\n\n" RESET_COLOR,
            debug_info.current_pos->value, (char)debug_info.current_pos->value);
    }
    else
    {
        printf(BLUE "\tVALUE: " RESET_COLOR);
        printf("%d\n\n", debug_info.current_pos->value);
    }
}

void print_tape_section(node_t* current_pos)
{
    //Try to go back five nodes
    int counter = 0;
    node_t* tmp = current_pos;
    node_t* backup_tmp = nullptr;
    while (tmp != nullptr)
    {
        if (counter == 5)
        {
            break;
        }
        counter++;

        backup_tmp = tmp;
        tmp = tmp->prev;
        if (tmp == nullptr)
        {
            tmp = backup_tmp;
            break;
        }
    }

    printf("Neighbors of current position on tape from [-5] to [+5] (if possible):\n");
    printf(BLUE "INDEX:\t" RESET_COLOR);

    // Print indexes
    counter = 0;
    backup_tmp = tmp;
    while (counter != 11 && tmp != nullptr)
    {
        if (current_pos->index == tmp->index)
        {
            printf(RED "%-3d" RESET_COLOR, tmp->index);
            printf(" | ");
        }
        else
        {
            printf("%-3d | ", tmp->index);
        }

        tmp = tmp->next;
        counter++;
    }

    printf("\n");
    printf(BLUE "VALUE:\t" RESET_COLOR);

    // Print values
    counter = 0;
    tmp = backup_tmp;
    while (counter != 11 && tmp != nullptr)
    {
        if (tmp->value != 0)
        {
            printf(MAGENTA "%-3d" RESET_COLOR, tmp->value);
            printf(" | ");
        }
        else
        {
            printf("%-3d | ", tmp->value);
        }

        tmp = tmp->next;
        counter++;
    }

    printf("\n\n");
}

void input_amount_of_instructions_to_skip(long* instructions_till_print_info, long* step)
{
    printf("Input number (0-9999) of instructions to skip [\"ENTER\" for next instruction]: ");

    // Problem to fix
    char buffer[5];
    fgets(buffer, 5, stdin);

    printf("\n");

    if (buffer[0] == NEW_LINE/*ENTER_KEY*/)
    {
        *instructions_till_print_info = 0;
    }
    else
    {
        long input = strtol(buffer, nullptr, 10);
        *instructions_till_print_info = input;
        *step += input;
    }
}































