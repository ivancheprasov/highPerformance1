//
// Created by subhuman on 03.10.2021.
//

#include <stddef.h>
#include <stdbool.h>
#include <malloc.h>
#include "binary_tree.h"
#include "../utils/array_utils.h"

int *tree_sort(int *array, int length) {
    tree_node *root = insert_all(array, length);
    linked_list *sorted = get_sorted(root);
    int *result = to_int_array(sorted, length);
    free_list(sorted, true);
    return result;
}

int *tree_sort_parallel(int *array, int length) {
    return sort_parallel(array, length, tree_sort_with_parallel_traverse);
}

int *tree_sort_with_parallel_traverse(int *array, int length) {
    tree_node *root = insert_all(array, length);
    linked_list *first_part = NULL;
    linked_list *second_part = NULL;
    int thread_number = 2;
#pragma omp parallel default(none) num_threads(thread_number) shared(thread_number, root, first_part, second_part)
    {
#pragma omp single
        {
#pragma omp task default(none) shared(first_part, root)
            if(root != NULL) first_part = get_sorted(root->left_child);
#pragma omp task default (none) shared(second_part, root)
            if(root != NULL) second_part = get_sorted(root->right_child);
        }
#pragma omp taskwait
    }
    if(first_part == NULL) {
        first_part = init_list();
    }
    if(root != NULL) add(first_part, root);
    if(second_part != NULL) {
        first_part->last->next = second_part->first;
        first_part->last = second_part->last;
        first_part->length = first_part->length + second_part->length;
    }
    int *parsed_result = to_int_array(first_part, length);
    free_list(first_part, true);
    if(second_part != NULL) free(second_part);
    return parsed_result;
}

static tree_node *insert_all(int *array, int length) {
    tree_node *root = NULL;
    if (length > 0) {
        root = insert(NULL, array[0]);
        for (int i = 1; i < length; i++) {
            insert(root, array[i]);
        }
    }
    return root;
}

static tree_node *insert(tree_node *root, int data) {
    tree_node *new_node = malloc(sizeof(tree_node));
    new_node->data = malloc(sizeof(int));
    *new_node->data = data;
    new_node->left_child = NULL;
    new_node->right_child = NULL;
    tree_node *current = root;
    tree_node *previous = NULL;
    while (current != NULL) {
        previous = current;
        current = data < *current->data ? current->left_child : current->right_child;
    }
    if (previous != NULL) {
        if (data < *previous->data) {
            previous->left_child = new_node;
        } else {
            previous->right_child = new_node;
        }
    }
    return new_node;
}

static linked_list *get_sorted(tree_node *node) {
    tree_node *current = node;
    linked_list *result = init_list();
    linked_list *stack = init_list();
    bool is_done = false;
    while (!is_done) {
        if (current != NULL) {
            add(stack, current);
            current = current->left_child;
        } else if (stack->length != 0) {
            current = stack->last->value;
            remove_last(stack, false);
            add(result, current);
            current = current->right_child;
        } else {
            is_done = true;
        }
    }
    free_list(stack, false);
    return result;
}

static int *to_int_array(linked_list *tree_node_list, int length) {
    node *current = tree_node_list->first;
    int *result = malloc(length * sizeof(int));
    for (int i = 0; i < tree_node_list->length; ++i) {
        result[i] = *(((tree_node *) current->value)->data);
        current = current->next;
    }
    return result;
}
