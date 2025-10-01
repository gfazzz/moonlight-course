/**
 * ═══════════════════════════════════════════════════════════════
 * Episode 14: Blockchain & Hashes — Промежуточная задача
 * ═══════════════════════════════════════════════════════════════
 * 
 * ЗАДАЧА: Avalanche Effect Tester для hash-функций
 * 
 * Цель: Проверить "лавинный эффект" hash-функции djb2.
 *       Малое изменение входа → большое изменение выхода (~50% бит).
 * 
 * Что реализовать:
 * 1. hash_djb2() — базовая hash-функция
 * 2. count_bit_differences() — подсчёт различающихся бит
 * 3. test_avalanche_effect() — тест лавинного эффекта
 * 4. visualize_bits() — визуализация бит в binary
 * 
 * Тестовые пары:
 * - "MOONLIGHT" vs "MOONLIGH" (1 символ удалён)
 * - "MOONLIGHT" vs "moonlight" (1 бит изменён в каждом символе)
 * - "BLOCK_0" vs "BLOCK_1" (1 цифра)
 * 
 * Ожидаемый результат:
 * ═══ AVALANCHE EFFECT TEST ═══
 * Input 1: "MOONLIGHT"
 * Input 2: "MOONLIGH"
 * 
 * Hash 1:  0x7C8DF4A2  01111100100011011111010010100010
 * Hash 2:  0x2F91B6E5  00101111100100011011011011100101
 * 
 * Bit differences: 17 / 32 (53.1%)
 * ✅ Good avalanche effect! (expected ~50%)
 * 
 * Компиляция:
 * $ gcc -Wall -Wextra -std=c11 hash_avalanche_tester.c -o hash_tester
 * 
 * Запуск:
 * $ ./hash_tester
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Hash function djb2 (Daniel J. Bernstein, 1991)
 * 
 * @param str Входная строка
 * @return Hash value (32-bit unsigned)
 * 
 * Формула: hash = hash * 33 + c
 * Magic number: 5381
 */
unsigned long hash_djb2(const unsigned char *str) {
    // TODO: Реализовать djb2
    // 1. Инициализировать hash = 5381
    // 2. Для каждого символа c в str:
    //    hash = ((hash << 5) + hash) + c  // hash * 33 + c
    // 3. Вернуть hash
    
    return 0;  // Placeholder
}

/**
 * Подсчитывает количество различающихся бит между двумя числами
 * 
 * @param h1 Первый hash
 * @param h2 Второй hash
 * @return Количество бит, которые отличаются
 * 
 * Алгоритм:
 * 1. XOR двух чисел → биты которые отличаются станут 1
 * 2. Подсчитать количество единиц (popcount)
 */
int count_bit_differences(unsigned long h1, unsigned long h2) {
    // TODO: Реализовать подсчёт различий
    // 1. Вычислить diff = h1 XOR h2
    // 2. Подсчитать количество установленных бит в diff
    //    Hint: while (diff) { count += diff & 1; diff >>= 1; }
    
    return 0;  // Placeholder
}

/**
 * Визуализирует 32-битное число в binary формате
 * 
 * @param value Число для визуализации
 * @param buffer Буфер для результата (минимум 33 байта)
 */
void visualize_bits(unsigned long value, char *buffer) {
    // TODO: Реализовать визуализацию
    // Для каждого бита (от старшего к младшему):
    //   buffer[i] = (value & (1UL << (31 - i))) ? '1' : '0'
    // buffer[32] = '\0'
    
    strcpy(buffer, "00000000000000000000000000000000");  // Placeholder
}

/**
 * Тестирует лавинный эффект для пары строк
 * 
 * @param str1 Первая строка
 * @param str2 Вторая строка (отличается на 1 символ/бит)
 */
void test_avalanche_effect(const char *str1, const char *str2) {
    printf("\n═══ AVALANCHE EFFECT TEST ═══\n");
    printf("Input 1: \"%s\"\n", str1);
    printf("Input 2: \"%s\"\n", str2);
    printf("\n");
    
    // Вычислить хеши
    unsigned long h1 = hash_djb2((unsigned char*)str1);
    unsigned long h2 = hash_djb2((unsigned char*)str2);
    
    // Визуализировать хеши
    char bits1[33], bits2[33];
    visualize_bits(h1, bits1);
    visualize_bits(h2, bits2);
    
    printf("Hash 1:  0x%08lX  %s\n", h1, bits1);
    printf("Hash 2:  0x%08lX  %s\n", h2, bits2);
    printf("\n");
    
    // Подсчитать различия
    int diff_count = count_bit_differences(h1, h2);
    double diff_percent = 100.0 * diff_count / 32;
    
    printf("Bit differences: %d / 32 (%.1f%%)\n", diff_count, diff_percent);
    
    // Оценка
    if (diff_percent >= 40 && diff_percent <= 60) {
        printf("✅ Good avalanche effect! (expected ~50%%)\n");
    } else if (diff_percent >= 30 && diff_percent <= 70) {
        printf("⚠️  Acceptable avalanche effect (%.1f%% is slightly off 50%%)\n", diff_percent);
    } else {
        printf("❌ Poor avalanche effect! Expected ~50%%, got %.1f%%\n", diff_percent);
    }
}

