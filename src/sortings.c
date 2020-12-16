#include "sortings.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//begin очередь для радикс сорта, годится только для решения заданной задачи

typedef struct query_t {
    unsigned *data;
    unsigned max_size;
    unsigned size;
    unsigned l;
    unsigned r;
} query;

static void init_query(query *q, unsigned max_size) {
    q->data = (unsigned *) malloc(max_size * sizeof(unsigned));
    q->max_size = max_size;
    q->size = 0;
    q->l = 0;
    q->r = 0;
}

static void free_query(query *q) {
    free(q->data);
}

static unsigned get_size(query *q) {
    return q->size;
}

static void push(query *q, unsigned element) {
    if (get_size(q) >= q->max_size)
        return;
    q->data[q->r] = element;
    q->r = (q->r + 1) % q->max_size;
    q->size++;
}

static unsigned pop(query *q) {
    if (q->size == 0)
        return 0;
    unsigned element = q->data[q->l];
    q->l = (q->l + 1) % q->max_size;
    q->size--;
    return element;
}

//end

static void swap(char **first, char **second) {
    char *tmp = *first;
    *first = *second;
    *second = tmp;
}

static array_size_t min(array_size_t first, array_size_t second) {
    if (first < second)
        return first;
    else
        return second;
}

void bubble(strings_array_t array, array_size_t size, comparator_func_t comparator) {
    if(size <= 1)
        return;

    for (array_size_t i = 0; i < size - 1; i++) {
        for (array_size_t j = 0; j < size - 1; j++) {
            if (comparator(array[j], array[j + 1]) > 0) {
                swap(&array[j], &array[j + 1]);
            }
        }
    }
}

void insertion(strings_array_t array, array_size_t size, comparator_func_t comparator) {
    if(size <= 1)
        return;

    for (array_size_t i = 1; i < size; i++) {
        for (array_size_t j = i; j > 0; j--) {
            if (comparator(array[j - 1], array[j]) > 0) {
                swap(&array[j - 1], &array[j]);
            }
        }
    }
}

void merge(strings_array_t array, array_size_t size, comparator_func_t comparator) {
    if(size <= 1)
        return;

    char **tmp_array = (char **) malloc(sizeof(char *) * size);

    for (array_size_t i = 1; i < size; i *= 2) {
        for (array_size_t j = 0; j < size; j += i * 2) {
            array_size_t left_part_size = min(size - j, i);
            array_size_t right_part_size;
            if (j + i >= size)
                right_part_size = 0;
            else
                right_part_size = min(size - (j + i), i);

            array_size_t p1 = 0;
            array_size_t p2 = 0;
            while (p1 < left_part_size && p2 < right_part_size) {
                if (comparator(array[j + p1], array[j + i + p2]) < 0) {
                    swap(&tmp_array[p1 + p2], &array[j + p1]);
                    p1++;
                } else {
                    swap(&tmp_array[p1 + p2], &array[j + i + p2]);
                    p2++;
                }
            }
            while (p1 < left_part_size) {
                swap(&tmp_array[p1 + p2], &array[j + p1]);
                p1++;
            }
            while (p2 < right_part_size) {
                swap(&tmp_array[p1 + p2], &array[j + i + p2]);
                p2++;
            }
            for (array_size_t k = 0; k < i * 2; k++) {
                if (j + k >= size)
                    break;
                swap(&tmp_array[k], &array[j + k]);
            }
        }
    }

    free(tmp_array);
}

void quick(strings_array_t array, array_size_t size, comparator_func_t comparator) {
    if (size <= 1)
        return;

    array_size_t l = 0;
    array_size_t r = size - 1;
    array_size_t delimiter = rand() % size;

    while (l <= r) {
        while (comparator(array[l], array[delimiter]) < 0)
            l++;
        while (comparator(array[delimiter], array[r]) < 0)
            r--;
        if (l <= r) {
            swap(&array[l], &array[r]);
            l++;
            if (r > 0)
                r--;
        }
    }

    if (r > 0)
        quick(array, r + 1, comparator);
    if (l < size)
        quick(array + l, size - l, comparator);
}

void radix(strings_array_t array, array_size_t size, comparator_func_t comparator) {
    if(size <= 1)
        return;

    query alphabet[256];
    size_t max_string_size = 0;
    for (unsigned i = 0; i < 256; i++) {
        init_query(&alphabet[i], size);
    }
    //подготовительная секция, раскладываем элементы по "рюкзакам"
    for (unsigned i = 0; i < size; i++) {
        unsigned char c;
        if (strlen(array[i]) > 0)
            c = array[i][strlen(array[i]) - 1];
        else
            c = '\0';
        if (strlen(array[i]) > max_string_size) {
            max_string_size = strlen(array[i]);
        }
        push(&alphabet[c], i);
    }
    //основная секция
    for (unsigned i = 1; i <= max_string_size; i++) {
        for (unsigned j = 0; j < 256; j++) {
            unsigned a_size = get_size(&alphabet[j]);
            while (a_size > 0) {
                a_size--;

                unsigned index = pop(&alphabet[j]);
                unsigned char c;
                if (i < strlen(array[index])) {
                    c = array[index][strlen(array[index]) - i - 1];
                } else {
                    c = (unsigned char) j;
                }

                push(&alphabet[c], index);
            }
        }
    }
    //секция соединения всех очередей в одну
    for (unsigned i = 0; i < 256; i++) {
        unsigned a_size = get_size(&alphabet[i]);
        while (a_size > 0) {
            a_size--;
            push(&alphabet[0], pop(&alphabet[i]));
        }
    }
    //секция сортировки основного массива с помощью заданной очереди, как должны идти элементы
    char** temp = (char**)malloc(size * sizeof(char*));
    for (unsigned i = 0; i < size; i++) {
        swap(&temp[i], &array[pop(&alphabet[0])]);
    }
    for(unsigned i = 0; i < size; i++) {
        swap(&array[i], &temp[i]);
    }
    //реверс массива, если отсортировать требовалось по убыванию
    for (unsigned i = 0; i < size / 2; i++) {
        if (comparator(array[i], array[size - i - 1]) > 0)
            swap(&array[i], &array[size - i - 1]);
    }
    //free секция
    free(temp);
    for(unsigned i = 0; i < 256; i++) {
        free_query(&alphabet[i]);
    }
}
