/**
 * ═══════════════════════════════════════════════════════════════
 * Episode 13: Crypto Basics — Промежуточная задача
 * ═══════════════════════════════════════════════════════════════
 * 
 * ЗАДАЧА: Frequency Analyzer для Caesar Cipher
 * 
 * Цель: Подсчитать частоты букв в тексте и визуализировать их.
 *       Это подготовка к автоматическому взлому Caesar cipher.
 * 
 * Что реализовать:
 * 1. count_frequency() — подсчёт частот букв (A-Z)
 * 2. print_histogram() — ASCII-график частот
 * 3. find_most_frequent() — поиск самой частой буквы
 * 4. detect_caesar_shift() — автоопределение сдвига
 * 
 * Тестовые файлы:
 * - tests/sample_english.txt (обычный английский текст)
 * - tests/sample_caesar.txt (зашифровано Caesar shift=7)
 * 
 * Пример вывода:
 * ═══ FREQUENCY ANALYSIS ═══
 * Total letters: 1523
 * 
 * E: ██████████████ 12.7% (194)
 * T: ██████████     9.1%  (139)
 * A: █████████      8.2%  (125)
 * ...
 * Most frequent: E
 * 
 * Компиляция:
 * $ gcc -Wall -Wextra -std=c11 frequency_analyzer.c -o frequency_analyzer
 * 
 * Запуск:
 * $ ./frequency_analyzer tests/sample_english.txt
 * $ ./frequency_analyzer tests/sample_caesar.txt
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Подсчитывает частоты букв A-Z в тексте (игнорирует регистр)
 * 
 * @param text Входной текст
 * @param freq Массив [26] для хранения частот (freq[0] = A, freq[1] = B, ...)
 * @param total Указатель для записи общего количества букв
 */
void count_frequency(const char *text, int freq[26], int *total) {
    // TODO: Реализовать подсчёт частот
    // 1. Инициализировать freq[] нулями
    // 2. Для каждого символа text:
    //    - Преобразовать в верхний регистр toupper()
    //    - Если буква A-Z: freq[c - 'A']++
    //    - Увеличить *total
}

/**
 * Печатает ASCII-гистограмму частот букв
 * 
 * @param freq Массив частот [26]
 * @param total Общее количество букв
 * 
 * Пример:
 * E: ██████████████ 12.7% (194)
 * T: ██████████     9.1%  (139)
 */
void print_histogram(int freq[26], int total) {
    printf("\n═══ FREQUENCY ANALYSIS ═══\n");
    printf("Total letters: %d\n\n", total);
    
    // TODO: Реализовать печать гистограммы
    // Для каждой буквы A-Z:
    // 1. Если freq[i] > 0:
    //    - Вычислить процент: 100.0 * freq[i] / total
    //    - Нарисовать bar: █ (повторить пропорционально проценту)
    //    - Вывести: буква: bar процент (количество)
    // 
    // Hint: █ = Unicode U+2588, можно использовать printf("█")
}

/**
 * Находит самую частую букву в массиве частот
 * 
 * @param freq Массив частот [26]
 * @return Самая частая буква ('A'..'Z')
 */
char find_most_frequent(int freq[26]) {
    // TODO: Реализовать поиск максимума
    // 1. Найти индекс max_idx с наибольшим freq[max_idx]
    // 2. Вернуть 'A' + max_idx
    
    return 'E';  // Placeholder
}

/**
 * Автоматически определяет Caesar shift, предполагая что
 * самая частая буква в английском тексте — 'E'
 * 
 * @param freq Массив частот [26]
 * @return Предполагаемый shift (0-25)
 * 
 * Логика:
 * Если в шифротексте самая частая буква 'K', а в plaintext обычно 'E',
 * то shift = (K - E + 26) % 26 = (10 - 4 + 26) % 26 = 6
 */
int detect_caesar_shift(int freq[26]) {
    // TODO: Реализовать автоопределение shift
    // 1. Найти самую частую букву в шифротексте
    // 2. Предположить, что это 'E' (4) в plaintext
    // 3. Вычислить shift = (most_frequent - 4 + 26) % 26
    
    return 0;  // Placeholder
}

/**
 * Читает содержимое файла в строку
 */
char* read_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("fopen");
        return NULL;
    }
    
    // Определить размер файла
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Выделить память
    char *content = malloc(size + 1);
    if (!content) {
        fclose(f);
        return NULL;
    }
    
    // Прочитать файл
    fread(content, 1, size, f);
    content[size] = '\0';
    
    fclose(f);
    return content;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <text_file>\n", argv[0]);
        fprintf(stderr, "Example: %s tests/sample_english.txt\n", argv[0]);
        return 1;
    }
    
    // Прочитать файл
    char *text = read_file(argv[1]);
    if (!text) {
        return 1;
    }
    
    // Подсчитать частоты
    int freq[26] = {0};
    int total = 0;
    count_frequency(text, freq, &total);
    
    // Вывести гистограмму
    print_histogram(freq, total);
    
    // Найти самую частую букву
    char most_freq = find_most_frequent(freq);
    printf("\nMost frequent letter: %c (%.1f%%)\n", 
           most_freq, 100.0 * freq[most_freq - 'A'] / total);
    
    // Определить shift (если это Caesar cipher)
    int shift = detect_caesar_shift(freq);
    printf("Detected Caesar shift: %d\n", shift);
    
    if (shift > 0) {
        printf("\n💡 Hint: Try decrypting with shift=%d\n", shift);
    }
    
    free(text);
    return 0;
}

