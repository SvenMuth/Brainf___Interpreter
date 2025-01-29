
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"
#include "debugger.h"

int main(int argc, char** argv)
{
    const char* filename;
    RUNNING_MODE running_mode = DEFAULT;

    if (argc == 2)
    {
        filename = argv[1];
    }
    else if (argc == 3)
    {
        if (strcmp(argv[1], MODE_STR[DEBUG]) == 0)
        {
            running_mode = DEBUG;
            filename = argv[2];
        }
        else if (strcmp(argv[1], MODE_STR[NO_LOG]) == 0)
        {
            running_mode = NO_LOG;
            filename = argv[2];
        }
        else
        {
            ERROR_PRINT("First argument is not matching any valid running mode!");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        ERROR_PRINT("The wrong number of arguments has been passed!");
        exit(EXIT_FAILURE);
    }

    data_t data;
    initialize_exec_data(&data);

    FILE* fp = open_file(filename);
    read_file_in_array(fp, &data);
    fclose(fp);

    print_log_header(running_mode);

    char buffer_output[BUFFER_SIZE];
    int buffer_output_index = 0;

    // When NO_LOG defined print the output directly without any logging
    if (running_mode == NO_LOG)
    {
        printf("Output: ");
    }


    for (; data.pos_orders < data.orders_length; data.pos_orders++)
    {
        if (data.orders[data.pos_orders] == NEW_LINE
            || data.orders[data.pos_orders] == SPACE_KEY)
        {
            continue;
        }

        if (run_jump_if_zero(&data))
        {
            continue;
        }
        const bool is_jump_active = data.is_jump_if_zero || data.is_jump_if_not_zero;

        //return true if jump conditions are met
        const bool status_jump_execution =
            process_instruction(&data, is_jump_active, running_mode);

        if (running_mode == DEBUG)
        {
            debug(&data);
        }


        if (data.orders[data.pos_orders] == TOKEN_DISPLAY)
        {
            if (running_mode == NO_LOG)
            {
                printf("%c", (char)data.tape[data.pos_tape]);
            }
            else
            {
                if (buffer_output_index == BUFFER_SIZE - 1)
                {
                    ERROR_PRINT("Output size to big for buffer. Try with NO_LOG option!");
                    exit(EXIT_FAILURE);
                }
                buffer_output[buffer_output_index] = (char)data.tape[data.pos_tape];
                buffer_output_index++;
            }

        }

        set_is_jump(&data, status_jump_execution);
        reset_is_jump(&data, status_jump_execution);
        run_jump_if_not_zero(&data);
    }


    if (running_mode != NO_LOG)
    {
        buffer_output[buffer_output_index] = 0;
        printf("\nOutput: %s", buffer_output);
    }

    free(data.orders);
    return EXIT_SUCCESS;
}

























