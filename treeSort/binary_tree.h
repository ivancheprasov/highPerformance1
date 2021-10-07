//
// Created by subhuman on 03.10.2021.
//

#ifndef HIGHPERFORMANCE1_BINARY_TREE_H
#define HIGHPERFORMANCE1_BINARY_TREE_H

#include "../utils/linked_list.h"

typedef struct tree_node tree_node;

struct tree_node {
    tree_node *left_child;
    int *data;
    tree_node *right_child;
};

int *tree_sort(int *array, int length);

int *tree_sort_parallel(int *array, int length);

int *tree_sort_with_parallel_traverse(int *array, int length);

static tree_node *insert_all(int *array, int length);

static tree_node *insert(tree_node *root, int data);

static linked_list *get_sorted(tree_node * node);

static tree_node *get_next(tree_node *node);

static int *to_int_array(linked_list *tree_node_list, int length);

#endif //HIGHPERFORMANCE1_BINARY_TREE_H
