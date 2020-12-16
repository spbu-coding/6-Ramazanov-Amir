#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sortings.h"

#define error(...) (fprintf(stderr, __VA_ARGS__))

typedef void (*sorting_algorithm_t)(strings_array_t, array_size_t, comparator_func_t);

int asc(const char *str1, const char *str2) {
    return strcmp(str1, str2);
}

int des(const char *str1, const char *str2) {
    return -strcmp(str1, str2);
}

void free_strings_array(strings_array_t strings_array, size_t size) {
    for(size_t i = 0; i < size; i++)
        free(strings_array[i]);
    free(strings_array);
    strings_array = NULL;
}

int get_strings_count(char *arg, size_t *strings_count) {
    char *pEnd;
    *strings_count = strtoull(arg, &pEnd, 10);

    if (*pEnd != '\0') return -1;
    else return 0;
}

sorting_algorithm_t get_sorting_algorithm(char *arg) {
    sorting_algorithm_t sorting_algorithm = NULL;

    if(strcmp(arg, "bubble") == 0) sorting_algorithm = bubble;
    else if(strcmp(arg, "insertion") == 0) sorting_algorithm = insertion;
    else if(strcmp(arg, "merge") == 0) sorting_algorithm = merge;
    else if(strcmp(arg, "quick") == 0) sorting_algorithm = quick;
    else if(strcmp(arg, "radix") == 0) sorting_algorithm = radix;

    return sorting_algorithm;
}

comparator_func_t get_comparator(char *arg) {
    comparator_func_t comparator = NULL;

    if(strcmp(arg, "asc") == 0) comparator = asc;
    else if(strcmp(arg, "des") == 0) comparator = des;

    return comparator;
}

int get_strings_from_file(FILE* file, size_t max_string_length, size_t strings_count, strings_array_t strings_array) {
    size_t i;
    for(i = 0; i < strings_count && !feof(file); i++) {
        if(fgets(strings_array[i], max_string_length, file) == NULL) {
            error("fgets() error in get_strings_from_file\n");
            return -3;
        }
    }
    if(i < strings_count && feof(file)) {
        error("Strings count arg > strings count in file\n");
        return -1;
    }
    else
        return 0;
}

int put_strings_in_file(FILE* file, size_t strings_count, strings_array_t strings_array) {
    if(strings_count > 0) {
        for(size_t i = 0; i < strings_count; i++) {
            if(fputs(strings_array[i], file) == EOF) {
                error("Error with fputs() in output file\n");
                return -1;
            }
            if(strcspn(strings_array[i], "\n") == strlen(strings_array[i])) {
                if(fputs("\n", file) == EOF) {
                    error("Error with fputs() in output file\n");
                    return -1;
                }
            }
        }
    }
    else {
        if(fputs("\n", file) == EOF) {
            error("Error with fputs() in output file\n");
            return -1;
        }
    }

    return 0;
}

int main(int argc, char **argv) {

    if (argc != 6) {
        error("Expected 5 command line arguments\n");
        return -1;
    }

    size_t strings_count;
    if (get_strings_count(argv[1], &strings_count) != 0) {
        error("Strings count must be >= 0 unsigned integer\n");
        return -1;
    }

    sorting_algorithm_t sorting_algorithm = get_sorting_algorithm(argv[4]);
    if(sorting_algorithm == NULL) {
        error("Not valid sorting algorithm");
        return -1;
    }

    comparator_func_t comparator = get_comparator(argv[5]);
    if(comparator == NULL) {
        error("Not valid comparator");
        return -1;
    }

    strings_array_t strings_array = (char**)malloc(sizeof(char*) * strings_count);
    if(strings_array == NULL) {
        error("Error with allocation memory error in main()");
        return -1;
    }
    else {
        for(size_t i = 0; i < strings_count; i++) {
            strings_array[i] = (char*)malloc(sizeof(char) * MAX_INPUT_STRING_SIZE);
            if(strings_array[i] == NULL) {
                free_strings_array(strings_array, i);
                error("Error with allocation memory error in main()");
                return -1;
            }
        }
    }

    FILE *in_file = fopen(argv[2], "r");
    if(in_file == NULL) {
        free_strings_array(strings_array, strings_count);
        error("Error with fopen() of input file\n");
        return -1;
    }

    if(get_strings_from_file(in_file, MAX_INPUT_STRING_SIZE, strings_count, strings_array) != 0) {
        fclose(in_file);
        free_strings_array(strings_array, strings_count);
        error("Error with reading file\n");
        return -1;
    }

    sorting_algorithm(strings_array, strings_count, comparator);

    FILE* out_file = fopen(argv[3], "w");
    if(out_file == NULL) {
        fclose(in_file);
        free_strings_array(strings_array, strings_count);
        error("Error with fopen() of output file\n");
        return -1;
    }

    if(put_strings_in_file(out_file, strings_count, strings_array) != 0) {
        fclose(in_file);
        fclose(out_file);
        free_strings_array(strings_array, strings_count);
        error("Error with writing file\n");
        return -1;
    }

    fclose(in_file);
    fclose(out_file);
    free_strings_array(strings_array, strings_count);

    return 0;
}
