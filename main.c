
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

    data_t data = {
        .tape = malloc(sizeof(int) * TAPE_SIZE),
        .index_tape = 0,
        .is_jump_if_zero = false,
        .is_jump_if_not_zero = false,
        .instructions = nullptr,
        .index_instructions = 0,
        .instructions_length = 0,
        .current_instruction = 0,
    };

    if (data.tape == nullptr)
    {
        ERROR_PRINT("Memory allocation failed!");
        exit(EXIT_FAILURE);
    }
    set_array_zero(&data);

    read_file_in_array(fp, &data);
    fclose(fp);

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

    for (; data.index_instructions < data.instructions_length; data.index_instructions++)
    {
        data.current_instruction = data.instructions[data.index_instructions];

        if (data.current_instruction == NEW_LINE
            || data.current_instruction == SPACE_KEY)
        {
            continue;
        }

        if (data.is_jump_if_zero && data.current_instruction != TOKEN_JUMP_IF_NOT_ZERO)
        {
            continue;
        }

        const bool is_jump_active = data.is_jump_if_zero || data.is_jump_if_not_zero;

        //return true if jump conditions are met
        const bool status_jump_execution =
            process_instruction(&data, is_jump_active);

#ifdef DEBUG
        debug(exec_data);
#endif


        if (data.current_instruction == TOKEN_DISPLAY)
        {
#ifndef NO_LOG
            if (buffer_output_index == BUFFER_SIZE - 1)
            {
                ERROR_PRINT("Output size to big for buffer. Try with NO_LOG option!");
                exit(EXIT_FAILURE);
            }
            buffer_output[buffer_output_index] = (char)data.tape[data.index_tape];
            buffer_output_index++;
#else
            printf("%c", (char)current_node->value);
#endif
        }

        if (status_jump_execution)
        {
            if (data.current_instruction == TOKEN_JUMP_IF_ZERO)
            {
                data.is_jump_if_zero = true;
            }
            if (data.current_instruction == TOKEN_JUMP_IF_NOT_ZERO)
            {
                data.is_jump_if_not_zero = true;
            }
        }

        // Case for "jump if zero"
        if (!status_jump_execution &&
            data.current_instruction == TOKEN_JUMP_IF_NOT_ZERO)
        {
            data.is_jump_if_zero = false;
        }
        // Case for "jump if not zero"
        if (!status_jump_execution &&
            data.current_instruction == TOKEN_JUMP_IF_ZERO)
        {
            data.is_jump_if_not_zero = false;
        }

        if (data.is_jump_if_not_zero)
        {
            while (data.current_instruction != TOKEN_JUMP_IF_ZERO)
            {
                data.index_instructions--;
                data.current_instruction = data.instructions[data.index_instructions];
            }
            // Subtract one, otherwise right token is skipped
            data.index_instructions--;
        }

    }

#ifndef NO_LOG
    buffer_output[buffer_output_index] = 0;
    printf("\nOutput: %s", buffer_output);
#endif

    free(data.instructions);
    return EXIT_SUCCESS;
}






















