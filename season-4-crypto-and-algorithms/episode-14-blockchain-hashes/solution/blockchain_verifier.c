/*
 * MOONLIGHT OPERATION - Episode 14: Blockchain & Hash Functions - SOLUTION
 * 
 * Complete implementation:
 * 1. djb2 hash function
 * 2. Block hash calculation
 * 3. Blockchain verification
 * 4. Financial transaction analysis
 * 5. Audit trail generation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// ============================================================================
// DATA STRUCTURES
// ============================================================================

typedef struct Block {
    int index;
    time_t timestamp;
    char data[256];
    unsigned long prev_hash;
    unsigned long hash;
    int nonce;
} Block;

typedef struct Transaction {
    char from[64];
    char to[64];
    double amount;
} Transaction;

// ============================================================================
// HASH FUNCTIONS
// ============================================================================

/**
 * djb2 hash function
 * Classic string hash by Dan Bernstein
 */
unsigned long hash_djb2(const unsigned char *str) {
    unsigned long hash = 5381;
    int c;
    
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }
    
    return hash;
}

/**
 * Calculate hash of a block
 */
unsigned long calculate_block_hash(Block *block) {
    char buffer[512];
    
    // Concatenate all fields
    snprintf(buffer, sizeof(buffer), "%d%ld%s%lu%d",
             block->index,
             (long)block->timestamp,
             block->data,
             block->prev_hash,
             block->nonce);
    
    return hash_djb2((unsigned char*)buffer);
}

// ============================================================================
// BLOCKCHAIN VERIFICATION
// ============================================================================

/**
 * Verify blockchain integrity
 */
bool verify_blockchain(Block *chain, int length) {
    printf("Verifying blockchain...\n");
    
    for (int i = 0; i < length; i++) {
        // Check 1: Hash integrity
        unsigned long calculated_hash = calculate_block_hash(&chain[i]);
        if (calculated_hash != chain[i].hash) {
            printf("❌ Block %d: hash mismatch!\n", i);
            printf("   Expected: %lu\n", chain[i].hash);
            printf("   Got:      %lu\n", calculated_hash);
            return false;
        }
        
        // Check 2: Chain linkage
        if (i > 0) {
            if (chain[i].prev_hash != chain[i-1].hash) {
                printf("❌ Block %d: broken chain link!\n", i);
                printf("   prev_hash: %lu\n", chain[i].prev_hash);
                printf("   Expected:  %lu\n", chain[i-1].hash);
                return false;
            }
        } else {
            // Check 3: Genesis block
            if (chain[0].prev_hash != 0) {
                printf("❌ Genesis block has invalid prev_hash: %lu\n", chain[0].prev_hash);
                return false;
            }
        }
        
        // Check 4: Index sequence
        if (chain[i].index != i) {
            printf("❌ Block %d: wrong index (expected %d, got %d)\n", 
                   i, i, chain[i].index);
            return false;
        }
        
        printf("✓ Block %d: ", i);
        if (i == 0) {
            printf("Genesis ");
        }
        printf("valid (hash: %lu)\n", chain[i].hash);
    }
    
    return true;
}

// ============================================================================
// TRANSACTION ANALYSIS
// ============================================================================

/**
 * Parse transaction from block data
 */
Transaction parse_transaction(const char *data) {
    Transaction tx = {0};
    
    // Format: "TRANSFER:FROM:TO:AMOUNT"
    sscanf(data, "TRANSFER:%63[^:]:%63[^:]:%lf", 
           tx.from, tx.to, &tx.amount);
    
    return tx;
}

/**
 * Calculate balance for an address
 */
double calculate_balance(Block *chain, int length, const char *address) {
    double balance = 0.0;
    
    for (int i = 0; i < length; i++) {
        // Skip non-transaction blocks
        if (strncmp(chain[i].data, "TRANSFER:", 9) != 0) {
            continue;
        }
        
        Transaction tx = parse_transaction(chain[i].data);
        
        // Outgoing transaction
        if (strcmp(tx.from, address) == 0) {
            balance -= tx.amount;
        }
        
        // Incoming transaction
        if (strcmp(tx.to, address) == 0) {
            balance += tx.amount;
        }
    }
    
    return balance;
}

/**
 * Find all corrupted blocks
 */
int find_corrupted_blocks(Block *chain, int length) {
    int corrupted_count = 0;
    
    printf("\nScanning for corruption...\n");
    
    for (int i = 0; i < length; i++) {
        unsigned long calculated = calculate_block_hash(&chain[i]);
        if (chain[i].hash != calculated) {
            printf("⚠️  Block %d is corrupted!\n", i);
            corrupted_count++;
        }
    }
    
    if (corrupted_count == 0) {
        printf("✓ No corruption detected\n");
    }
    
    return corrupted_count;
}

/**
 * Generate audit trail report
 */
