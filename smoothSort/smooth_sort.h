//
// Created by subhuman on 05.10.2021.
//

#ifndef HIGHPERFORMANCE1_SMOOTH_SORT_H
#define HIGHPERFORMANCE1_SMOOTH_SORT_H

#include "../utils/linked_list.h"

typedef struct {
    int **array;
    long r;
    long q;
    long p;
    long b;
    long c;
    long r1;
    long b1;
    long c1;
} heap_info;

typedef struct {
   unsigned long long left_child_index;
   int left_child_value;
   unsigned long long right_child_index;
   int right_child_value;
} heap_node_info;

int *smooth_sort_parallel(int *array, int length);

int *smooth_sort(int *array, int length);

int *alternative_smooth_sort(int *array, int length);

static void semi_trinkle(heap_info *info);

static void trinkle(heap_info *info);

static void sift(heap_info *info);

static unsigned long long *get_leonardo_numbers(int length);

static void restore_heaps(int *array, unsigned long long index, linked_list *heaps, const unsigned long long *leonardo_numbers);

static heap_node_info *get_heap_node_info(unsigned long long index, int value, const unsigned long long *leonardo_numbers);

#endif //HIGHPERFORMANCE1_SMOOTH_SORT_H
