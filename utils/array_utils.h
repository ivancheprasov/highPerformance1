//
// Created by subhuman on 04.10.2021.
//

#ifndef HIGHPERFORMANCE1_ARRAY_UTILS_H
#define HIGHPERFORMANCE1_ARRAY_UTILS_H

int *generate_random_array(int length);

void print_array(int *array, int length);

int compare(const void *first_value, const void *second_value);

int *sort_parallel(int *array, int length, int *(*algorithm)(int *array, int length));

#endif //HIGHPERFORMANCE1_ARRAY_UTILS_H
