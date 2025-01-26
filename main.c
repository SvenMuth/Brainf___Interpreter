#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        error_print("The wrong number of arguments has been passed!");
        return EXIT_FAILURE;
    }

    FILE* fp = fopen(argv[1], "r");
    if (!fp)
    {
        perror("File opening failed!");
        return EXIT_FAILURE;
    }

    file_data_t file_data = read_file_in_array(fp);
    fclose(fp);

    node_t* head = init_tape();

    printf("STEP\t INSTRUCTION    VALUE\n");
    bool is_jump_if_zero = false;
    bool is_jump_if_not_zero = false;

    for (int i = 0; i < file_data.index; i++)
    {
        char instruction = (char)file_data.data[i];

        if (is_jump_if_zero && instruction != TOKEN_JUMP_IF_NOT_ZERO)
        {
            continue;
        }

        bool is_jump_active = is_jump_if_zero || is_jump_if_not_zero;

        //return true if jump conditions are met
        const bool status_jump_execution =
            process_instruction(instruction, &head, is_jump_active);

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
                instruction = (char)file_data.data[i];
            }
            // Subtract one, otherwise right token is skipped
            i--;
        }

    }

    //print_list(head);
    free_list(head);
    free(file_data.data);

    return EXIT_SUCCESS;
}





















