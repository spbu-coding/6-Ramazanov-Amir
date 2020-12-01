#include "sortings.h"
#include "headers.h"

#define COUNT_ELEMENTS_IN_CHARSET 128


void bubble(strings_array_t arr, array_size_t array_size, comparator_func_t comparator) {
	for (int i = 0; i < (int) array_size - 1; i++) {
		for (int j = 0; j < (int) array_size - 1 - i; j++) {
			if(comparator(arr[j], arr[j + 1]) == 1) {
				swap(&arr[j], &arr[j + 1]);
			}
		}
	}
}

void insertion(strings_array_t arr, array_size_t array_size, comparator_func_t comparator) {
	for (int i = 1; i < (int) array_size; i++) {
		int j = i - 1;
		while(j >= 0 && comparator(arr[j], arr[j + 1]) == 1) {
			swap(&arr[j], &arr[j + 1]);
			j--;
		}
	}
}

void merge(strings_array_t arr, array_size_t array_size, comparator_func_t comparator) {
	int mid = array_size / 2;
	if (array_size % 2 == 1) {
		mid++;
	}
	strings_array_t intermediate_arr;
	intermediate_arr = (strings_array_t)malloc( sizeof(strings_array_t) * array_size);
	for (int i = 0; i < (int) array_size; ++i) {
		intermediate_arr[i] = (char *)malloc(sizeof(char) * (MAX_INPUT_STRING_SIZE + 1));
	}
	int step;
	int h = 1; //шаг
	while (h < (int) array_size) {
		step = h;
		int i = 0;
		int j = mid;
		int k = 0;
		while (step <= mid) {
			while ((i < step) && (j < (int) array_size) && j < (mid + step)) {
				if (comparator(arr[i], arr[j]) == 2) {
					intermediate_arr[k] = arr[i];
					i++; k++; 
				}
				else {
					intermediate_arr[k] = arr[j];
					j++; k++; 
				}
			}
			while (i < step) {
				intermediate_arr[k] = arr[i];
				i++; k++; 
			}
			while (j < (mid + step) && (j < (int) array_size)) {
				intermediate_arr[k] = arr[j];
				j++; k++;
			}
			step += h;
		}
		h *= 2;
		for (int i = 0; i < (int) array_size; i++) {
			arr[i] = intermediate_arr[i];
		}
	}
}

int partition(strings_array_t arr, int left, int right, comparator_func_t comparator) {
	char* pivot = arr[(left + right) / 2];
	int i = left;
	int j = right;
	while (1) {
		while (comparator(arr[i], pivot) == 2) i++;
		while (comparator(arr[j], pivot) == 1) j--;
		if (i >= j) {
			if (comparator == string_compare_asc) return j;
			else return i;
		} 
		swap(&arr[i], &arr[j]);
	}
}

void quick1(strings_array_t arr, int left, int right, comparator_func_t comparator) {
	if (left < right) {
		int it = partition(arr, left, right, comparator);
		quick1(arr, left, it - 1, comparator);
		quick1(arr, it + 1, right, comparator);
	}
}

void quick(strings_array_t arr, array_size_t array_size, comparator_func_t comparator) {
	quick1(arr, 0, array_size - 1, comparator);
}

void radix(strings_array_t arr, array_size_t array_size, comparator_func_t comparator) {
	int max_string_size = strlen(arr[0]);
	for (int i = 1; i < (int) array_size; i++) {
		if ((int) strlen(arr[i]) > max_string_size) max_string_size = strlen(arr[i]);
	}
	
	for (int i = max_string_size - 1; i >= 0; i--) {
		char *intermediate_arr[array_size];
		int count[COUNT_ELEMENTS_IN_CHARSET] = { 0 };

		for (int j = 0; j < (int) array_size; j++) {
			count[(int) arr[j][i]]++;
		}
		for (int j = 1; j < COUNT_ELEMENTS_IN_CHARSET; j++) {
			count[j] += count[j - 1];
		}
		for (int j = array_size - 1; j >= 0; j--) {
			intermediate_arr[count[(int) arr[j][i]] - 1] = arr[j];
			count[(int) arr[j][i]]--;
		}
		for (int j = array_size - 1; j >= 0; j--) {
			if (i > 0 || comparator == string_compare_asc) {
				arr[j] = intermediate_arr[j];
			}
			else {
				arr[array_size - 1 - j] = intermediate_arr[j];
			}
		}
	}
}
