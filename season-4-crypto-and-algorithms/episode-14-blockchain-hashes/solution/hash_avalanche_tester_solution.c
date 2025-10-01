/**
 * ═══════════════════════════════════════════════════════════════
 * Episode 14: Blockchain & Hashes — Промежуточная задача (РЕШЕНИЕ)
 * ═══════════════════════════════════════════════════════════════
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

unsigned long hash_djb2(const unsigned char *str) {
    unsigned long hash = 5381;
    int c;
    
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }
    
    return hash;
}

int count_bit_differences(unsigned long h1, unsigned long h2) {
    unsigned long diff = h1 ^ h2;  // XOR выделяет различающиеся биты
    int count = 0;
    
    // Подсчёт единиц (popcount)
    while (diff) {
        count += diff & 1;
        diff >>= 1;
    }
    
    return count;
}

void visualize_bits(unsigned long value, char *buffer) {
    for (int i = 0; i < 32; i++) {
        // Проверяем бит слева направо
        buffer[i] = (value & (1UL << (31 - i))) ? '1' : '0';
    }
    buffer[32] = '\0';
}

void test_avalanche_effect(const char *str1, const char *str2) {
    printf("\n═══ AVALANCHE EFFECT TEST ═══\n");
    printf("Input 1: \"%s\"\n", str1);
    printf("Input 2: \"%s\"\n", str2);
    printf("\n");
    
    unsigned long h1 = hash_djb2((unsigned char*)str1);
    unsigned long h2 = hash_djb2((unsigned char*)str2);
    
    char bits1[33], bits2[33];
    visualize_bits(h1, bits1);
    visualize_bits(h2, bits2);
    
    printf("Hash 1:  0x%08lX  %s\n", h1, bits1);
    printf("Hash 2:  0x%08lX  %s\n", h2, bits2);
    printf("\n");
    
    int diff_count = count_bit_differences(h1, h2);
    double diff_percent = 100.0 * diff_count / 32;
    
    printf("Bit differences: %d / 32 (%.1f%%)\n", diff_count, diff_percent);
    
    if (diff_percent >= 40 && diff_percent <= 60) {
        printf("✅ Good avalanche effect! (expected ~50%%)\n");
    } else if (diff_percent >= 30 && diff_percent <= 70) {
        printf("⚠️  Acceptable avalanche effect (%.1f%% is slightly off 50%%)\n", diff_percent);
    } else {
        printf("❌ Poor avalanche effect! Expected ~50%%, got %.1f%%\n", diff_percent);
    }
}

void test_with_visualization(const char *str1, const char *str2) {
    printf("\n═══════════════════════════════════════════════════════════\n");
    printf("DETAILED AVALANCHE ANALYSIS\n");
    printf("═══════════════════════════════════════════════════════════\n\n");
    
    printf("Input 1: \"%s\"\n", str1);
    printf("Input 2: \"%s\"\n", str2);
    
    printf("\nASCII difference:\n");
    for (size_t i = 0; i < strlen(str1) || i < strlen(str2); i++) {
        char c1 = (i < strlen(str1)) ? str1[i] : ' ';
        char c2 = (i < strlen(str2)) ? str2[i] : ' ';
        printf("  [%2zu] '%c' vs '%c'  %s\n", i, c1, c2, 
               (c1 == c2) ? "✓" : "✗ DIFFERENT");
    }
    
    unsigned long h1 = hash_djb2((unsigned char*)str1);
    unsigned long h2 = hash_djb2((unsigned char*)str2);
    
    printf("\nHash values:\n");
    printf("  Hash 1: 0x%08lX (%lu)\n", h1, h1);
    printf("  Hash 2: 0x%08lX (%lu)\n", h2, h2);
    
    char bits1[33], bits2[33];
    visualize_bits(h1, bits1);
    visualize_bits(h2, bits2);
    
    printf("\nBinary representation:\n");
    printf("  Hash 1: %s\n", bits1);
    printf("  Hash 2: %s\n", bits2);
    printf("  Diff:   ");
    
    for (int i = 0; i < 32; i++) {
        printf("%c", (bits1[i] == bits2[i]) ? '.' : '#');
    }
    printf("\n");
    
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
    
    printf("\n[TEST 1] One character removed\n");
    printf("─────────────────────────────────────────────────────────\n");
    test_avalanche_effect("MOONLIGHT", "MOONLIGH");
    
    printf("\n[TEST 2] Case change (uppercase → lowercase)\n");
    printf("─────────────────────────────────────────────────────────\n");
    test_avalanche_effect("MOONLIGHT", "moonlight");
    
    printf("\n[TEST 3] One digit changed\n");
    printf("─────────────────────────────────────────────────────────\n");
    test_avalanche_effect("BLOCK_0", "BLOCK_1");
    
    printf("\n[TEST 4] Blockchain data (Episode 14 context)\n");
    printf("─────────────────────────────────────────────────────────\n");
    test_avalanche_effect("Transfer $15.7M to 0x4A7F", "Transfer $25.0M to 0x9B2C");
    
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

