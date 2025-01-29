
#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"
#include "debugger.h"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        ERROR_PRINT("The wrong number of arguments has been passed!");
        return EXIT_FAILURE;
    }

    data_t data;
    initialize_exec_data(&data);

    FILE* fp = open_file(argv[1]);
    read_file_in_array(fp, &data);
    fclose(fp);

    print_log_header();

    // When NO_LOG defined print the output directly without any logging
#ifndef NO_LOG
    char buffer_output[BUFFER_SIZE];
    int buffer_output_index = 0;
#else
    printf("Output: ");
#endif

    for (; data.pos_orders < data.orders_length; data.pos_orders++)
    {
        //data.current_instruction = data.orders[data.pos_orders];

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
            process_instruction(&data, is_jump_active);

#ifdef DEBUG
        debug(&data);
#endif

        if (data.orders[data.pos_orders] == TOKEN_DISPLAY)
        {
#ifndef NO_LOG
            if (buffer_output_index == BUFFER_SIZE - 1)
            {
                ERROR_PRINT("Output size to big for buffer. Try with NO_LOG option!");
                exit(EXIT_FAILURE);
            }
            buffer_output[buffer_output_index] = (char)data.tape[data.pos_tape];
            buffer_output_index++;
#else
            printf("%c", (char)data.tape[data.pos_tape]);
#endif
        }

        set_is_jump(&data, status_jump_execution);
        reset_is_jump(&data, status_jump_execution);
        run_jump_if_not_zero(&data);
    }


#ifndef NO_LOG
    buffer_output[buffer_output_index] = 0;
    printf("\nOutput: %s", buffer_output);
#endif

    free(data.orders);
    return EXIT_SUCCESS;
}

























