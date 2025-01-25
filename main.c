#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"


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
    bool is_jump_failed = true;

    printf("STEP\t INSTRUCTION    VALUE\n");

    int c;
    while ((c = fgetc(fp)) != EOF)
    {
        //TODO: Test jump instructions to verify that everything is implemented correctly
        if (!is_jump_failed)
        {
            if (jump_to_token == c)
            {
                is_jump_failed = true;
            }
            else
            {
                continue;
            }
        }

        if (c == NEW_LINE || c == SPACE_KEY)
        {
            continue;
        }

        if (!process_instruction(c, current_node))
        {
            if (c == TOKEN_JUMP_IF_ZERO)
            {
                jump_to_token = TOKEN_JUMP_IF_NOT_ZERO;
            }
            if (c == TOKEN_JUMP_IF_NOT_ZERO)
            {
                jump_to_token = TOKEN_JUMP_IF_ZERO;
            }
            is_jump_failed = false;
        }
    }

    fclose(fp);

    //print_list(head);
    free_list(head);

    return EXIT_SUCCESS;
}
