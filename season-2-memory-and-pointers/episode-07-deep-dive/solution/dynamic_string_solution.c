/**
 * OPERATION MOONLIGHT
 * Episode 07: Deep Dive
 * Промежуточная задача: Dynamic String Reader
 * 
 * Чтение строки неизвестной длины с использованием realloc
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Читает строку из stdin динамически (с realloc)
 * @return Указатель на строку (нужно освободить вызывающим!)
 */
char* read_dynamic_string() {
    int capacity = 10;  // Начальная вместимость
    int length = 0;
    char *buffer = malloc(capacity);
    
    if (!buffer) {
        return NULL;
    }
    
    printf("Enter text (press Enter to finish):\n> ");
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Если буфер заполнен, увеличиваем
        if (length + 1 >= capacity) {
            capacity *= 2;  // Удваиваем размер
            char *temp = realloc(buffer, capacity);
            
            if (!temp) {
                free(buffer);
                return NULL;
            }
            buffer = temp;
            
            printf("[Realloc: %d → %d bytes]\n", capacity/2, capacity);
        }
        
        buffer[length++] = c;
    }
    
    buffer[length] = '\0';
    
    // Опционально: сжать до точного размера
    char *final = realloc(buffer, length + 1);
    if (final) {
        buffer = final;
    }
    
    return buffer;
}

int main() {
    printf("╔══════════════════════════════════════╗\n");
    printf("║  DYNAMIC STRING READER               ║\n");
    printf("╚══════════════════════════════════════╝\n\n");
    
    char *str = read_dynamic_string();
    
    if (!str) {
        fprintf(stderr, "✗ Memory allocation failed!\n");
        return 1;
    }
    
    printf("\n✓ String read successfully!\n");
    printf("Length: %zu characters\n", strlen(str));
    printf("Content: \"%s\"\n", str);
    
    // КРИТИЧЕСКИ ВАЖНО: освободить память!
    free(str);
    
    printf("\n✓ Memory freed. No leaks!\n");
    
    return 0;
}

