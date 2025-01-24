#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"


node_t* create_new_node(const int value)
{
    node_t* new_node = malloc(sizeof(node_t));

    new_node->value = value;
    new_node->next = nullptr;
    new_node->prev = nullptr;

    return new_node;
}

node_t* insert_at_head(node_t** head, node_t* node_to_insert)
{
    node_to_insert->next = *head;
    if (*head != nullptr)
    {
        (*head)->prev = node_to_insert;
    }
    *head = node_to_insert;
    node_to_insert->prev = nullptr;

    return node_to_insert;
}

void insert_after_node(node_t* node_to_insert_after, node_t* new_node)
{
    new_node->next = node_to_insert_after->next;
    if (new_node->next != nullptr)
    {
        new_node->next->prev = new_node;
    }
    new_node->prev = node_to_insert_after;
    node_to_insert_after->next = new_node;
}

node_t* find_node(node_t* head, const int value)
{
    node_t* tmp = head;
    while (tmp != nullptr)
    {
        if (tmp->value == value) return tmp;
        tmp = tmp->next;
    }

    return nullptr;
}

void remove_node(node_t** head, node_t* node_to_remove)
{
    if (node_to_remove->prev) node_to_remove->prev->next = node_to_remove->next;
    if (node_to_remove->next) node_to_remove->next->prev = node_to_remove->prev;
    if (node_to_remove == *head) *head = node_to_remove->next;

    node_to_remove->prev = node_to_remove->next = nullptr;
}

void free_list(node_t* head)
{
    while (head != nullptr)
    {
        node_t* tmp = head;
        head = head->next;
        free(tmp);
    }
}

void print_list(node_t* head)
{
    node_t* tmp = head;
    while (tmp != nullptr)
    {
        printf("%d - ", tmp->value);
        tmp = tmp->next;
    }
    printf("\n\n");
}


void progress_instructions(const char instruction, node_t** current_pos)
{
    switch (instruction)
    {
    case TOKEN_MOVE_RIGHT:
        move_right(current_pos);
        break;

    case TOKEN_MOVE_LEFT:
        move_left(current_pos);
        break;

    case TOKEN_DISPLAY:
        display(*current_pos);
        break;

    case TOKEN_READ:
        read(*current_pos);
        break;

    case TOKEN_ADD_ONE:
        add(*current_pos);
        break;

    case TOKEN_SUBTRACT_ONE:
        subtract(*current_pos);
        break;

    case TOKEN_JUMP_IF_ZERO:
        jump_if_zero(current_pos);
        break;

    case TOKEN_JUMP_IF_NOT_ZERO:
        jump_if_not_zero(current_pos);
        break;

    default:
        break;
    }
}

void move_right(node_t** current_pos)
{
    if ((*current_pos)->next != nullptr)
    {
        *current_pos = (*current_pos)->next;
    }
    else
    {
        node_t* tmp = create_new_node(0);
        insert_after_node((*current_pos)->next, tmp);
    }
}

void move_left(node_t** current_pos)
{
    if ((*current_pos)->prev != nullptr)
    {
        *current_pos = (*current_pos)->prev;
    }
    else
    {
        node_t* tmp = create_new_node(0);
        insert_after_node((*current_pos)->prev, tmp);
    }
}

void display(const node_t* current_pos)
{
    const char value_char = (char)current_pos->value;
    printf("%c", value_char);
}

void read(node_t* current_pos)
{
    printf("Input value: ");
    current_pos->value = getchar();
}

void add(node_t* current_pos)
{
    current_pos->value += 1;
}

void subtract(node_t* current_pos)
{
    current_pos->value -= 1;
}

void jump_if_zero(node_t** current_pos)
{
    if ((*current_pos)->value == 0)
    {
        node_t* tmp = *current_pos;
        while (tmp != nullptr)
        {
            if (tmp->value == TOKEN_JUMP_IF_ZERO)
            {
                break;
            }
            tmp = tmp->next;
        }
        *current_pos = tmp;
    }
}

void jump_if_not_zero(node_t** current_pos)
{
    if ((*current_pos)->value != 0)
    {
        node_t* tmp = *current_pos;
        while (tmp != nullptr)
        {
            if (tmp->value == TOKEN_JUMP_IF_NOT_ZERO)
            {
                break;
            }
            tmp = tmp->prev;
        }
        *current_pos = tmp;
    }
}


int main(void)
{
    node_t* head = nullptr;
    node_t* tmp = create_new_node(0);

    insert_at_head(&head, tmp);

    const char instructions[] = ", > , > ,. <. <.";
    for (int i = 0; i < strlen(instructions); i++)
    {
        if (instructions[i] == SPACE_KEY)
        {
            continue;
        }

        printf("%c\n", instructions[i]);
        progress_instructions(instructions[i], &head);
    }
}
















