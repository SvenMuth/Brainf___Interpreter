//
// Created by sven on 27.01.25.
//

#include <stdio.h>
#include <stdlib.h>

#include "debugger.h"
#include "colors.h"

void debug(const data_t* data)
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

    print_debug_code(data);

    constexpr char seperator[] = "-----------------------------------------------------------------------------------";
    printf(BLUE "\n\n%s" RESET_COLOR, seperator);
    print_status(data, step);
    printf(BLUE "%s\n\n" RESET_COLOR, seperator);

    print_tape_section(data);
    input_amount_of_instructions_to_skip(&instructions_till_print_info, &step);

    fseek(stdin,0,SEEK_END);
    CLEAR_SCREEN();
}

void calculate_range_of_code_to_print(int* range_negative, int* range_positive, const data_t* data)
{
    int range_neg = data->pos_orders - 200;
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

    if (range_pos > data->orders_length)
    {
        range_pos = data->orders_length;
        range_neg = range_pos - 400;
    }

    if (data->orders_length <= 400)
    {
        range_pos = data->orders_length;
        range_neg = 0;
    }

    *range_negative = range_neg;
    *range_positive = range_pos;
}

void print_debug_code(const data_t* data)
{
    printf("\n");
    printf(RED "\nDEBUG INFO\n\n" RESET_COLOR);

    // Calculate range for code output -> large file only 400 instruction shown each time
    int range_negative;
    int range_positive;
    calculate_range_of_code_to_print(&range_negative, &range_positive, data);

    printf(HI_YELLOW "%d\n" RESET_COLOR, range_negative);
    while (range_negative < range_positive)
    {
        if (data->pos_orders == range_negative)
        {
            printf(HI_RED "%c" RESET_COLOR, data->orders[range_negative]);
            range_negative++;
            continue;
        }

        printf("%c", data->orders[range_negative]);
        range_negative++;
    }
    printf(HI_YELLOW "\n%d" RESET_COLOR, range_positive);
}

void print_status(const data_t* data, const long step)
{
    printf("\nCURRENT STATUS:\n");
    printf(BLUE "STEP: " RESET_COLOR);
    printf("%ld\n\n", step);

    printf(BLUE "INSTRUCTION: " RESET_COLOR);

    switch (data->orders[data->pos_orders])
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
            data->orders[data->pos_orders]);
        exit(EXIT_FAILURE);
        break;
    }

    printf(BLUE "\nINDEX: " RESET_COLOR);
    printf("%ld", data->pos_tape);

    if (data->orders[data->pos_orders] == TOKEN_DISPLAY)
    {
        printf(HI_MAGENTA "\tVALUE: %d -> \'%c\'\n\n" RESET_COLOR,
            data->tape[data->pos_tape], (char)data->tape[data->pos_tape]);
    }
    else
    {
        printf(BLUE "\tVALUE: " RESET_COLOR);
        printf("%d\n", data->tape[data->pos_tape]);
    }
}

void print_tape_section(const data_t* data)
{
    //Try to go back five nodes
    long start = data->pos_tape - 5;
    long end = 0;
    if (start < 0)
    {
        start = 0;
    }

    end += start + 5;

    if (end >= TAPE_SIZE)
    {
        end = data->tape_length;
    }

    long tmp_start = start;
    long tmp_end = end;

    printf("Neighbors of current position on tape from [-5] to [+5] (if possible):\n");
    printf(BLUE "INDEX:\t" RESET_COLOR);

    // Print indexes
    for (; tmp_start <= tmp_end; tmp_start++)
    {
        if (data->pos_tape == tmp_start)
        {
            printf(RED "%-3ld" RESET_COLOR, tmp_start);
            printf(" | ");
        }
        else
        {
            printf("%-3ld | ", tmp_start);
        }
    }

    printf("\n");
    printf(BLUE "VALUE:\t" RESET_COLOR);

    // Print values
    for (; start <= end; start++)
    {
        if (data->tape[start] != 0)
        {
            printf(MAGENTA "%-3d" RESET_COLOR, data->tape[start]);
            printf(" | ");
        }
        else
        {
            printf("%-3d | ", data->tape[start]);
        }
    }

    printf("\n\n");
}

void input_amount_of_instructions_to_skip(long* instructions_till_print_info, long* step)
{
    printf("Input number (0-9999) of instructions to skip [\"ENTER\" for next instruction]: ");

    // Problem to fix
    char buffer[5];
    fgets(buffer, 5, stdin);

    if (buffer[0] == NEW_LINE/*ENTER_KEY*/)
    {
        *instructions_till_print_info = 0;
    }
    else
    {
        const long input = strtol(buffer, nullptr, 10);
        *instructions_till_print_info = input;
        *step += input;
    }

    printf("\n");
}































