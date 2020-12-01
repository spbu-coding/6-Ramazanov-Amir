#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sortings.h"

#define BUBBLE_LENGTH 6
#define INSERTION_LENGTH 9
#define MERGE_LENGTH 5
#define QUICK_LENGTH 5
#define RADIX_LENGTH 5
#define ASC_LENGTH 3
#define DES_LENGTH 3

void swap(char** first_str, char** second_str);

int string_compare_asc(const char* string1, const char* string2);

int string_compare_des(const char* string1, const char* string2);

void free_arr(strings_array_t arr, int strings_count);

strings_array_t Read_file(char* filename, int* return_value, int strings_count);

void Write_file(char* filename, int* return_value, int strings_count,  strings_array_t arr);

int checking_parameters(int argc);

int main(int argc, char* argv[]);

#endi
