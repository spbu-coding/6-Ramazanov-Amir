#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#define MAX_LENGTH_FILE_NAME 100
#define MAX_LENGTH_VECTOR_NAME 10
#define MAX_LENGTH_COMPARER_NAME 4

typedef struct {
    long long strings_count;
    char input_filename[MAX_LENGTH_FILE_NAME];
    char output_filename[MAX_LENGTH_FILE_NAME];
    char vector_name[MAX_LENGTH_VECTOR_NAME];
    char comparer_name[MAX_LENGTH_COMPARER_NAME];
} arguments_t;

int check_vector_name(char *vector_name);
int check_comparer_name(char *comparer_name);
int set_parameters_values(int argc, char *argv[], arguments_t *arguments);

#endif