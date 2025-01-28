
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

    data_t exec_data = {
        .current_pos = nullptr,
        .is_jump_if_zero = false,
        .is_jump_if_not_zero = false,
        .instructions = nullptr,
        .current_index = 0,
        .instructions_length = 0,
        .current_instruction = 0,
    };

    read_file_in_array(fp, &exec_data);
    fclose(fp);

    node_t* head = init_tape();
    exec_data.current_pos = &head;

#if !defined(DEBUG) && !defined(NO_LOG)
    printf("STEP\t INSTRUCTION\t\t\tVALUE\n");
#endif

    // When NO_LOG defined print the output directly without any logging
#ifndef NO_LOG
    char buffer_output[BUFFER_SIZE];
    int buffer_output_index = 0;
#else
    printf("Output: ");
#endif

    for (; exec_data.current_index < exec_data.instructions_length; exec_data.current_index++)
    {
        exec_data.current_instruction = exec_data.instructions[exec_data.current_index];

        if (exec_data.current_instruction == NEW_LINE
            || exec_data.current_instruction == SPACE_KEY)
        {
            continue;
        }

        if (exec_data.is_jump_if_zero && exec_data.current_instruction != TOKEN_JUMP_IF_NOT_ZERO)
        {
            continue;
        }

        const bool is_jump_active = exec_data.is_jump_if_zero || exec_data.is_jump_if_not_zero;

        //return true if jump conditions are met
        const bool status_jump_execution =
            process_instruction(&exec_data, is_jump_active);

#ifdef DEBUG
        debug(exec_data);
#endif


        if (exec_data.current_instruction == TOKEN_DISPLAY)
        {
#ifndef NO_LOG
            if (buffer_output_index == BUFFER_SIZE - 1)
            {
                ERROR_PRINT("Output size to big for buffer. Try with NO_LOG option!");
                exit(EXIT_FAILURE);
            }
            buffer_output[buffer_output_index] = (char)(*exec_data.current_pos)->value;
            buffer_output_index++;
#else
            printf("%c", (char)current_node->value);
#endif
        }

        if (status_jump_execution)
        {
            if (exec_data.current_instruction == TOKEN_JUMP_IF_ZERO)
            {
                exec_data.is_jump_if_zero = true;
            }
            if (exec_data.current_instruction == TOKEN_JUMP_IF_NOT_ZERO)
            {
                exec_data.is_jump_if_not_zero = true;
            }
        }

        // Case for "jump if zero"
        if (!status_jump_execution &&
            exec_data.current_instruction == TOKEN_JUMP_IF_NOT_ZERO)
        {
            exec_data.is_jump_if_zero = false;
        }
        // Case for "jump if not zero"
        if (!status_jump_execution &&
            exec_data.current_instruction == TOKEN_JUMP_IF_ZERO)
        {
            exec_data.is_jump_if_not_zero = false;
        }

        if (exec_data.is_jump_if_not_zero)
        {
            while (exec_data.current_instruction != TOKEN_JUMP_IF_ZERO)
            {
                exec_data.current_index--;
                exec_data.current_instruction = exec_data.instructions[exec_data.current_index];
            }
            // Subtract one, otherwise right token is skipped
            exec_data.current_index--;
        }

    }

#ifndef NO_LOG
    buffer_output[buffer_output_index] = 0;
    printf("\nOutput: %s", buffer_output);
#endif

    //print_list(head);
    free_list(head);
    free(exec_data.instructions);

    return EXIT_SUCCESS;
}






















