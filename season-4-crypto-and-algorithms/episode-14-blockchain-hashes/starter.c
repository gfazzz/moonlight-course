/*
 * MOONLIGHT OPERATION - Episode 14: Blockchain & Hash Functions
 * 
 * Mission: Verify blockchain integrity and analyze financial transactions
 * 
 * Tasks:
 * 1. Implement hash function (djb2)
 * 2. Calculate block hashes
 * 3. Verify blockchain integrity
 * 4. Analyze financial transactions
 * 
 * Compilation: make
 * Usage: ./blockchain_verifier <blockchain_file>
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
    int index;              // Block number (0, 1, 2, ...)
    time_t timestamp;       // Creation time
    char data[256];         // Transaction data
    unsigned long prev_hash; // Hash of previous block
    unsigned long hash;      // Hash of this block
    int nonce;              // Proof-of-work nonce
} Block;

typedef struct Transaction {
    char from[64];
    char to[64];
    double amount;
} Transaction;

// ============================================================================
// TODO: IMPLEMENT THESE FUNCTIONS
// ============================================================================

/**
 * djb2 hash function
 * 
 * This is a simple but effective hash function.
 * Algorithm: hash = hash * 33 + c
 * 
 * @param str - input string
 * @return hash value
 */
unsigned long hash_djb2(const unsigned char *str) {
    // TODO: Implement djb2 hash
    // Initial value: 5381
    // For each character: hash = ((hash << 5) + hash) + c
    
    unsigned long hash = 5381;
    
    // YOUR CODE HERE
    
    return hash;
}

/**
 * Calculate hash of a block
 * 
 * Hash is calculated from all block fields:
 * hash(index + timestamp + data + prev_hash + nonce)
 * 
 * @param block - block to hash
 * @return calculated hash value
 */
unsigned long calculate_block_hash(Block *block) {
    char buffer[512];
    
    // TODO: Create string from all block fields
    // Format: "%d%ld%s%lu%d"
    // YOUR CODE HERE
    
    // TODO: Calculate hash
    // YOUR CODE HERE
    
    return 0;  // Replace with actual hash
}

/**
 * Verify blockchain integrity
 * 
 * Checks:
 * 1. Each block's hash is correct
 * 2. prev_hash links are valid
 * 3. Genesis block has prev_hash = 0
 * 4. Indices are sequential
 * 
 * @param chain  - array of blocks
 * @param length - number of blocks
 * @return true if blockchain is valid
 */
bool verify_blockchain(Block *chain, int length) {
    printf("Verifying blockchain...\n");
    
    for (int i = 0; i < length; i++) {
        // TODO: Check 1 - Hash integrity
        unsigned long calculated_hash = calculate_block_hash(&chain[i]);
        if (calculated_hash != chain[i].hash) {
            printf("❌ Block %d: hash mismatch!\n", i);
            printf("   Expected: %lu\n", chain[i].hash);
            printf("   Got:      %lu\n", calculated_hash);
            return false;
        }
        
        // TODO: Check 2 - Chain linkage
        if (i > 0) {
            // YOUR CODE HERE
            // Check: chain[i].prev_hash == chain[i-1].hash
        } else {
            // TODO: Check 3 - Genesis block
            // YOUR CODE HERE
            // Check: chain[0].prev_hash == 0
        }
        
        // TODO: Check 4 - Index sequence
        // YOUR CODE HERE
        
        printf("✓ Block %d: ", i);
        if (i == 0) {
            printf("Genesis ");
        }
        printf("valid (hash: %lu)\n", chain[i].hash);
    }
    
    return true;
}

/**
 * Parse transaction from block data
 * 
 * Format: "TRANSFER:FROM:TO:AMOUNT"
 * Example: "TRANSFER:MOONLIGHT:V:50000.00"
 * 
 * @param data - transaction string
 * @return parsed transaction
 */
Transaction parse_transaction(const char *data) {
    Transaction tx = {0};
    
    // TODO: Parse transaction string
    // Use sscanf with format: "TRANSFER:%[^:]:%[^:]:%lf"
    // YOUR CODE HERE
    
    return tx;
}

/**
 * Calculate balance for an address
 * 
 * Balance = sum of incoming - sum of outgoing
 * 
 * @param chain   - blockchain array
 * @param length  - number of blocks
 * @param address - address to calculate balance for
 * @return balance (positive or negative)
 */
double calculate_balance(Block *chain, int length, const char *address) {
    double balance = 0.0;
    
    // TODO: Iterate through all blocks
    for (int i = 0; i < length; i++) {
        // Skip non-transaction blocks
        if (strncmp(chain[i].data, "TRANSFER:", 9) != 0) {
            continue;
        }
        
        // TODO: Parse transaction
        // YOUR CODE HERE
        
        // TODO: Update balance
        // If tx.from == address: balance -= amount (outgoing)
        // If tx.to == address:   balance += amount (incoming)
        // YOUR CODE HERE
    }
    
    return balance;
}

/**
 * Find all corrupted blocks
 * 
 * @param chain  - blockchain array
 * @param length - number of blocks
 * @return number of corrupted blocks found
 */
int find_corrupted_blocks(Block *chain, int length) {
    int corrupted_count = 0;
    
    printf("\nScanning for corruption...\n");
    
    // TODO: Check each block for corruption
    // YOUR CODE HERE
    
    return corrupted_count;
}

/**
 * Generate audit trail report
 * 
 * Saves all transactions to artifacts/audit_trail.txt
 * 
 * @param chain  - blockchain array
 * @param length - number of blocks
 */
void generate_audit_report(Block *chain, int length) {
    FILE *f = fopen("artifacts/audit_trail.txt", "w");
    if (!f) {
        perror("Error creating audit report");
        return;
    }
    
    fprintf(f, "MOONLIGHT BLOCKCHAIN AUDIT TRAIL\n");
    fprintf(f, "═══════════════════════════════════════════════\n\n");
    fprintf(f, "Total blocks: %d\n", length);
    fprintf(f, "Report generated: %s\n", ctime(&(time_t){time(NULL)}));
    fprintf(f, "\n");
    
    // TODO: Write all transactions
    fprintf(f, "TRANSACTIONS:\n");
    fprintf(f, "───────────────────────────────────────────────\n");
    
    int tx_count = 0;
    for (int i = 0; i < length; i++) {
        if (strncmp(chain[i].data, "TRANSFER:", 9) == 0) {
            // TODO: Parse and write transaction
            // YOUR CODE HERE
            tx_count++;
        }
    }
    
    fprintf(f, "\nTotal transactions: %d\n", tx_count);
    
    fclose(f);
    printf("✓ Audit trail saved to artifacts/audit_trail.txt\n");
}

// ============================================================================
// FILE I/O
// ============================================================================

/**
 * Load blockchain from file
 * 
 * @param filename - blockchain data file
 * @param length   - output: number of blocks loaded
 * @return array of blocks (caller must free)
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
    
    // TODO: Count transactions
    int tx_count = 0;
    for (int i = 0; i < length; i++) {
        if (strncmp(chain[i].data, "TRANSFER:", 9) == 0) {
            tx_count++;
        }
    }
    printf("Total transactions: %d\n", tx_count);
    
    // TODO: Calculate MOONLIGHT balance
    double moonlight_balance = calculate_balance(chain, length, "MOONLIGHT");
    printf("MOONLIGHT balance: $%.2f\n", moonlight_balance);
    
    // Generate reports
    printf("\n");
    generate_audit_report(chain, length);
    
    // Cleanup
    free(chain);
    
    return is_valid ? 0 : 1;
}
