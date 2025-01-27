//
// Created by sven on 27.01.25.
//

#include <stdio.h>
#include <stdlib.h>

#include "debugger.h"
#include "colors.h"

void debug(char_array_t file_data_as_char_array, const int current_index, node_t* current_pos)
{
    static long instructions_till_print_info = 0;
    if (instructions_till_print_info != 0)
    {
        instructions_till_print_info--;
        return;
    }

    printf("\n");
    printf(RED "\nDEBUG INFO\n\n" RESET_COLOR);

    int i = 0;
    while (i < file_data_as_char_array.length)
    {
        if (current_index == i)
        {
            printf(HI_RED "%c" RESET_COLOR, file_data_as_char_array.instructions[i]);
            i++;
            continue;
        }

        printf("%c", file_data_as_char_array.instructions[i]);
        i++;
    }

    printf(BLUE "\n\nPosition on tape now:\n" RESET_COLOR);
    printf("INDEX: %d\n", current_pos->index);

    if (file_data_as_char_array.instructions[current_index] == TOKEN_DISPLAY)
    {
        printf(HI_MAGENTA "VALUE: %d -> \'%c\'\n" RESET_COLOR,
            current_pos->value, (char)current_pos->value);
    }
    else
    {
        printf("VALUE: %d\n\n", current_pos->value);
    }

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

    printf(BLUE "Neighbors of current position on tape from [-5] to [+5] (if possible):\n" RESET_COLOR);
    printf("INDEX:  ");
    counter = 0;
    backup_tmp = tmp;
    while (counter != 10 && tmp != nullptr)
    {
        printf("%-3d | ", tmp->index);
        tmp = tmp->next;
        counter++;
    }

    printf("\n");
    printf("VALUE:  ");

    counter = 0;
    tmp = backup_tmp;
    while (counter != 10 && tmp != nullptr)
    {
        printf("%-3d | ", tmp->value);
        tmp = tmp->next;
        counter++;
    }

    printf("\n\n");

    printf("Input number (0-999) of instructions to skip [\"ENTER\" for next instruction]: ");

    char buffer[4];
    fgets(buffer, 4, stdin);
    long input = strtol(buffer, (char**)nullptr, 10);

    printf("\n");

    if (input == NEW_LINE/*ENTER_KEY*/)
    {
        instructions_till_print_info = 0;
    }
    else
    {
        instructions_till_print_info = input;
    }

    fseek(stdin,0,SEEK_END);

    CLEAR_SCREEN();
}





























