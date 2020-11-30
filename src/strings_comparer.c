#include "sortings.h"

int des(char *first, char *second) { return strlen(first) < strlen(second); }

int asc(char *first, char *second) { return !(des(first, second)); }

// сортировка пузырьком
void bubble(struct vector *mas, int func_cmp(char *, char *)) {
  for (int i = 0; i < mas->size; ++i) {
    for (int j = 0; j < mas->size - i - 1; ++j) {
  if (func_cmp(mas->data[j], mas->data[j + 1])) {
        char *tmp = mas->data[j];
        mas->data[j] = mas->data[j + 1];
        mas->data[j + 1] = tmp;
      }
    }
  }
}

// сортировка вставками
void insertion(struct vector *mas, int func_cmp(char *, char *)) {
  for (int i = 1, item = 0; i < mas->size; ++i) {
    char *temp = mas->data[i];
    item = i - 1;
    while (item >= 0 && func_cmp(mas->data[item], temp)) {
      mas->data[item + 1] = mas->data[item];
      mas->data[item] = temp;
      --item;
    }
    mas->data[item + 1] = temp;
  }
}
/////////////////////////////////////////////////////////
void merge_sort(struct vector *mas, struct vector *copy, int head, int middle,
                int tail, int func_cmp(char *, char *)) {
  int temp = 0;
  int lower = head;
  int upper = middle + 1;

  while (lower <= middle && upper <= tail) {
	// вызов функции сравнения (больше или меньше)
    if (!func_cmp(mas->data[lower], mas->data[upper])) {
      emplace(copy, temp, mas->data[lower], strlen(mas->data[lower]));
      ++temp;
      ++lower;
    } else {
      emplace(copy, temp, mas->data[upper], strlen(mas->data[upper]));
      ++temp;
      ++upper;
    }
  }

  if (lower <= middle) {
    for (; lower <= middle; ++temp, ++lower) {
      emplace(copy, temp, mas->data[lower], strlen(mas->data[lower]));
    }
  } else {
    for (; upper <= tail; ++temp, ++upper) {
      emplace(copy, temp, mas->data[upper], strlen(mas->data[upper]));
    }
  }

  int pointer = head;
  for (temp = 0; pointer <= tail; ++pointer, ++temp) {
    emplace(mas, pointer, copy->data[temp], strlen(copy->data[temp]));
  }
}
void merge_sort_helper(struct vector *mas, struct vector *copy, int head,
                       int tail, int func_cmp(char *, char *)) {
  if (head == tail) {
    return;
  }

  int middle = (head + tail) / 2;
  // рекурсия вызова
  merge_sort_helper(mas, copy, head, middle, func_cmp);
  merge_sort_helper(mas, copy, middle + 1, tail, func_cmp);

  merge_sort(mas, copy, head, middle, tail, func_cmp);
}
// сортировка слиянием
void merge(struct vector *mas, int func_cmp(char *, char *)) {
  struct vector copy = copy_vector(mas);
  merge_sort_helper(mas, &copy, 0, mas->size - 1, func_cmp);
  remove_vector(&copy);
}
// #######################################################################
void quick_help(struct vector *vec, const int size,
                int func_cmp(char *, char *)) {
  const int len = size;
  struct vector L = new_vector_size(len);
  struct vector R = new_vector_size(len);
  int is_remove = 0;
  if (len > 1) {
    int middle = len / 2;
    int j = 0;
    int k = 0;
    char *temp = vec->data[middle];
    for (int i = 0; i < len; ++i) {
      if (i != middle) {
		// вызов функции сравнения (больше или меньше)
        if (!func_cmp(vec->data[i], temp)) {
          emplace(&L, j, vec->data[i], strlen(vec->data[i]));
          ++j;
        } else {
          emplace(&R, k, vec->data[i], strlen(vec->data[i]));
          ++k;
        }
      }
    }
    // рекурсия вызовов
    quick_help(&L, j, func_cmp);
    quick_help(&R, k, func_cmp);
    for (int cnt = 0; cnt < len; ++cnt) {
      if (cnt < j) {
        vec->data[cnt] = L.data[cnt];
      } else if (cnt == j) {
        vec->data[cnt] = temp;
      } else {
        vec->data[cnt] = R.data[cnt - (j + 1)];
      }
    }
    is_remove = 1;
  }
  // удалить ветора если не были удалены
  if (!is_remove) {
    remove_vector(&R);
    remove_vector(&L);
  }
}
// быстрая сортировка
void quick(struct vector *vec, int func_cmp(char *, char *)) {
  quick_help(vec, vec->size, func_cmp);
}
// ##############################################
// Узел связанного списка, содержащий строку и длину
struct bucket_entry {
  char *str;
  size_t len;
  struct bucket_entry *next;
};
typedef struct bucket_entry bucket_entry;
// Связанный список
struct bucket {
  bucket_entry *head;
  bucket_entry *tail;
};
typedef struct bucket bucket;
// Инициализировать сегменты
static void init_buckets(bucket *buckets) {
  unsigned int b;
  for (b = 0; b < 256; ++b) {
    buckets[b].head = 0;
    buckets[b].tail = 0;
  }
}

