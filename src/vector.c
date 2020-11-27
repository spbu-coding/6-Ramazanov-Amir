#include "vector.h"

void remove_vector (struct vector* vec)
{
    for (int i = 0; i < vec->size; ++i)
    {
        free (vec->data[i]);
    }
    vec->size = 0;
    vec->size_max = 0;
    vec->data = 0;
}
struct vector new_vector ()
{
    struct vector tmp;
    tmp.size = 0;
    tmp.size_max = MINIMAL_SIZE;
    tmp.data = (char**) malloc (MINIMAL_SIZE * sizeof (char*));
    return tmp;
}
struct vector new_vector_size (int size)
{
    struct vector tmp;
    tmp.size = size;
    tmp.size_max = size;
    tmp.data = (char**) calloc (size, sizeof (char*));
    return tmp;
}
struct vector copy_vector (struct vector* vec)
{
    struct vector out = new_vector_size (vec->size);
    for (int i = 0; i < vec->size; ++i)
    {
        emplace (&out, i, vec->data[i], strlen (vec->data[i]));
    }
    return out;
}
void emplace(struct vector* vec, int idx, char* rhs, size_t size) {
  if (vec->data[idx] != 0) {
    free(vec->data[idx]);
  }
  vec->data[idx] = calloc(size, sizeof(char));
  memcpy(vec->data[idx], rhs, size);
}
void push_back(struct vector* vec, char* str, size_t size) {
  if (vec->size_max < vec->size) {
    vec->size_max = vec->size_max * 2;
    char** new_p = (char**)calloc(vec->size_max, sizeof(char*));
    for (int i = 0; i < vec->size; ++i) {
      new_p[i] = vec->data[i];
    }
    vec->data = new_p;
  }
  char* p = (char*)calloc(size, sizeof(char));
  for (int i = 0; i < size; ++i) {
    p[i] = str[i];
  }
  vec->data[vec->size] = p;
  ++vec->size;
}
