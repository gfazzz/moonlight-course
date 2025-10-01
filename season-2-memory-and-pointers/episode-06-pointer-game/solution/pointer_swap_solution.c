/**
 * OPERATION MOONLIGHT
 * Episode 06: Pointer Game
 * Промежуточная задача: Pointer Swap
 * 
 * Обмен значений через указатели — фундаментальная операция
 */

#include <stdio.h>

/**
 * Меняет местами два значения через указатели
 * @param a Указатель на первое значение
 * @param b Указатель на второе значение
 */
void swap(int *a, int *b) {
    int temp = *a;  // Сохраняем значение по адресу a
    *a = *b;        // Записываем в a значение из b
    *b = temp;      // Записываем в b сохранённое значение
}

int main() {
    int x = 5, y = 10;
    
    printf("╔══════════════════════════════════════╗\n");
    printf("║  POINTER SWAP DEMONSTRATION          ║\n");
    printf("╚══════════════════════════════════════╝\n\n");
    
    printf("Before swap: x=%d, y=%d\n", x, y);
    
    // Вызываем swap, передавая адреса переменных
    swap(&x, &y);
    
    printf("After swap:  x=%d, y=%d\n", x, y);
    
    // Проверка
    if (x == 10 && y == 5) {
        printf("\n✓ Swap successful!\n");
        return 0;
    } else {
        printf("\n✗ Swap failed!\n");
        return 1;
    }
}

