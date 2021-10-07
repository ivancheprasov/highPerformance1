//
// Created by subhuman on 05.10.2021.
//

#include <malloc.h>
#include <string.h>
#include "smooth_sort.h"
#include "../utils/array_utils.h"

#define UP(IA, IB) temp = IA; (IA) += (IB) + 1; (IB) = temp;
#define DOWN(IA, IB) temp = IB; (IB) = (IA) - (IB) - 1; (IA) = temp;

int *smooth_sort_parallel(int *array, int length) {
    return sort_parallel(array, length, smooth_sort);
}

int *smooth_sort(int *array, int length) {
    int *result = malloc(length * sizeof(int));
    memcpy(result, array, length * sizeof(int));
    heap_info *info = malloc(sizeof(heap_info));
    long temp;
    info->array = &result;
    info->q = 1;
    info->r = 0;
    info->p = 1;
    info->b = 1;
    info->c = 1;
    while (info->q < length) {
        info->r1 = info->r;
        if ((info->p & 7) == 3) {
            info->b1 = info->b;
            info->c1 = info->c;
            sift(info);
            info->p = (info->p + 1) >> 2;
            UP(info->b, info->c);
            UP(info->b, info->c);
        } else if ((info->p & 3) == 1) {
            if (info->q + info->c < length) {
                info->b1 = info->b;
                info->c1 = info->c;
                sift(info);
            } else {
                trinkle(info);
            }
            DOWN(info->b, info->c);
            info->p <<= 1;
            while (info->b > 1) {
                DOWN(info->b, info->c);
                info->p <<= 1;
            }
            info->p++;
        }
        info->q++;
        info->r++;
    }
    info->r1 = info->r;
    trinkle(info);
    while (info->q > 1) {
        --info->q;
        if (info->b == 1) {
            info->r--;
            info->p--;
            while ((info->p & 1) == 0) {
                info->p >>= 1;
                UP(info->b, info->c);
            }
        } else {
            if (info->b >= 3) {
                info->p--;
                info->r = info->r - info->b + info->c;
                if (info->p > 0)
                    semi_trinkle(info);
                DOWN(info->b, info->c);
                info->p = (info->p << 1) + 1;
                info->r = info->r + info->c;
                semi_trinkle(info);
                DOWN(info->b, info->c);
                info->p = (info->p << 1) + 1;
            }
        }
    }
    free(info);
    return result;
}

int *alternative_smooth_sort(int *array, int length) {
    int *result = malloc(length * sizeof(int));
    memcpy(result, array, length * sizeof(int));
    unsigned long long *leonardo_numbers = get_leonardo_numbers(length);
    linked_list *heap = init_list();
    for (int i = 0; i < length; ++i) {
        node *last = heap->last;
        if (heap->length >= 2 && *((int *) last->prev->value) == *((int *) last->value) + 1) {
            remove_last(heap, true);
            *((int *) heap->last->value) += 1;
        } else {
            int *new_element = malloc(sizeof(int));
            if (heap->length >= 1 && *((int *) heap->last->value) == 1) {
                *new_element = 0;
            } else {
                *new_element = 1;
            }
            add(heap, new_element);
        }
        restore_heaps(result, i, heap, leonardo_numbers);
    }
    for (int i = length - 1; i >= 0; --i) {
        int last_value = *((int *) heap->last->value);
        if (last_value < 2) {
            remove_last(heap, true);
        } else {
            remove_last(heap, true);
            heap_node_info *info = get_heap_node_info(i, last_value, leonardo_numbers);
            int *new_element = malloc(sizeof(int));
            *new_element = info->left_child_value;
            add(heap, new_element);
            restore_heaps(result, info->left_child_index, heap, leonardo_numbers);
            int *new_element_2 = malloc(sizeof(int));
            *new_element_2 = info->right_child_value;
            add(heap, new_element_2);
            restore_heaps(result, info->right_child_index, heap, leonardo_numbers);
        }
    }
    free(leonardo_numbers);
    if(length > 35) {
        int *full_sorted = smooth_sort_parallel(result, length);
        free(result);
        return full_sorted;
    }
    return result;
}

