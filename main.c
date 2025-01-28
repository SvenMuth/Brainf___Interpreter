
#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"
#include "debugger.h"

#define BUFFER_SIZE 1000


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

    char_array_t file_data_as_char_array = read_file_in_array(fp);
    fclose(fp);

    node_t* head = init_tape();
    node_t* current_node = head;

#if !defined(DEBUG) && !defined(NO_LOG)
    printf("STEP\t INSTRUCTION\t\t\tVALUE\n");
#endif

    bool is_jump_if_zero = false;
    bool is_jump_if_not_zero = false;

    // When NO_LOG defined print the output directly without any logging
#ifndef NO_LOG
    char buffer_output[BUFFER_SIZE];
    int buffer_output_index = 0;
#else
    printf("Output: ");
#endif

    for (int i = 0; i < file_data_as_char_array.length; i++)
    {
        char instruction = file_data_as_char_array.instructions[i];

        if (instruction == NEW_LINE || instruction == SPACE_KEY)
        {
            continue;
        }

        if (is_jump_if_zero && instruction != TOKEN_JUMP_IF_NOT_ZERO)
        {
            continue;
        }

        const bool is_jump_active = is_jump_if_zero || is_jump_if_not_zero;

        //return true if jump conditions are met
        const bool status_jump_execution =
            process_instruction(instruction, &current_node, is_jump_active);

#ifdef DEBUG
        debug(file_data_as_char_array, i, current_node);
#endif


        if (instruction == TOKEN_DISPLAY)
        {
#ifndef NO_LOG
            if (buffer_output_index == BUFFER_SIZE - 1)
            {
                ERROR_PRINT("Output size to big for buffer. Try with NO_LOG option!");
                exit(EXIT_FAILURE);
            }
            buffer_output[buffer_output_index] = (char)current_node->value;
            buffer_output_index++;
#else
            printf("%c", (char)current_node->value);
#endif
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
                instruction = file_data_as_char_array.instructions[i];
            }
            // Subtract one, otherwise right token is skipped
            i--;
        }

    }

#ifndef NO_LOG
    buffer_output[buffer_output_index] = 0;
    printf("\nOutput: %s", buffer_output);
#endif

    //print_list(head);
    free_list(head);
    free(file_data_as_char_array.instructions);

    return EXIT_SUCCESS;
}





















