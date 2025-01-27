#include <stdio.h>
#include <stdlib.h>


#include "interpreter.h"

#define CLEAR_SCREEN() printf("\033[H\033[J")

typedef struct instructions_char_array
{
    char* instructions;
    int length;
} instructions_char_t;

instructions_char_t int_to_char_array(file_data_t file_data)
{
    char* buff = malloc(sizeof(char) * file_data.length);

    int i = 0;
    for (; i < file_data.length; i++)
    {
        buff[i] = (char)file_data.instructions[i];
    }

    instructions_char_t arr = {
        .instructions = buff,
        .length = file_data.length,
    };

    return arr;
}

void debug(instructions_char_t instructions_char, int current_index, node_t* current_pos)
{
    static int instructions_till_print_info = 0;
    if (instructions_till_print_info != 0)
    {
        instructions_till_print_info--;
        return;
    }

    printf("\n");
    int i = 0;
    for (; i < instructions_char.length;)
    {
        if (current_index == i)
        {
            printf("\033[31m%c\033[0m", instructions_char.instructions[i]);
            i++;
            continue;
        }

        printf("%c", instructions_char.instructions[i]);
        i++;
    }
    printf("\n");


    printf("\nDEBUG INFO\n");
    //printf("instruction: \'%c\'\n", file_data.instructions[index]);
    printf("index: %d\n", current_pos->index);
    printf("value: %d -> \'%c\'\n", current_pos->value, (char)current_pos->value);


    printf("Input after how many instructions info should printed again: "
           "[\"ENTER\" for following instruction]\n");

    int input = getchar();

    if (input == NEW_LINE/*ENTER_KEY*/)
    {
        instructions_till_print_info = 0;
    }
    else
    {
        instructions_till_print_info = input - '0';
    }

    fseek(stdin,0,SEEK_END);

    CLEAR_SCREEN();
}




int main(int argc, char** argv)
{
    if (argc != 2)
    {
        ERROR_PRINT("The wrong number of arguments has been passed!");
        return EXIT_FAILURE;
    }

    FILE* fp = fopen(argv[1], "r");
    if (!fp)
    {
        perror("File opening failed!");
        return EXIT_FAILURE;
    }

    file_data_t file_data = read_file_in_array(fp);
    instructions_char_t instructions_char = int_to_char_array(file_data);
    fclose(fp);

    node_t* head = init_tape();
    node_t* current_node = head;

    printf("STEP\t INSTRUCTION\t\t\tVALUE\n");
    bool is_jump_if_zero = false;
    bool is_jump_if_not_zero = false;

    char buffer_output[1000];
    int buffer_output_index = 0;

    for (int i = 0; i < file_data.length; i++)
    {
        char instruction = (char)file_data.instructions[i];

        if (instruction == NEW_LINE || instruction == SPACE_KEY)
        {
            continue;
        }

        if (is_jump_if_zero && instruction != TOKEN_JUMP_IF_NOT_ZERO)
        {
            continue;
        }

        bool is_jump_active = is_jump_if_zero || is_jump_if_not_zero;

        //return true if jump conditions are met
        const bool status_jump_execution =
            process_instruction(instruction, &current_node, is_jump_active);

        if (instruction == TOKEN_DISPLAY)
        {
            buffer_output[buffer_output_index] = (char)current_node->value;
            buffer_output_index++;
        }

        if (status_jump_execution)
        {
            if (instruction == TOKEN_JUMP_IF_ZERO)
            {
                is_jump_if_zero = true;
            }
            if (instruction == TOKEN_JUMP_IF_NOT_ZERO)
            {
                is_jump_if_not_zero = true;
            }
        }

        // Case for "jump if zero"
        if (!status_jump_execution && instruction == TOKEN_JUMP_IF_NOT_ZERO)
        {
            is_jump_if_zero = false;
        }
        // Case for "jump if not zero"
        if (!status_jump_execution && instruction == TOKEN_JUMP_IF_ZERO)
        {
            is_jump_if_not_zero = false;
        }

        if (is_jump_if_not_zero)
        {
            while (instruction != TOKEN_JUMP_IF_ZERO)
            {
                i--;
                instruction = (char)file_data.instructions[i];
            }
            // Subtract one, otherwise right token is skipped
            i--;
        }

#ifdef DEBUG
        debug(instructions_char, i, current_node);
#endif

    }
    buffer_output[buffer_output_index] = 0;
    printf("\nOutput: %s", buffer_output);

    //print_list(head);
    free_list(head);
    free(file_data.instructions);
    free(instructions_char.instructions);

    return EXIT_SUCCESS;
}





















