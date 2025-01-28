//
// Created by sven on 24.01.25.
//

#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "interpreter.h"


node_t* create_new_node(const int value)
{
    node_t* new_node = malloc(sizeof(node_t));
    *new_node = (node_t) {
        .index = -1,
        .next = nullptr,
        .prev = nullptr,
        .value = value,
    };

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
    if (head->prev != nullptr)
    {
        ERROR_PRINT("Freeing the linked list failed. Pointer was not the head of the list");
        exit(EXIT_FAILURE);
    }

    while (head != nullptr)
    {
        node_t* tmp = head;
        head = head->next;
        free(tmp);
    }
}

void print_list(node_t* head)
{
    print_value(head);
    print_index(head);
}

void print_value(node_t* head)
{
    printf("value: ");
    node_t* tmp = head;
    while (tmp != nullptr)
    {
        printf("%d - ", tmp->value);
        tmp = tmp->next;
    }
    printf("\n\n");
}

void print_index(node_t* head)
{
    printf("index: ");
    node_t* tmp = head;
    while (tmp != nullptr)
    {
        printf("%d - ", tmp->index);
        tmp = tmp->next;
    }
    printf("\n\n");
}
















