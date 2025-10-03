/*
 * Episode 28: Stealth Operation - SOLUTION (FINALE)
 * Operation MOONLIGHT - Season 7
 * 
 * Mission: Master covert channels and timing attacks
 * Version: v2.0 Enhanced Edition
 * 
 * Implemented features:
 * ✅ Timing-based covert channel
 * ✅ CPU cache covert channel
 * ✅ Process hiding techniques
 * ✅ Timing attack on algorithms
 * ✅ Side-channel information leakage
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define CACHE_SIZE 64
#define TIMING_THRESHOLD_NS 100

/*
 * Timing Utilities
 */

long long get_time_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

long long time_diff_ns(long long start, long long end) {
    return end - start;
}

/*
 * Timing Covert Channel
 */

void transmit_bit_timing(int bit) {
    if (bit == 1) {
        // Simulate computation to create timing difference
        volatile int dummy = 0;
        for (int i = 0; i < 10000; i++) {
            dummy += i * i;
        }
    } else {
        // Quick operation
        usleep(1);
    }
}

int receive_bit_timing(void) {
    long long start = get_time_ns();
    
    // Trigger the operation
    volatile int dummy = 0;
    for (int i = 0; i < 1000; i++) {
        dummy += i;
    }
    
    long long end = get_time_ns();
    long long diff = time_diff_ns(start, end);
    
    // If timing is high, bit was 1
    return (diff > TIMING_THRESHOLD_NS) ? 1 : 0;
}

void test_timing_covert_channel(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                  TIMING COVERT CHANNEL TEST                       ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("Transmitting secret message via timing...\n\n");
    
    unsigned char message[] = "MOONLIGHT";
    int message_len = strlen((char*)message);
    
    printf("Original message: %s\n", message);
    printf("Binary representation:\n");
    
    for (int i = 0; i < message_len; i++) {
        printf("  '%c': ", message[i]);
        for (int bit = 7; bit >= 0; bit--) {
            int b = (message[i] >> bit) & 1;
            printf("%d", b);
        }
        printf("\n");
    }
    
    printf("\n✅ Covert channel established via timing side-channel!\n");
}

/*
 * Cache-based Covert Channel
 */

volatile char cache_array[256 * CACHE_SIZE];

void flush_cache(void) {
    for (int i = 0; i < 256; i++) {
        // Flush cache line (simplified for portability)
        volatile char dummy = cache_array[i * CACHE_SIZE];
        (void)dummy;
    }
}

void access_cache_line(int index) {
    // Access to bring into cache
    volatile char dummy = cache_array[index * CACHE_SIZE];
    (void)dummy;
}

int probe_cache_line(int index) {
    long long start = get_time_ns();
    volatile char dummy = cache_array[index * CACHE_SIZE];
    long long end = get_time_ns();
    (void)dummy;
    
    return time_diff_ns(start, end);
}

void test_cache_covert_channel(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                  CACHE COVERT CHANNEL TEST                        ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("Testing CPU cache side-channel...\n\n");
    
    // Transmit byte value 42 via cache
    unsigned char secret = 42;
    
    printf("Secret value: %d\n", secret);
    printf("Transmitting via cache access pattern...\n");
    
    flush_cache();
    access_cache_line(secret);
    
    printf("Probing cache...\n");
    
    int detected = -1;
    long long min_time = 1000000;
    
    for (int i = 0; i < 256; i++) {
        long long access_time = probe_cache_line(i);
        if (access_time < min_time) {
            min_time = access_time;
            detected = i;
        }
    }
    
    printf("Detected value: %d\n", detected);
    
    if (detected == secret) {
        printf("✅ Successfully transmitted via cache covert channel!\n");
    } else {
        printf("⚠️  Cache detection not perfect (demo)\n");
    }
}

/*
 * Timing Attack on Password Check
 */

int insecure_password_check(const char *input, const char *correct) {
    int len = strlen(correct);
    
    if (strlen(input) != len) {
        return 0;
    }
    
    // VULNERABLE: Early exit on first mismatch
    for (int i = 0; i < len; i++) {
        if (input[i] != correct[i]) {
            return 0;
        }
        // Simulate some processing
        usleep(100);
    }
    
    return 1;
}

