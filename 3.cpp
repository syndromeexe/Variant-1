#include <stdio.h>

int main() {
    // Поиск машинного эпсилон для double
    double eps = 1.0;
    while (1.0 + eps > 1.0) {
        eps /= 2.0;
    }
    eps *= 2.0;  // Возвращаем последнее значение, при котором условие выполнялось
    
    // Поиск машинного нуля (минимального нормализованного числа)
    double min_normal = 1.0;
    double prev = min_normal;
    while (min_normal > 0) {
        prev = min_normal;
        min_normal /= 2.0;
    }
    min_normal = prev;
    
    printf("Результаты для типа double:\n");
    printf("Машинный эпсилон: %.10e\n", eps);
    printf("Машинный ноль (минимальное нормализованное число): %.10e\n", min_normal);
    
    // Проверки
    printf("\nПроверки:\n");
    printf("1.0 + eps/2 == 1.0: %s\n", (1.0 + eps/2 == 1.0) ? "да" : "нет");
    printf("1.0 + eps > 1.0:    %s\n", (1.0 + eps > 1.0) ? "да" : "нет");
    printf("min_normal > 0:     %s\n", (min_normal > 0) ? "да" : "нет");
    printf("min_normal/2 == 0:  %s\n", (min_normal/2 == 0) ? "да" : "нет");
    
    return 0;
}