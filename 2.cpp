#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 100
#define COLS 200

int main() {
    int* array = NULL;
    
    // Выделение непрерывного блока памяти для всего массива
    array = (int*)malloc(ROWS * COLS * sizeof(int));
    if (array == NULL) {
        printf("Ошибка выделения памяти для массива!\n");
        return 1;
    }
    
    // Инициализация генератора случайных чисел
    srand(time(NULL));
    
    // Заполнение массива случайными числами от -3 до 10
    for (int i = 0; i < ROWS * COLS; i++) {
        array[i] = -3 + rand() % 14;  // 14 = 10 - (-3) + 1
    }
    
    // Вывод первых 5x10 элементов для проверки
    printf("Первые 5x10 элементов массива:\n");
    for (int i = 0; i < 5 && i < ROWS; i++) {
        for (int j = 0; j < 10 && j < COLS; j++) {
            printf("%3d ", array[i * COLS + j]);
        }
        printf("\n");
    }
    printf("...\n");
    
    // Подсчет статистики для проверки диапазона
    int min_val = 100, max_val = -100;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int val = array[i * COLS + j];
            if (val < min_val) min_val = val;
            if (val > max_val) max_val = val;
        }
    }
    printf("\nСтатистика:\n");
    printf("Минимальное значение в массиве: %d\n", min_val);
    printf("Максимальное значение в массиве: %d\n", max_val);
    
    // Освобождение памяти
    free(array);
    
    printf("\nПамять успешно освобождена.\n");
    
    return 0;
}