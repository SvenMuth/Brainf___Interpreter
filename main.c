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

    int c;
    while ((c = fgetc(fp)) != EOF)
    {
        if (c == NEW_LINE || c == SPACE_KEY)
        {
            continue;
        }

        process_instruction(c, current_node);
        printf("%c\n", c);
    }

    fclose(fp);

    print_list(head);
    free_list(head);

    return EXIT_SUCCESS;
}
