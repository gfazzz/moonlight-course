/**
 * ═══════════════════════════════════════════════════════════════
 * Episode 13: Crypto Basics — Промежуточная задача (РЕШЕНИЕ)
 * ═══════════════════════════════════════════════════════════════
 * 
 * Frequency Analyzer — Solution
 * 
 * Реализует:
 * - Подсчёт частот букв A-Z
 * - ASCII-гистограмму частот
 * - Поиск самой частой буквы
 * - Автоопределение Caesar shift (предполагая E = самая частая)
 * 
 * Компиляция: gcc -std=c11 -Wall -Wextra -O2 frequency_analyzer_solution.c
 * Использование: ./a.out <text_file>
 * 
 * @author MOONLIGHT Course
 * @version 2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ═══════════════════════════════════════════════════════════════
// Константы
// ═══════════════════════════════════════════════════════════════

#define ALPHABET_SIZE 26
#define EXPECTED_MOST_FREQUENT 'E'  // Для английского текста
#define MAX_HISTOGRAM_WIDTH 50      // Максимальная ширина гистограммы

/**
 * Подсчёт частот букв A-Z в тексте (игнорируя регистр)
 * 
 * @param text  Входной текст
 * @param freq  Массив частот [26] (A-Z)
 * @param total Общее количество букв
 */
void count_frequency(const char *text, int freq[ALPHABET_SIZE], int *total) {
    // Инициализация
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freq[i] = 0;
    }
    *total = 0;
    
    // Подсчёт частот
    for (int i = 0; text[i] != '\0'; i++) {
        char c = toupper(text[i]);
        
        if (c >= 'A' && c <= 'Z') {
            freq[c - 'A']++;
            (*total)++;
        }
    }
}

void print_histogram(int freq[26], int total) {
    printf("\n═══ FREQUENCY ANALYSIS ═══\n");
    printf("Total letters: %d\n\n", total);
    
    // Сортировка по убыванию частоты (для красивого вывода)
    // Создаём пары (буква, частота)
    typedef struct {
        char letter;
        int count;
    } LetterFreq;
    
    LetterFreq pairs[26];
    for (int i = 0; i < 26; i++) {
        pairs[i].letter = 'A' + i;
        pairs[i].count = freq[i];
    }
    
    // Простая сортировка пузырьком
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 25 - i; j++) {
            if (pairs[j].count < pairs[j+1].count) {
                LetterFreq temp = pairs[j];
                pairs[j] = pairs[j+1];
                pairs[j+1] = temp;
            }
        }
    }
    
    // Вывод гистограммы
    for (int i = 0; i < 26; i++) {
        if (pairs[i].count > 0) {
            double percent = 100.0 * pairs[i].count / total;
            
            printf("%c: ", pairs[i].letter);
            
            // ASCII bar (масштаб: 1% = 1 блок █)
            int bar_len = (int)(percent);
            for (int j = 0; j < bar_len; j++) {
                printf("█");
            }
            
            printf(" %.1f%% (%d)\n", percent, pairs[i].count);
        }
    }
}

char find_most_frequent(int freq[26]) {
    int max_idx = 0;
    int max_count = freq[0];
    
    for (int i = 1; i < 26; i++) {
        if (freq[i] > max_count) {
            max_count = freq[i];
            max_idx = i;
        }
    }
    
    return 'A' + max_idx;
}

int detect_caesar_shift(int freq[26]) {
    char most_frequent = find_most_frequent(freq);
    
    // Предполагаем, что самая частая буква — это 'E' (4) в plaintext
    int shift = (most_frequent - 'E' + 26) % 26;
    
    return shift;
}

char* read_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("fopen");
        return NULL;
    }
    
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char *content = malloc(size + 1);
    if (!content) {
        fclose(f);
        return NULL;
    }
    
    size_t bytes_read = fread(content, 1, size, f);
    content[bytes_read] = '\0';  // Используем фактически прочитанное
    
    fclose(f);
    return content;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <text_file>\n", argv[0]);
        fprintf(stderr, "Example: %s tests/sample_english.txt\n", argv[0]);
        return 1;
    }
    
    char *text = read_file(argv[1]);
    if (!text) {
        return 1;
    }
    
    int freq[26] = {0};
    int total = 0;
    count_frequency(text, freq, &total);
    
    print_histogram(freq, total);
    
    char most_freq = find_most_frequent(freq);
    printf("\nMost frequent letter: %c (%.1f%%)\n", 
           most_freq, 100.0 * freq[most_freq - 'A'] / total);
    
    int shift = detect_caesar_shift(freq);
    printf("Detected Caesar shift: %d\n", shift);
    
    if (shift > 0) {
        printf("\n💡 Hint: Try decrypting with shift=%d\n", shift);
    }
    
    free(text);
    return 0;
}

