#ifndef SORTING_H
#define SORTING_H

#include "headers.h"
#include "vector.h"

static const int NUM_ARGS = 6;
static const int MAX_SIZE_STR = 100;

enum arguments
{
    SIZE = 1,
    INPUT,
    OUTPUT,
    ALG,
    COMP
};

int des (char* first, char* second);
int asc (char* first, char* second);

void bubble (struct vector* vec, int func_cmp (char*, char*));
void insertion (struct vector* vec, int func_cmp (char*, char*));
void merge(struct vector* vec, int func_cmp(char*, char*));
void quick (struct vector* vec, int func_cmp (char*, char*));
void radix(struct vector* vec, int comp);

#endif // SORTING_H
