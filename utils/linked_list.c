//
// Created by subhuman on 03.10.2021.
//

#include <malloc.h>
#include "linked_list.h"

linked_list *init_list() {
    linked_list *list = calloc(1, sizeof(linked_list));
    list->length = 0;
    return list;
}

void free_list(linked_list *list, bool should_free_value) {
    node *ptr = list->first;
    node *to_remove;
    while (ptr != NULL) {
        to_remove = ptr;
        ptr = ptr->next;
        if(should_free_value) free(to_remove->value);
        free(to_remove);
    }
    free(list);
}

void add(linked_list *list, void *value) {
    node *ptr = malloc(sizeof(node));
    ptr->value = value;
    ptr->next = NULL;
    if(list->length == 0) {
        ptr->prev = NULL;
        list->first = ptr;
    } else {
        if(list->length == 1) {
            list->first->next = ptr;
        }
        ptr->prev = list->last;
        list->last->next = ptr;
    }
    list->last = ptr;
    list->length += 1;
}

void remove_last(linked_list *list, bool should_free_value) {
    if(list->length > 1) {
        list->last->prev->next = NULL;
        node *prev = list->last->prev;
        if(should_free_value) free(list->last->value);
        free(list->last);
        list->last = prev;
        list->length -= 1;
    } else if (list->length == 1) {
        free(list->last);
        list->first = NULL;
        list->last = NULL;
        list->length = 0;
    }
}

node *get(linked_list *list, int index) {
    if(index < list->length) {
        node *current = list->first;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current;
    } else {
        return NULL;
    }
}