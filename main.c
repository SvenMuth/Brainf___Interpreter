#include <stdio.h>
#include <string.h>

#include "interpreter.h"


int main(void)
{
    node_t* head = nullptr;
    node_t* tmp = create_new_node(0);

    insert_at_head(&head, tmp);
    node_t** current_node = &head;

    //TODO: Add specify file to read instructions from
    constexpr char instructions[] = ", > , > ,. <. <.";
    for (int i = 0; i < strlen(instructions); i++)
    {
        if (instructions[i] == SPACE_KEY)
        {
            continue;
        }

        process_instructions(instructions[i], current_node);
        //printf("%d\n", head->index);
    }

    print_list(head);
    free_list(head);
}
