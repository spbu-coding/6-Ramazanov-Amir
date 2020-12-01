#include "sortings.h"
#include "headers.h"

void swap(char **str1, char **str2) {
    char *tmp = *str1;
    *str1 = *str2;
    *str2 = tmp;
}

void bubble(strings_array_t line_array, array_size_t array_size, comparator_func_t comparator) {

    for (unsigned i = 0; i + 1 < array_size; i++) {

        for (unsigned j = 0; j < array_size - 1; j++) {

            if (comparator(line_array[j], line_array[j + 1])) {

                swap(&line_array[j], &line_array[j + 1]);
            }
        }
    }
}

void insertion(strings_array_t line_array, array_size_t array_size, comparator_func_t comparator) {

    for (unsigned i = 0; i + 1 < array_size; i++) {

        unsigned temp = i;
        for (unsigned j = i + 1; j < array_size; j++) {

            if (comparator(line_array[temp], line_array[j])) temp = j;
        }
        if (temp != i) swap(&line_array[i], &line_array[temp]);
    }
}

void merge(strings_array_t line_array, array_size_t array_size, comparator_func_t comparator) {

    if (array_size == 1) return;
    array_size_t left_size = (array_size - array_size / 2);
    array_size_t right_size = array_size / 2;
    strings_array_t array_left = malloc(left_size * sizeof(char *));
    for (unsigned i = 0; i < left_size; i++) array_left[i] = line_array[i];
    strings_array_t array_right = malloc(right_size * sizeof(char *));
    for (unsigned i = left_size; i < array_size; i++) array_right[i - left_size] = line_array[i];

    merge(array_left, left_size, comparator);
    merge(array_right, right_size, comparator);

    unsigned i = 0, j = 0, k = 0;
    while (i < left_size && j < right_size) {

        if (comparator(array_left[i], array_right[j])) {

            line_array[k] = array_right[j];
            j++;
            k++;
        } else {

            line_array[k] = array_left[i];
            i++;
            k++;
        }
    }
    while (i < left_size) {

        line_array[k] = array_left[i];
        i++;
        k++;
    }
    while (j < right_size) {

        line_array[k] = array_right[j];
        j++;
        k++;
    }
    free(array_left);
    free(array_right);
}

void quick(strings_array_t line_array, array_size_t array_size, comparator_func_t comparator) {

    unsigned mid = array_size / 2, i = 0, j = array_size - 1;
    do {

        while (comparator(line_array[mid], line_array[i])) i++;
        while (comparator(line_array[j], line_array[mid])) j--;
        if (i <= j) {

            swap(&line_array[i], &line_array[j]);
            i++;
            j--;
        }
    } while (i <= j);
    if (j > 0) quick(line_array, j + 1, comparator);
    if (i < array_size - 1) quick(&line_array[i], array_size - i, comparator);
}

void radix(strings_array_t line_array, array_size_t array_size, comparator_func_t comparator) {

    unsigned max_len = strlen(line_array[0]);
    for (unsigned i = 1; i < array_size; i++) {

        if (strlen(line_array[i]) > max_len) max_len = strlen(line_array[i]);
    }
    int radix_num = -1;
    for (unsigned j = 0; j < max_len; j++) {

        unsigned i = 0;
        while (strlen(line_array[i]) <= j) i++;
        char first = line_array[0][j];
        for (i++; i < array_size; i++) {

            if (strlen(line_array[i]) <= j) continue;
            if (line_array[i][j] != first) break;
        }
        if (i < array_size) {

            radix_num = j;
            break;
        }
    }
    if (radix_num == -1) return; 
    unsigned symbols[256] = {0};
    unsigned **lines_via_symbol = malloc(256 * sizeof(unsigned *));
    if (lines_via_symbol == NULL) {
        for (unsigned i = 0; i < 256; i++) {
            lines_via_symbol[i] = malloc(array_size * sizeof(unsigned));
        }
        for (unsigned i = 0; i < array_size; i++) {
            lines_via_symbol[(unsigned) line_array[i][radix_num]][symbols[(unsigned) line_array[i][radix_num]]] = i;
            symbols[(unsigned) line_array[i][radix_num]]++;
        }

        unsigned k = 0;
        strings_array_t line_array_copy = malloc(array_size * sizeof(char *));
        if (comparator("b", "a")) {
            for (int i = 0; i < 256; i++) {
                if (symbols[i] > 0) {
                    strings_array_t buf = malloc(symbols[i] * sizeof(char *));
                    for (unsigned j = 0; j < symbols[i]; j++) {
                        buf[j] = line_array[lines_via_symbol[i][j]];
                    }
                    if (symbols[i] > 1) radix(buf, symbols[i], comparator);
                    for (unsigned j = 0; j < symbols[i]; j++) {
                        line_array_copy[k] = buf[j];
                        k++;
                    }
                    free(buf);
                }
            }
        } else {

            for (int i = 255; i >= 0; i--) {

                if (symbols[i] > 0) {

                    strings_array_t buf = malloc(symbols[i] * sizeof(char *));
                    for (unsigned j = 0; j < symbols[i]; j++) {
                        buf[j] = line_array[lines_via_symbol[i][j]];
                    }
                    if (symbols[i] > 1) radix(buf, symbols[i], comparator);
                    for (unsigned j = 0; j < symbols[i]; j++) {

                        line_array_copy[k] = buf[j];
                        k++;
                    }
                    free(buf);
                }
            }
        }
        for (unsigned i = 0; i < 256; i++) free(lines_via_symbol[i]);
        free(lines_via_symbol);
        memcpy(line_array, line_array_copy, array_size * sizeof(char *));
        free(line_array_copy);
    }
}
