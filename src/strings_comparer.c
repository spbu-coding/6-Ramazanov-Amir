#include "string_compare.h"

char* algorithms[] = {"bubble", "insertion", "merge", "quick", "radix"};

char* comparators[] = {"asc", "des"};

void swap(char** first_str, char** second_str) {
	char* for_swap = *first_str;
    *first_str = *second_str;
    *second_str = for_swap;
}

int string_compare_asc(const char* string1, const char* string2) {
	int string_length;
	if (strlen(string1) > strlen(string2)) string_length = strlen(string2);
	else string_length = strlen(string1);
	for (int i = 0; i < string_length; i++) {
		if (string1[i] > string2[i]) {
			return 1;
		}
		if (string1[i] < string2[i]) {
			return 2;
		}
	}
	if (strlen(string1) > strlen(string2)) return 1;         //необязательно, можно удалить
	if (strlen(string1) < strlen(string2)) return 2;
	return 0;
}

int string_compare_des(const char* string1, const char* string2) {
	int string_length;
	if (strlen(string1) > strlen(string2)) string_length = strlen(string2);
	else string_length = strlen(string1);
	for (int i = 0; i < string_length; i++) {
		if (string1[i] > string2[i]) {
			return 2;
		}
		if (string1[i] < string2[i]) {
			return 1;
		}
	}
	if (strlen(string1) > strlen(string2)) return 2;         //необязательно, можно удалить
	if (strlen(string1) < strlen(string2)) return 1;
	return 0;
}

void free_arr(strings_array_t arr, int strings_count) {
	for (int i = 0; i < strings_count; ++i) {
		free(arr[i]);
	}
	free(arr);
}

strings_array_t Read_file(char* filename, int* return_value, int strings_count) {
	FILE* input_file;
	if ((input_file = fopen(filename, "rb")) == NULL) {
        printf("Can not open file %s.\n", filename);
        *return_value = -1;
        return NULL;
    }
    strings_array_t arr;
    arr = (strings_array_t)malloc( sizeof(strings_array_t) * strings_count);
    if (!arr) {
    	fclose(input_file);
    	*return_value = -1;
		return NULL;
	}
	for (int i = 0; i < strings_count; ++i) {
		arr[i] = (char *)calloc(sizeof(char), (MAX_INPUT_STRING_SIZE + 1));
		if (!arr[i]) {
			free_arr(arr, i);
			fprintf(stderr, "Cannot allocate memory for %d string \n", i);
			fclose(input_file);
			*return_value = -1;
			return NULL;
		}
		if (fgets(arr[i], MAX_INPUT_STRING_SIZE + 1, input_file) == NULL) {
			fprintf(stderr, "The number of strings in input file is less than the specified number\n");
			fclose(input_file);
			*return_value = -1;
			return NULL;
		}
	}
	fclose(input_file);

	int string_length = strlen(arr[strings_count - 1]);
	if (arr[strings_count - 1][string_length - 1] != '\n') {
		if (string_length == MAX_INPUT_STRING_SIZE) {
            fprintf(stderr, "The last line contains an unsupported number of characters)\n");
            *return_value = -1;
			return NULL;
        }
        arr[strings_count - 1][string_length] = '\n';
        arr[strings_count - 1][string_length + 1] = '\0';
	}

	return arr;
}

void Write_file(char* filename, int* return_value, int strings_count,  strings_array_t arr) {
	FILE* output_file;
	if ((output_file = fopen(filename, "wb")) == NULL) {
        printf("Can not create file %s.\n", filename);
        *return_value = -1;
        return;
    }
    for (int i = 0; i < strings_count; ++i) {
		fputs( arr[i], output_file );
	}
	fclose(output_file);
}

int checking_parameters(int argc) {
	if (argc != 6) {
		printf("Error! Call function like this: \n");
		printf("1. The number of strings to sort\n");
		printf("2. The name of the input file\n");
		printf("3. The name of the output file\n");
		printf("4. The name of the algorithm\n");
		printf("5. The name of the comparator\n");
		return -1;
	}
	return 0;
}

int main(int argc, char* argv[]) {
	int return_value = 0;
	comparator_func_t comparator;
	if (checking_parameters(argc) != 0) return -1;
	strings_array_t arr = Read_file(argv[2], &return_value, atoi(argv[1]));
	if (return_value != 0) return -1;

	if (strncmp(argv[5], comparators[0], ASC_LENGTH) == 0) {
		comparator = string_compare_asc;
	}
	else if (strncmp(argv[5], comparators[1], DES_LENGTH) == 0) {
		comparator = string_compare_des;
	}
	else {
		printf("Error! Available comparators: asc, des.\n");
		return -1;
	}

	if (strncmp(argv[4], algorithms[0], BUBBLE_LENGTH) == 0) {
		bubble(arr, atoi(argv[1]), comparator);
	}
	else if (strncmp(argv[4], algorithms[1], INSERTION_LENGTH) == 0) {
		insertion(arr, atoi(argv[1]), comparator);
	}
	else if (strncmp(argv[4], algorithms[2], MERGE_LENGTH) == 0) {
		merge(arr, atoi(argv[1]), comparator);
	}
	else if (strncmp(argv[4], algorithms[3], QUICK_LENGTH) == 0) {
		quick(arr, atoi(argv[1]), comparator);
	}
	else if (strncmp(argv[4], algorithms[4], RADIX_LENGTH) == 0) {
		radix(arr, atoi(argv[1]), comparator);
	}
	else {
		printf("Error! Available algorithms: bubble, insertion, merge, quick, radix.\n");
		return -1;
	}

	Write_file(argv[3], &return_value, atoi(argv[1]), arr);
	if (return_value != 0) return -1;
	return 0;
}
