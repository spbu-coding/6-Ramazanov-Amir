#include "vector.h"
#include "headers.h"

#define error(...) (fprintf(stderr, __VA_ARGS__))

#define ERROR_EXIT_CODE -1

#define VECTOR_COUNT 5
#define COMPARER_COUNT 2

const char *VALID_ALGORITHM_NAME[MAX_LENGTH_VECTOR_NAME] = {"bubble", "insertion", "merge", "quick", "radix"};
const char *VALID_COMPARER_NAME[MAX_LENGTH_COMPARER_NAME] = {"asc", "des"};

int check_algorithm_name(char *algorithm_name) {
    for (int i = 0; i < VECTOR_COUNT; ++i) {
        if (strcmp(algorithm_name, VALID_ALGORITHM_NAME[i]) == 0) {
            return 0;
        }
    }
    return -1;
}

int check_comparer_name(char *comparer_name) {
    for (int i = 0; i < COMPARER_COUNT; ++i) {
        if (strcmp(comparer_name, VALID_COMPARER_NAME[i]) == 0) {
            return 0;
        }
    }
    return -1;
}

int set_parameters_values(int argc, char *argv[], arguments_t *arguments) {
    if (argc != 6) {
        error("Parameters were not entered.\n");
        return ERROR_EXIT_CODE;
    }
    arguments->strings_count = strtoll(argv[1], &argv[1], 10);
    if (arguments->strings_count < 0) {
        error("The number of rows must be greater than 0\n");
        return ERROR_EXIT_CODE;
    }
    strncpy(arguments->input_filename, argv[2], MAX_LENGTH_FILE_NAME);
    strncpy(arguments->output_filename, argv[3], MAX_LENGTH_FILE_NAME);
    strncpy(arguments->vector_name, argv[4], MAX_LENGTH_VECTOR_NAME);
    strncpy(arguments->comparer_name, argv[5], MAX_LENGTH_COMPARER_NAME);
    if (check_algorithm_name(arguments->vector_name) != 0) {
        error("Incorrect algorithm name\n");
        return ERROR_EXIT_CODE;
    }
    if (check_comparer_name(arguments->comparer_name) != 0) {
        error("Invalid name of comparer\n");
        return ERROR_EXIT_CODE;
    }
    return 0;
}