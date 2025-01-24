//
// Created by sven on 24.01.25.
//

#ifndef MAIN_H
#define MAIN_H

#define TOKEN_MOVE_RIGHT          '>'
#define TOKEN_MOVE_LEFT           '<'
#define TOKEN_DISPLAY             '.'
#define TOKEN_READ                ','
#define TOKEN_ADD_ONE             '+'
#define TOKEN_SUBTRACT_ONE        '-'
#define TOKEN_JUMP_IF_ZERO        '['
#define TOKEN_JUMP_IF_NOT_ZERO    ']'
#define SPACE_KEY                 ' '

typedef struct node
{
    int value;
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

void progress_instructions(const char instruction, node_t** current_pos);
void move_right(node_t** current_pos);
void move_left(node_t** current_pos);
void display(const node_t* current_pos);
void read(node_t* current_pos);
void add(node_t* current_pos);
void subtract(node_t* current_pos);

void jump_if_zero(node_t** current_pos);
void jump_if_not_zero(node_t** current_pos);

#endif //MAIN_H
