#include "headers.h"
#include "sortings.h"
#include "vector.h"

#define ERROR_EXIT_CODE -1

#define error(...) (fprintf(stderr, __VA_ARGS__))

int des_cmp(const char *str1, const char *str2) {
    return -strcmp(str1, str2);
}

void sort(arguments_t arguments, strings_array_t strings) {
    if (strcmp(arguments.comparer_name, "asc") == 0) {
        if (strcmp(arguments.vector_name, "bubble") == 0) {
            bubble(strings, arguments.strings_count, strcmp);
        } else if (strcmp(arguments.vector_name, "insertion") == 0) {
            insertion(strings, arguments.strings_count, strcmp);
        } else if (strcmp(arguments.vector_name, "merge") == 0) {
            merge(strings, arguments.strings_count, strcmp);
        } else if (strcmp(arguments.vector_name, "quick") == 0) {
            quick(strings, arguments.strings_count, strcmp);
        } else {
            radix(strings, arguments.strings_count, strcmp);
        }
    } else {
        if (strcmp(arguments.vector_name, "bubble") == 0) {
            bubble(strings, arguments.strings_count, des_cmp);
        } else if (strcmp(arguments.vector_name, "insertion") == 0) {
            insertion(strings, arguments.strings_count, des_cmp);
        } else if (strcmp(arguments.vector_name, "merge") == 0) {
            merge(strings, arguments.strings_count, des_cmp);
        } else if (strcmp(arguments.vector_name, "quick") == 0) {
            quick(strings, arguments.strings_count, des_cmp);
        } else {
            radix(strings, arguments.strings_count, des_cmp);
        }
    }
}

int alloc_strings(strings_array_t *array, long long strings_count) {
    (*array) = malloc(sizeof(char *) * strings_count);
    if ((*array) == NULL) {
        error("Cannot allocate array\n");
        return ERROR_EXIT_CODE;
    }
    for (long long i = 0; i < strings_count; ++i) {
        (*array)[i] = malloc(sizeof(char) * MAX_INPUT_STRING_SIZE);
        if ((*array)[i] == NULL) {
            for (long long j = 0; j < i; ++j) {
                free((*array)[j]);
            }
            error("Cannot allocate array\n");
            return ERROR_EXIT_CODE;
        }
    }
    return 0;
}

void dealloc_strings(strings_array_t *array, array_size_t size) {
    for (array_size_t i = 0; i < size; ++i) {
        free((*array)[i]);
    }
    free(*array);
}

int read_strings_from_file(const char *filename, strings_array_t strings, long long strings_count) {
    FILE *input = fopen(filename, "rt");
    if (input == NULL) {
        error("Cannot open file %s", filename);
        return ERROR_EXIT_CODE;
    }
    for (int i = 0; i < strings_count; ++i) {
        if (feof(input)) {
            error("File does`n contain %lld strings", strings_count);
            return ERROR_EXIT_CODE;
        } else {
            fgets(strings[i], MAX_INPUT_STRING_SIZE, input);
        }
    }
    fclose(input);
    return 0;
}

int write_string_to_file(const char *filename, strings_array_t strings, long long strings_count) {
    FILE *output = fopen(filename, "wt");
    if (output == NULL) {
        error("Cannot open file %s", filename);
        return ERROR_EXIT_CODE;
    }
    for (int i = 0; i < strings_count; ++i) {
        fputs(strings[i], output);
        if (i == strings_count - 1 && strings[i][strlen(strings[i]) - 1] != '\n') {
            fputs("\n", output);
        }
    }
    fclose(output);
    return 0;
}

int main(int argc,  char *argv[]) {
    arguments_t arguments;
    int result = set_parameters_values(argc, argv, &arguments);
    if (result != 0) {
        return result;
    }
    strings_array_t strings = NULL;
    int alloc_result = alloc_strings(&strings, arguments.strings_count);
    if (alloc_result != 0) {
        return alloc_result;
    }
    int reading_result = read_strings_from_file(arguments.input_filename, strings, arguments.strings_count);
    if (reading_result != 0) {
        return reading_result;
    }
    sort(arguments, strings);
    int writing_result = write_string_to_file(arguments.output_filename, strings, arguments.strings_count);
    if (writing_result != 0) {
        return writing_result;
    }
    dealloc_strings(&strings, arguments.strings_count);
    return 0;
}