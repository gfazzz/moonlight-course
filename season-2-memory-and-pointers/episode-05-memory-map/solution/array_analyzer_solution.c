/**
 * OPERATION MOONLIGHT
 * Episode 05: Memory Map
 * Промежуточная задача: Array Analyzer
 * 
 * Анализ массива байтов — подготовка к парсингу memory dump
 */

#include <stdio.h>

int main() {
    // Массив байтов для анализа
    unsigned char data[] = {0x42, 0x1A, 0xFF, 0x00, 0x7E};
    int size = sizeof(data) / sizeof(data[0]);
    
    printf("=== BYTE ARRAY ANALYSIS ===\n");
    printf("Array size: %d bytes\n\n", size);
    
    // 1. Вывод каждого байта в hex и decimal
    for (int i = 0; i < size; i++) {
        printf("Byte %d: 0x%02X (%d decimal)\n", i, data[i], data[i]);
    }
    printf("\n");
    
    // 2. Вычисление суммы
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += data[i];
    }
    
    // 3. Поиск минимума
    unsigned char min = data[0];
    int min_index = 0;
    for (int i = 1; i < size; i++) {
        if (data[i] < min) {
            min = data[i];
            min_index = i;
        }
    }
    
    // 4. Поиск максимума
    unsigned char max = data[0];
    int max_index = 0;
    for (int i = 1; i < size; i++) {
        if (data[i] > max) {
            max = data[i];
            max_index = i;
        }
    }
    
    // 5. Вычисление среднего
    double average = (double)sum / size;
    
    // Вывод статистики
    printf("Statistics:\n");
    printf("Sum of all bytes: %d\n", sum);
    printf("Min byte: 0x%02X (%d)\n", min, min);
    printf("Max byte: 0x%02X (%d)\n", max, max);
    printf("Average: %.1f\n\n", average);
    
    printf("Index of max: %d\n", max_index);
    printf("Index of min: %d\n", min_index);
    
    return 0;
}

