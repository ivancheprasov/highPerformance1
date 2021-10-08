//
// Created by subhuman on 04.10.2021.
//

#include <malloc.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <limits.h>
#include "array_utils.h"

int *generate_random_array(int length) {
    int *result = malloc(length * sizeof(int));
    for (int i = 0; i < length; ++i) {
        result[i] = rand();
    }
    return result;
}

void print_array(int *array, int length) {
    printf("[");
    for (int i = 0; i < length - 1; ++i) {
        printf("%d, ", array[i]);
    }
    printf("%d]\n", array[length - 1]);
}

int compare(const void *first_value, const void *second_value) {
    return (*(int *) first_value) - (*(int *) second_value);
}

int *sort_parallel(int *array, int length, int *(*algorithm)(int *array, int length)) {
    const int thread_num = omp_get_max_threads();
    int *result = malloc(length * sizeof(int));
    int **result_array = malloc(thread_num * sizeof(int *));
    int part_length[thread_num];
#pragma omp parallel default(none) num_threads(thread_num) shared(array, result, result_array, part_length, thread_num, algorithm, length)
    {
        int thread_id = omp_get_thread_num();
        part_length[thread_id] = length / thread_num;
        if (thread_id == thread_num - 1) {
            part_length[thread_id] += length % thread_num;
        }
        int *result_part = malloc(part_length[thread_id] * sizeof(int));
        memcpy(result_part, array + (length / thread_num) * thread_id, part_length[thread_id] * sizeof(int));
#pragma omp for
        for (int i = 0; i < thread_num; ++i) {
            int current_length = part_length[thread_id];
#pragma omp task default(none) shared(result_part, current_length, algorithm)
            result_part = (*algorithm)(result_part, current_length);
        }
        result_array[thread_id] = result_part;
#pragma omp barrier
#pragma omp single
        {
            int index_array[thread_num];
            memset(index_array, 0, thread_num * sizeof(int));
            for (int i = 0; i < length; ++i) {
                int current_min = INT_MAX;
                int current_part;
                for (int j = 0; j < thread_num; ++j) {
                    int current_value = *(result_array[j] + index_array[j]);
                    if (index_array[j] < part_length[j] && current_min > current_value) {
                        current_min = current_value;
                        current_part = j;
                    }
                }
                index_array[current_part] += 1;
                result[i] = current_min;
            }
        }
#pragma omp barrier
        free(result_part);
    }
    free(result_array);
    return result;
}