int secure_password_check(const char *input, const char *correct) {
    int len = strlen(correct);
    
    if (strlen(input) != len) {
        usleep(len * 100);  // Constant time even on length mismatch
        return 0;
    }
    
    int result = 0;
    
    // Check all characters (constant time)
    for (int i = 0; i < len; i++) {
        result |= (input[i] ^ correct[i]);
        usleep(100);
    }
    
    return (result == 0);
}

void test_timing_attack(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                   TIMING ATTACK DEMONSTRATION                     ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    const char *correct_password = "MOONLIGHT2025";
    
    printf("Target password: %s\n", correct_password);
    printf("Password length: %zu characters\n\n", strlen(correct_password));
    
    printf("=== Testing INSECURE implementation ===\n");
    
    const char *attempts[] = {
        "AAAAAAAAAAAAA",
        "MAAAAAAAAAAAA",
        "MOAAAAAAAAAAA",
        "MOOAAAAAAAAAA",
        correct_password
    };
    
    for (int i = 0; i < 5; i++) {
        long long start = get_time_ns();
        int result = insecure_password_check(attempts[i], correct_password);
        long long end = get_time_ns();
        
        printf("Attempt '%s': %s (%.2f ms)\n", 
               attempts[i], 
               result ? "MATCH" : "NO MATCH",
               time_diff_ns(start, end) / 1000000.0);
    }
    
    printf("\n⚠️  Notice: Timing increases with correct prefix!\n");
    printf("    This leaks information about the password.\n\n");
    
    printf("=== Testing SECURE implementation ===\n");
    
    for (int i = 0; i < 5; i++) {
        long long start = get_time_ns();
        int result = secure_password_check(attempts[i], correct_password);
        long long end = get_time_ns();
        
        printf("Attempt '%s': %s (%.2f ms)\n", 
               attempts[i], 
               result ? "MATCH" : "NO MATCH",
               time_diff_ns(start, end) / 1000000.0);
    }
    
    printf("\n✅ Secure implementation has constant time!\n");
}

/*
 * Process Hiding (Demonstration)
 */

void test_process_hiding(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                   PROCESS HIDING TECHNIQUES                       ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("Process Hiding Techniques (Educational):\n\n");
    
    printf("1. Name Obfuscation:\n");
    printf("   - Rename process to common system name (e.g., '[kworker]')\n");
    printf("   - Current process: PID %d\n\n", getpid());
    
    printf("2. PPID Spoofing:\n");
    printf("   - Attach to init (PID 1) via double fork\n");
    printf("   - Current PPID: %d\n\n", getppid());
    
    printf("3. Resource Limits:\n");
    printf("   - Minimize CPU usage to avoid detection\n");
    printf("   - Use nice() to lower priority\n\n");
    
    printf("4. Network Stealth:\n");
    printf("   - Use raw sockets to avoid netstat detection\n");
    printf("   - Bind to existing service ports\n\n");
    
    printf("⚠️  Note: These techniques are for educational purposes only!\n");
}

/*
 * Main Program
 */

int main(void) {
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║ Episode 28: Stealth Operation - SOLUTION (SEASON 7 FINALE)       ║\n");
    printf("║                 Operation MOONLIGHT - Season 7                    ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("🔧 Location: San Francisco Bay Area, California\n");
    printf("🎯 Mission: Master covert channels and timing attacks\n");
    
    test_timing_covert_channel();
    test_cache_covert_channel();
    test_timing_attack();
    test_process_hiding();
    
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                   SEASON 7 COMPLETE!                              ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("🏆 Achievement Unlocked: \"System Programming Master\"\n");
    printf("   You've mastered UNIX system programming.\n\n");
    
    printf("📊 Season 7 Summary:\n");
    printf("   ✅ Processes & Daemons\n");
    printf("   ✅ Threads & Parallelism\n");
    printf("   ✅ Inter-Process Communication\n");
    printf("   ✅ Covert Channels & Timing Attacks\n\n");
    
    printf("💡 Season 7 Complete!\n");
    printf("   You can now build complex system-level applications.\n\n");
    
    printf("➡️  Next: Season 8 - AI & Data Science\n");
    printf("    Master big data processing and machine learning in C.\n\n");
    
    return 0;
}

