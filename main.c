#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils/array_utils.h"
#include "const.h"
#include "treeSort/binary_tree.h"
#include "utils/test_utils.h"
#include "smoothSort/smooth_sort.h"

int main() {
    int *array = generate_random_array(ARRAY_LENGTH);
    int *quick_sorted = calloc(ARRAY_LENGTH, sizeof(int));
    memcpy(quick_sorted, array, sizeof(int) * ARRAY_LENGTH);
    qsort(quick_sorted, ARRAY_LENGTH, sizeof(int), compare);
    test_sorting_algorithm(array, quick_sorted, "Multi-threaded Tree Sort", tree_sort_parallel);
    puts("");
    test_sorting_algorithm(array, quick_sorted, "Tree Sort with Parallel Traverse", tree_sort_with_parallel_traverse);
    puts("");
    test_sorting_algorithm(array, quick_sorted, "Single-threaded Tree Sort", tree_sort);
    puts("");
    test_sorting_algorithm(array, quick_sorted, "Multi-threaded Smooth Sort", smooth_sort_parallel);
    puts("");
    test_sorting_algorithm(array, quick_sorted, "Single-threaded Smooth Sort", smooth_sort);
    puts("");
    test_sorting_algorithm(array, quick_sorted, "Alternative Smooth Sort", alternative_smooth_sort);
    free(array);
    free(quick_sorted);
}
