#include "headers.h"
#include "sortings.h"
#include "vector.h"

int main(int argc, char* argv[]) {
  system("chcp 1251");
  // проверка количества аргументов
  if (argc != NUM_ARGS) {
    printf("Не верно введены параметры!\n");
    return -1;
  }
  // открыть исходный файл
  FILE* src = fopen(argv[INPUT], "r");
  if (!src) {
    printf("Ошибка! Файл \"%s \" не удалось открыть!\n", argv[INPUT]);
    return -1;
  }
  // чтение файла
  char word[100];
  struct vector vec = new_vector();
  while ((fgets(word, 100, src)) != NULL) {
    push_back(&vec, word, strlen(word));
  }
  // сортировка файла
  if (strcmp(argv[COMP], "des") == 0)  // если тип компаратора des
  {
    if (strcmp(argv[ALG], "bubble") == 0)  // если алгоритм bubble
    {
      bubble(&vec, des);
    } else if (strcmp(argv[ALG], "insertion") ==
               0)  // если алгоритм insertion des
    {
      insertion(&vec, des);
    } else if (strcmp(argv[ALG], "merge") == 0)  // если алгоритм merge
    {
      merge(&vec, des);
    } else if (strcmp(argv[ALG], "quick") == 0)  // если алгоритм quick
    {
      quick(&vec, des);
    } else if (strcmp(argv[ALG], "radix") == 0)  // если алгоритм radix
    {
      radix(&vec, 1);
    } else {
      printf("Данный алгоритм сортировки не поддерживается!\n");
      return -1;
    }
  } else {
    if (strcmp(argv[ALG], "bubble") == 0)  // если алгоритм bubble
    {
      bubble(&vec, asc);
    } else if (strcmp(argv[ALG], "insertion") == 0)  // если алгоритм insertion
    {
      insertion(&vec, asc);
    } else if (strcmp(argv[ALG], "merge") == 0)  // если алгоритм merge
    {
      merge(&vec, asc);
    } else if (strcmp(argv[ALG], "quick") == 0)  // если алгоритм quick
    {
      quick(&vec, asc);
    } else if (strcmp(argv[ALG], "radix") == 0)  // если алгоритм radix
    {
      radix(&vec, 0);
    } else {
      printf("Данный алгоритм сортировки не поддерживается!\n");
      return -1;
    }
  }
  // открыть файл результата
  FILE* dst = fopen(argv[OUTPUT], "w");
  if (!dst) {
    printf("Ошибка! Файл \"%s \" не удалось открыть!\n", argv[OUTPUT]);
    return -1;
  }
  // запись рузультата сортировки в файл
  int size_str = atoi(argv[SIZE]);
  int len = vec.size < size_str ? vec.size : size_str;
  for (int i = 0; i < len; ++i) {
    fwrite(vec.data[i], strlen(vec.data[i]), 1, dst);
  }
  // удалить выделенную память
  remove_vector(&vec);
  printf("Программа успешно выполнена!\n");
  return 0;
}
