#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 100
#define COLS 200

int main() {
    float *array = NULL;
    
    // Выделение непрерывного блока памяти для всего массива
    array = (float*)malloc(ROWS * COLS * sizeof(float));
    if (array == NULL) {
        printf("Ошибка выделения памяти для массива!\n");
        return 1;
    }
    
    // Инициализация генератора случайных чисел
    srand(time(NULL));
    
    // Заполнение массива случайными числами от -1 до 1
    for (int i = 0; i < ROWS * COLS; i++) {
        array[i] = -1.0f + (rand() % 20001) / 10000.0f;
    }
    
    // Вывод первых 5x10 элементов для проверки
    printf("Первые 5x10 элементов массива:\n");
    for (int i = 0; i < 5 && i < ROWS; i++) {
        for (int j = 0; j < 10 && j < COLS; j++) {
            printf("%7.4f ", array[i * COLS + j]);
        }
        printf("\n");
    }
    printf("...\n");
    
    // Освобождаем память
    free(array);
    
    printf("\nПрограмма успешно завершена. Память освобождена.\n");
    
    return 0;
}