static void sift(heap_info *info) {
    long r0, r2, temp;
    int t;
    r0 = info->r1;
    t = (*info->array)[r0];
    while (info->b1 >= 3) {
        r2 = info->r1 - info->b1 + info->c1;
        if ((*info->array)[info->r1 - 1] > (*info->array)[r2]) {
            r2 = info->r1 - 1;
            DOWN(info->b1, info->c1);
        }
        if ((*info->array)[r2] <= t) {
            info->b1 = 1;
        } else {
            (*info->array)[info->r1] = (*info->array)[r2];
            info->r1 = r2;
            DOWN(info->b1, info->c1);
        }
    }
    if (info->r1 - r0)
        (*info->array)[info->r1] = t;
}

static void trinkle(heap_info *info) {
    long p1, r2, r3, r0, temp;
    p1 = info->p;
    info->b1 = info->b;
    info->c1 = info->c;
    r0 = info->r1;
    int t = (*info->array)[r0];
    while (p1 > 0) {
        while ((p1 & 1) == 0) {
            p1 >>= 1;
            UP(info->b1, info->c1)
        }
        r3 = info->r1 - info->b1;
        if ((p1 == 1) || (*info->array)[r3] <= t) {
            p1 = 0;
        } else {
            --p1;
            if (info->b1 == 1) {
                (*info->array)[info->r1] = (*info->array)[r3];
                info->r1 = r3;
            } else {
                if (info->b1 >= 3) {
                    r2 = info->r1 - info->b1 + info->c1;
                    if ((*info->array)[info->r1 - 1] > (*info->array)[r2]) {
                        r2 = info->r1 - 1;
                        DOWN(info->b1, info->c1);
                        p1 <<= 1;
                    }
                    if ((*info->array)[r2] <= (*info->array)[r3]) {
                        (*info->array)[info->r1] = (*info->array)[r3];
                        info->r1 = r3;
                    } else {
                        (*info->array)[info->r1] = (*info->array)[r2];
                        info->r1 = r2;
                        DOWN(info->b1, info->c1);
                        p1 = 0;
                    }
                }
            }
        }
    }
    if (r0 - info->r1)
        (*info->array)[info->r1] = t;
    sift(info);
}

static void semi_trinkle(heap_info *info) {
    int temp;
    info->r1 = info->r - info->c;
    if ((*info->array)[info->r1] > (*info->array)[info->r]) {
        temp = (*info->array)[info->r];
        (*info->array)[info->r] = (*info->array)[info->r1];
        (*info->array)[info->r1] = temp;
        trinkle(info);
    }
}

static unsigned long long *get_leonardo_numbers(int length) {
    unsigned long long *result = malloc(length * sizeof(unsigned long long));
    result[0] = 1;
    result[1] = 1;
    for (int i = 2; i < length; ++i) {
        result[i] = result[i - 1] + result[i - 2] + 1;
    }
    return result;
}

static void
restore_heaps(int *array, unsigned long long index, linked_list *heaps, const unsigned long long *leonardo_numbers) {
    node *current = heaps->last;
    unsigned long long computed_index;
    int current_value = *((int *) current->value);
    while (current->prev != NULL) {
        computed_index = index - leonardo_numbers[current_value];
        if (array[computed_index] > array[index] && (current_value < 2 || array[computed_index] > array[index - 1] &&
                                                                          array[computed_index] > array[index - 2])) {
            int temp = array[computed_index];
            array[computed_index] = array[index];
            array[index] = temp;
            index = computed_index;
            current = current->prev;
            current_value = *((int *) current->value);
        } else {
            break;
        }
    }
    while (current_value >= 2) {
        heap_node_info *info = get_heap_node_info(index, current_value, leonardo_numbers);
        if (array[index] < array[info->right_child_index] || array[index] < array[info->left_child_index]) {
            if (array[info->right_child_index] > array[info->left_child_index]) {
                int temp = array[info->right_child_index];
                array[info->right_child_index] = array[index];
                array[index] = temp;
                index = info->right_child_index;
                current_value = info->right_child_value;
            } else {
                int temp = array[info->left_child_index];
                array[info->left_child_index] = array[index];
                array[index] = temp;
                index = info->left_child_index;
                current_value = info->left_child_value;
            }
            free(info);
        } else {
            free(info);
            break;
        }
    }
}

static heap_node_info *
get_heap_node_info(unsigned long long index, int value, const unsigned long long *leonardo_numbers) {
    heap_node_info *result = malloc(sizeof(heap_node_info));
    result->right_child_index = index - 1;
    result->right_child_value = value - 2;
    result->left_child_index = result->right_child_index - leonardo_numbers[result->right_child_value];
    result->left_child_value = value - 1;
    return result;
}