void generate_audit_report(Block *chain, int length) {
    FILE *f = fopen("artifacts/audit_trail.txt", "w");
    if (!f) {
        perror("Error creating audit report");
        return;
    }
    
    time_t now = time(NULL);
    
    fprintf(f, "MOONLIGHT BLOCKCHAIN AUDIT TRAIL\n");
    fprintf(f, "═══════════════════════════════════════════════\n\n");
    fprintf(f, "Total blocks: %d\n", length);
    fprintf(f, "Report generated: %s", ctime(&now));
    fprintf(f, "\n");
    
    fprintf(f, "TRANSACTIONS:\n");
    fprintf(f, "───────────────────────────────────────────────\n");
    
    int tx_count = 0;
    for (int i = 0; i < length; i++) {
        if (strncmp(chain[i].data, "TRANSFER:", 9) == 0) {
            Transaction tx = parse_transaction(chain[i].data);
            
            fprintf(f, "[Block %d] %s -> %s: $%.2f\n", 
                    i, tx.from, tx.to, tx.amount);
            
            tx_count++;
        }
    }
    
    fprintf(f, "\nTotal transactions: %d\n", tx_count);
    
    // Calculate balances for all addresses
    fprintf(f, "\n");
    fprintf(f, "BALANCES:\n");
    fprintf(f, "───────────────────────────────────────────────\n");
    
    // Find unique addresses
    char addresses[100][64] = {0};
    int addr_count = 0;
    
    for (int i = 0; i < length; i++) {
        if (strncmp(chain[i].data, "TRANSFER:", 9) == 0) {
            Transaction tx = parse_transaction(chain[i].data);
            
            // Add 'from' address
            int found = 0;
            for (int j = 0; j < addr_count; j++) {
                if (strcmp(addresses[j], tx.from) == 0) {
                    found = 1;
                    break;
                }
            }
            if (!found && addr_count < 100) {
                strcpy(addresses[addr_count++], tx.from);
            }
            
            // Add 'to' address
            found = 0;
            for (int j = 0; j < addr_count; j++) {
                if (strcmp(addresses[j], tx.to) == 0) {
                    found = 1;
                    break;
                }
            }
            if (!found && addr_count < 100) {
                strcpy(addresses[addr_count++], tx.to);
            }
        }
    }
    
    // Calculate and print balances
    for (int i = 0; i < addr_count; i++) {
        double balance = calculate_balance(chain, length, addresses[i]);
        fprintf(f, "%s: $%.2f\n", addresses[i], balance);
    }
    
    fclose(f);
    printf("✓ Audit trail saved to artifacts/audit_trail.txt\n");
}

// ============================================================================
// FILE I/O
// ============================================================================

/**
 * Load blockchain from file
 */
Block* load_blockchain(const char *filename, int *length) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("Error opening blockchain file");
        return NULL;
    }
    
    // Read number of blocks
    fread(length, sizeof(int), 1, f);
    
    // Allocate memory
    Block *chain = malloc(sizeof(Block) * (*length));
    if (!chain) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(f);
        return NULL;
    }
    
    // Read blocks
    fread(chain, sizeof(Block), *length, f);
    
    fclose(f);
    
    printf("Blocks loaded: %d\n\n", *length);
    
    return chain;
}

// ============================================================================
// MAIN
// ============================================================================

void print_usage(const char *program_name) {
    printf("MOONLIGHT Blockchain Verifier - Episode 14\n\n");
    printf("Usage: %s <blockchain_file>\n\n", program_name);
    printf("Example:\n");
    printf("  %s moonlight_chain.dat\n", program_name);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    const char *filename = argv[1];
    
    printf("═══════════════════════════════════════════════\n");
    printf("  MOONLIGHT Blockchain Verifier\n");
    printf("═══════════════════════════════════════════════\n\n");
    
    // Create artifacts directory
    system("mkdir -p artifacts");
    
    // Load blockchain
    printf("Loading blockchain: %s\n", filename);
    int length;
    Block *chain = load_blockchain(filename, &length);
    
    if (!chain) {
        return 1;
    }
    
    // Verify blockchain
    printf("\n");
    bool is_valid = verify_blockchain(chain, length);
    
    printf("\n═══════════════════════════════════════════════\n");
    if (is_valid) {
        printf("VERIFICATION RESULT: ✓ BLOCKCHAIN VALID\n");
    } else {
        printf("VERIFICATION RESULT: ❌ BLOCKCHAIN CORRUPTED\n");
    }
    printf("═══════════════════════════════════════════════\n\n");
    
    // Financial analysis
    printf("Financial Analysis:\n");
    printf("───────────────────────────────────────────────\n");
    
    // Count transactions
    int tx_count = 0;
    for (int i = 0; i < length; i++) {
        if (strncmp(chain[i].data, "TRANSFER:", 9) == 0) {
            tx_count++;
        }
    }
    printf("Total transactions: %d\n", tx_count);
    
    // Calculate MOONLIGHT balance
    double moonlight_balance = calculate_balance(chain, length, "MOONLIGHT");
    printf("MOONLIGHT balance: $%.2f\n", moonlight_balance);
    
    // Generate reports
    printf("\n");
    generate_audit_report(chain, length);
    
    // Cleanup
    free(chain);
    
    return is_valid ? 0 : 1;
}

