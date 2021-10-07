//
// Created by subhuman on 03.10.2021.
//

#ifndef HIGHPERFORMANCE1_LINKED_LIST_H
#define HIGHPERFORMANCE1_LINKED_LIST_H

#include <stdbool.h>

typedef struct node node;

struct node {
    node *next;
    node *prev;
    void *value;
};

typedef struct {
    node *first;
    node *last;
    int length;
} linked_list;

linked_list *init_list();

void add(linked_list *list, void *value);

void remove_last(linked_list *list, bool should_free_value);

void free_list(linked_list *list, bool should_free_value);

node *get(linked_list *list, int index);

#endif //HIGHPERFORMANCE1_LINKED_LIST_H
