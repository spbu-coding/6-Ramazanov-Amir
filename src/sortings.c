#include "sortings.h"
#include "headers.h"

void swap(char **str1, char **str2) {
    char *tmp = *str1;
    *str1 = *str2;
    *str2 = tmp;
}

array_size_t get_max_length(strings_array_t strings, size_t strings_count) {
    size_t max = strlen(strings[0]);
    for (size_t i = 0; i < strings_count; ++i) {
        size_t tmp_size = strlen(strings[i]);
        if (tmp_size > max) {
            max = tmp_size;
        }
    }
    return max;
}

void count_sort(strings_array_t array, size_t size, size_t k, comparator_func_t cmp) {
    strings_array_t ram = malloc(sizeof(char *) * size);
    int *memory = malloc(sizeof(int) * 257);
    for (int i = 0; i < 257; ++i) {
        memory[i] = 0;
    }
    for (size_t i = 0; i < size; ++i) {
        int tmp = k < strlen(array[i]) ? (int)(unsigned char)array[i][k] + 1 : 0;
        memory[tmp]++;
    }
    if (cmp("a", "b") < 0) {
        for (int i = 1; i < 257; ++i) {
            memory[i] += memory[i - 1];
        }
    } else {
        for (int i = 257 - 2; i >=0; --i) {
            memory[i] += memory[i + 1];
        }
    }
    for (int i = (int)size - 1; i >= 0; --i) {
        ram[memory[k < strlen(array[i]) ? (int)(unsigned char)array[i][k] + 1 : 0] - 1] = array[i];
        memory[k < strlen(array[i]) ? (int)(unsigned char)array[i][k] + 1 : 0]--;
    }
    for (size_t i = 0; i < size; ++i) {
        array[i] = ram[i];
    }
    free(ram);
    free(memory);
}

void bubble(strings_array_t array, array_size_t size, comparator_func_t cmp) {
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = i; j < size; ++j) {
            if (cmp(array[j], array[i]) < 0) {
                swap(&array[i], &array[j]);
            }
        }
    }
}

void insertion(strings_array_t array, array_size_t size, comparator_func_t cmp) {
    for (array_size_t i = 1; i < size; i++) {
        char *tmp = array[i];
        for (int j = (int)i - 1; j >= 0; j--) {
            if (cmp(array[j], tmp) < 0) {
                break;
            }
            array[j + 1] = array[j];
            array[j] = tmp;
        }
    }
}

void merge(strings_array_t a, array_size_t n, comparator_func_t cmp) {
    array_size_t step = 1;
    strings_array_t  temp = malloc(n * sizeof(char *));
    for (array_size_t i = 0; i < n; ++i) {
        temp[i] = malloc(sizeof(char) * MAX_INPUT_STRING_SIZE);
    }
    while (step < n) {
        array_size_t index = 0;
        array_size_t l = 0;
        array_size_t m = l + step;
        array_size_t r = l + step * 2;
        do {
            m = m < n ? m : n;
            r = r < n ? r : n;
            array_size_t i1 = l, i2 = m;
            for (; i1 < m && i2 < r; ) {
                if (cmp(a[i1], a[i2]) < 0) {
                    temp[index++] = a[i1++];
                } else {
                    temp[index++] = a[i2++];
                }
            }
            while (i1 < m) {
                temp[index++] = a[i1++];
            }
            while (i2 < r) {
                temp[index++] = a[i2++];
            }
            l += step * 2;
            m += step * 2;
            r += step * 2;
        } while (l < n);
        for (array_size_t i = 0; i < n; i++) {
            a[i] = temp[i];
        }
        step *= 2;
    }
    for (array_size_t i = 0; i <= n; ++i) {
        free(temp[i]);
    }
    free(temp);
}

void quick_split(strings_array_t array, unsigned int beg, const unsigned int end, comparator_func_t cmp) {
    while (beg < end) {
        if ((array[beg] <= array[(beg + end - 1) / 2] && array[(beg + end - 1) / 2] <= array[end - 1]) || (array[end - 1] <= array[(beg + end - 1) / 2] && array[(beg + end - 1) / 2] <= array[beg])) {
            swap(&array[beg], &array[(beg + end - 1) / 2]);
        } else if ((array[beg] <= array[end - 1] && array[end - 1] <= array[(beg + end - 1) / 2]) || (array[(beg + end - 1) / 2] <= array[end - 1] && array[end - 1] <= array[beg])) {
            swap(&array[beg], &array[end - 1]);
        }
        unsigned int left = beg, mid = beg + 1, right = end;
        for (unsigned int i = 0; i < end - beg - 1; i++) {
            const int cmp_result = cmp(array[mid], array[beg]);
            if (cmp_result < 0) {
                left++;
                swap(&array[mid], &array[left]);
                mid++;
            } else if (cmp_result > 0) {
                right--;
                swap(&array[mid], &array[right]);
            } else {
                mid++;
            }
        }
        swap(&array[beg], &array[left]);
        quick_split(array, beg, left, cmp);
        beg = right;
    }
}

void quick(strings_array_t strings_array, array_size_t array_size, comparator_func_t comparator_func) {
    int left = 0;
    int right = array_size - 1;
    char *mid, *temp;
    mid = strings_array[array_size / 2];
    do {
        while(comparator_func(strings_array[left],mid)<0) left++;
        while(comparator_func(strings_array[right],mid)>0) right--;
        if (left <= right) {
            temp = strings_array[left];
            strings_array[left] = strings_array[right];
            strings_array[right] = temp;
            left++;
            right--;
        }
    } while (left <= right);
    if(right > 0)
        quick(strings_array, right + 1, comparator_func);
    if (left < (int)array_size)
        quick(&strings_array[left], array_size - left, comparator_func);
}

void radix(strings_array_t strings_array, array_size_t array_size, comparator_func_t comparator_func) {

    int addit_arr[array_size];
    int max_addit_arr = 0;
    for (unsigned int i = 0; i < array_size; i++) {
        addit_arr[i] = strlen(strings_array[i]) - 1;
        if (addit_arr[i] > max_addit_arr)  max_addit_arr = addit_arr[i];
    }
    for (int i = (int) max_addit_arr - 1; i >= 0; i--) {
        unsigned int pocket[256] = {0};
        for (unsigned int j = 0; j < array_size; j++)
            if ((int) addit_arr[j] - 1 >= i)
                pocket[(unsigned int) strings_array[j][i]]++;
            else pocket[0]++;

        if (comparator_func("a", "b") < 0)
            for (unsigned int j = 1; j < 256; j++)
                pocket[j] += pocket[j - 1];
        else
            for (int j = 256 - 2; j >= 0; j--)
                pocket[j] += pocket[j + 1];

        char* str_res_arr[array_size];
        int int_res_arr[array_size];
        for (int j = (int) array_size - 1; j >= 0; j--) {
            if ((int) addit_arr[j] - 1 >= i) {
                str_res_arr[(pocket[(unsigned int) strings_array[j][i]]) - 1] = strings_array[j];
                int_res_arr[(pocket[(unsigned int) strings_array[j][i]]--) - 1] = addit_arr[j];
            } else {
                str_res_arr[(pocket[0]) - 1] = strings_array[j];
                int_res_arr[(pocket[0]--) - 1] = addit_arr[j];
            }
        }
        memcpy(strings_array, str_res_arr, array_size * sizeof(char*));
        memcpy(addit_arr, int_res_arr, array_size * sizeof(size_t));
    }
}
