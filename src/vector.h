#ifndef VECTOR_H
#define VECTOR_H

#include "headers.h"

static const int MINIMAL_SIZE = 20;

struct vector
{
    int size;
    int size_max;
    char** data;
};

void remove_vector (struct vector* vec);
struct vector new_vector ();
struct vector new_vector_size (int size);
struct vector copy_vector (struct vector* vec);
void emplace(struct vector* vec, int idx, char* rhs, size_t size);
void push_back(struct vector* vec, char* str, size_t size);

#endif // VECTOR_H
