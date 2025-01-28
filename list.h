//
// Created by sven on 24.01.25.
//

#ifndef LIST_H
#define LIST_H

typedef struct node
{
    int value;
    int index;
    struct node* next;
    struct node* prev;
} node_t;


node_t* create_new_node(const int value);
node_t* insert_at_head(node_t** head, node_t* node_to_insert);
void insert_after_node(node_t* node_to_insert_after, node_t* new_node);
node_t* find_node(node_t* head, const int value);
void remove_node(node_t** head, node_t* node_to_remove);
void free_list(node_t* head);

void print_list(node_t* head);
void print_value(node_t* head);
void print_index(node_t* head);

#endif //LIST_H













