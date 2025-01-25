#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"

void clear_buffer(char* buffer)
{
    for (int i = 0; i < strlen(buffer); i++)
    {
        buffer[i] = 0;
    }
}


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf(stderr,"The wrong number of arguments has been passed!");
        return EXIT_FAILURE;
    }

    FILE* fp = fopen(argv[1], "r");
    if (!fp)
    {
        perror("File opening failed!");
        return EXIT_FAILURE;
    }

    node_t* head = nullptr;
    node_t* tmp = create_new_node(0);

    insert_at_head(&head, tmp);
    node_t** current_node = &head;

    char jump_to_token;
    bool is_jump_failed = false;

    int buffer_index = 0;
    char buffer_instructions[100];
    clear_buffer(buffer_instructions);

    bool store_instructions = false;

    printf("STEP\t INSTRUCTION    VALUE\n");

    int c;
    while ((c = fgetc(fp)) != EOF)
    {
        if (c == NEW_LINE || c == SPACE_KEY)
        {
            continue;
        }

        //TODO: Test jump instructions to verify that everything is implemented correctly
        if (is_jump_failed)
        {
            if (jump_to_token == c)
            {
                is_jump_failed = false;
            }
            else
            {
                //TODO: Add buffer for skipped instructions
                //continue;
            }
        }

        bool status = false;

        if (!is_jump_failed)
        {
            status = process_instruction(c, current_node);
            if (!status)
            {
                if (c == TOKEN_JUMP_IF_ZERO)
                {
                    jump_to_token = TOKEN_JUMP_IF_NOT_ZERO;
                }
                if (c == TOKEN_JUMP_IF_NOT_ZERO)
                {
                    jump_to_token = TOKEN_JUMP_IF_ZERO;
                }
                is_jump_failed = true;
            }
        }

        //Store instructions if TOKEN_JUMP_IF_NOT_ZERO is executed
        if (status && c == TOKEN_JUMP_IF_ZERO || store_instructions)
        {
            if (c != TOKEN_JUMP_IF_ZERO && c != TOKEN_JUMP_IF_NOT_ZERO
                || (c == TOKEN_JUMP_IF_NOT_ZERO && (*current_node)->value == 0))
            {
                buffer_instructions[buffer_index] = (char)c;
                buffer_index++;
            }

            store_instructions = true;

            if (c == TOKEN_JUMP_IF_NOT_ZERO && (*current_node)->value != 0)
            {
                process_instruction(TOKEN_JUMP_IF_NOT_ZERO, current_node);

                buffer_index--;
                for (;buffer_index >= 0; buffer_index--)
                {
                    process_instruction(buffer_instructions[buffer_index], current_node);
                }

                buffer_index = 0;
                clear_buffer(buffer_instructions);
                store_instructions = false;
            }
        }
    }

    fclose(fp);

    //print_list(head);
    free_list(head);

    return EXIT_SUCCESS;
}