// запись в список длины строк. вернуть размер самой длинной строки
static size_t init_entries(char **strings, size_t len, bucket_entry *entries) {
  unsigned int s;
  size_t maxlen = 0;
  for (s = 0; s < len; ++s) {
    entries[s].str = strings[s];
    entries[s].len = strlen(strings[s]);
    if (entries[s].len > maxlen) {
      maxlen = entries[s].len;
    }
    if (s < len - 1) {
      entries[s].next = &entries[s + 1];
    } else {
      entries[s].next = 0;
    }
  }
  return maxlen;
}

// Поместите строки в сегменты в соответствии с символом c справа
void bucket_strings(bucket_entry *head, bucket *buckets, unsigned int c) {
  bucket_entry *current = head;
  while (current != 0) {
    bucket_entry *next = current->next;
    current->next = 0;
    int pos = current->len - 1 - c;
    unsigned char ch;
    if (pos < 0) {
      ch = 0;
    } else {
      ch = current->str[pos];
    }
    if (buckets[ch].head == 0) {
      buckets[ch].head = current;
      buckets[ch].tail = current;
    } else {
      buckets[ch].tail->next = current;
      buckets[ch].tail = buckets[ch].tail->next;
    }
    current = next;
  }
}

// Объединить сегменты в единый связанный список
bucket_entry *merge_buckets(bucket *buckets) {
  bucket_entry *head = 0;
  bucket_entry *tail = 0;
  unsigned int b;
  for (b = 0; b < 256; ++b) {
    if (buckets[b].head != 0) {
      if (head == 0) {
        head = buckets[b].head;
        tail = buckets[b].tail;
      } else {
        tail->next = buckets[b].head;
        tail = buckets[b].tail;
      }
    }
  }
  return head;
}
// получить размер списка сегментов
size_t size_list_bucket(const bucket_entry *head) {
  const bucket_entry *current ;
  size_t size = 0;
  for (current = head; current != 0; current = current->next, ++size) {
  }
  return size;
}
// получить строки для вывода
void copy_list(const bucket_entry *head, char **strings) {
  const bucket_entry *current ;
  unsigned int s;
  for (current = head, s = 0; current != 0; current = current->next, ++s) {
    strings[s] = current->str;
  }
}

void radix_help(char **strings, size_t len, int comp) {
  size_t maxlen ;
  unsigned int c ;
  bucket_entry *head ;
  bucket_entry *entries = malloc(len * sizeof(bucket_entry));
  bucket *buckets = malloc(256 * sizeof(bucket));
  init_buckets(buckets);
  maxlen = init_entries(strings, len, entries);
  head = &entries[0];
  // главный цикл
  for (c = 0; c < maxlen; ++c) {
    bucket_strings(head, buckets, c);
    head = merge_buckets(buckets);
    init_buckets(buckets);
  }
  if (comp) {
	// если по убыванию
    size_t size = size_list_bucket(head) - 1;
    const bucket_entry *current ;
    for (current = head; current != 0; current = current->next, --size) {
      strings[size] = current->str;
    }
  } else {
	// по возрастанию
    copy_list(head, strings);
  }
  free(buckets);
  free(entries);
}
void radix(struct vector *vec, int comp) {
  radix_help(vec->data, vec->size, comp);
}
