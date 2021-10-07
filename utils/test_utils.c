//
// Created by subhuman on 05.10.2021.
//

#include <bits/types/clock_t.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <malloc.h>
#include "test_utils.h"
#include "../const.h"

double test_sorting_algorithm(int *original_array, int *verification_array, char *algorithm_name, int *(*algorithm)(int *, int)){
    struct timeval tv1, tv2;
    clock_t start_t, end_t;
    double cpu_time, real_time;
    int attempts_number = 5;
    printf("%s\n", algorithm_name);
    printf("Starting %d test(s) on %d elements...\n", attempts_number, ARRAY_LENGTH);
    bool is_correct = true;
    double real_time_sum = 0, cpu_time_sum = 0;
    for (int i = 1; i < attempts_number + 1; ++i) {
        start_t = clock();
        gettimeofday(&tv1, NULL);
        int *sorted = (*algorithm)(original_array, ARRAY_LENGTH);
        gettimeofday(&tv2, NULL);
        end_t = clock();
        cpu_time = (double) (end_t - start_t) / CLOCKS_PER_SEC;
        real_time = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
        printf("Test %d finished in %.2fs and used %.2fs of cpu time.\n", i, real_time, cpu_time);
        if (memcmp(sorted, verification_array, sizeof(int) * ARRAY_LENGTH) != 0) {
            is_correct = false;
        }
        free(sorted);
        real_time_sum += real_time;
        cpu_time_sum += cpu_time;
    }
    puts("Checking accuracy...");
    if (is_correct) {
        puts("Great Job! No errors occurred");
    } else {
        puts("Something went wrong.(");
    }
    double acceleration_rate = cpu_time_sum / real_time_sum;
    printf("Mean real time is: %.2f seconds\n", real_time_sum / attempts_number);
    printf("Mean cpu time is: %.2f seconds\n", cpu_time_sum / attempts_number);
    printf("Total acceleration acceleration_rate is %.2f!\n", acceleration_rate);
    return acceleration_rate;
}