/**
 * Расширенный тест с визуализацией различий
 */
void test_with_visualization(const char *str1, const char *str2) {
    printf("\n═══════════════════════════════════════════════════════════\n");
    printf("DETAILED AVALANCHE ANALYSIS\n");
    printf("═══════════════════════════════════════════════════════════\n\n");
    
    printf("Input 1: \"%s\"\n", str1);
    printf("Input 2: \"%s\"\n", str2);
    
    // Показать ASCII различия
    printf("\nASCII difference:\n");
    for (size_t i = 0; i < strlen(str1) || i < strlen(str2); i++) {
        char c1 = (i < strlen(str1)) ? str1[i] : ' ';
        char c2 = (i < strlen(str2)) ? str2[i] : ' ';
        printf("  [%2zu] '%c' vs '%c'  %s\n", i, c1, c2, 
               (c1 == c2) ? "✓" : "✗ DIFFERENT");
    }
    
    // Хеши
    unsigned long h1 = hash_djb2((unsigned char*)str1);
    unsigned long h2 = hash_djb2((unsigned char*)str2);
    
    printf("\nHash values:\n");
    printf("  Hash 1: 0x%08lX (%lu)\n", h1, h1);
    printf("  Hash 2: 0x%08lX (%lu)\n", h2, h2);
    
    // Binary визуализация
    char bits1[33], bits2[33];
    visualize_bits(h1, bits1);
    visualize_bits(h2, bits2);
    
    printf("\nBinary representation:\n");
    printf("  Hash 1: %s\n", bits1);
    printf("  Hash 2: %s\n", bits2);
    printf("  Diff:   ");
    
    // Показать какие биты отличаются
    for (int i = 0; i < 32; i++) {
        printf("%c", (bits1[i] == bits2[i]) ? '·' : '█');
    }
    printf("\n");
    
    // Статистика
    int diff_count = count_bit_differences(h1, h2);
    double diff_percent = 100.0 * diff_count / 32;
    
    printf("\nStatistics:\n");
    printf("  Bits changed:   %d / 32\n", diff_count);
    printf("  Percentage:     %.1f%%\n", diff_percent);
    printf("  Expected:       ~50%% for good hash\n");
    
    if (diff_percent >= 40 && diff_percent <= 60) {
        printf("  Assessment:     ✅ Excellent avalanche effect\n");
    } else if (diff_percent >= 30 && diff_percent <= 70) {
        printf("  Assessment:     ⚠️  Acceptable\n");
    } else {
        printf("  Assessment:     ❌ Poor (hash function may be weak)\n");
    }
}

int main() {
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║  HASH AVALANCHE EFFECT TESTER                             ║\n");
    printf("║  Testing djb2 hash function (Daniel J. Bernstein, 1991)  ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    // Тест 1: Один символ удалён
    printf("\n[TEST 1] One character removed\n");
    printf("─────────────────────────────────────────────────────────\n");
    test_avalanche_effect("MOONLIGHT", "MOONLIGH");
    
    // Тест 2: Регистр изменён (uppercase → lowercase)
    printf("\n[TEST 2] Case change (uppercase → lowercase)\n");
    printf("─────────────────────────────────────────────────────────\n");
    test_avalanche_effect("MOONLIGHT", "moonlight");
    
    // Тест 3: Одна цифра изменена
    printf("\n[TEST 3] One digit changed\n");
    printf("─────────────────────────────────────────────────────────\n");
    test_avalanche_effect("BLOCK_0", "BLOCK_1");
    
    // Тест 4: Blockchain данные (Episode 14 context!)
    printf("\n[TEST 4] Blockchain data (Episode 14 context)\n");
    printf("─────────────────────────────────────────────────────────\n");
    test_avalanche_effect("Transfer $15.7M to 0x4A7F", "Transfer $25.0M to 0x9B2C");
    
    // Детальный тест
    printf("\n");
    test_with_visualization("GENESIS", "GENESIB");
    
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║  💡 ВЫВОДЫ:                                               ║\n");
    printf("╠═══════════════════════════════════════════════════════════╣\n");
    printf("║  Лавинный эффект (Avalanche Effect) критичен для:        ║\n");
    printf("║                                                           ║\n");
    printf("║  1. BLOCKCHAIN (Episode 14):                              ║\n");
    printf("║     Изменение 1 байта в block data → весь hash другой    ║\n");
    printf("║     → следующий блок обнаруживает подделку!              ║\n");
    printf("║                                                           ║\n");
    printf("║  2. PASSWORD HASHING:                                     ║\n");
    printf("║     \"password123\" vs \"password124\" → хеши не похожи    ║\n");
    printf("║     → злоумышленник не может угадать по similarity       ║\n");
    printf("║                                                           ║\n");
    printf("║  3. DATA INTEGRITY:                                       ║\n");
    printf("║     Малое повреждение файла → hash полностью изменён     ║\n");
    printf("║     → легко обнаружить corruption                        ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n\n");
    
    return 0;
}

