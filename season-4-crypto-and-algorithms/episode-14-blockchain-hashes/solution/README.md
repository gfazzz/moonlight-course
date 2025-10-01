# Solution for Episode 14: Blockchain & Hashes

## Complete Implementation

This solution implements a complete blockchain verifier with hash functions, integrity checking, and financial transaction analysis.

## Files

- `blockchain_verifier.c` — Complete blockchain verifier with:
  - djb2 hash function implementation
  - Block hash calculation
  - Blockchain integrity verification
  - Transaction parsing and analysis
  - Balance calculation
  - Audit trail generation

## Features

✅ djb2 hash function (Dan Bernstein's algorithm)  
✅ Block hash calculation from all fields  
✅ Complete blockchain verification:
  - Hash integrity check
  - Chain linkage validation
  - Genesis block verification
  - Index sequence validation  
✅ Transaction parsing (TRANSFER format)  
✅ Balance calculation for addresses  
✅ Audit trail report generation  

## Compilation

```bash
gcc -o blockchain_verifier blockchain_verifier.c -Wall
```

## Usage

```bash
./blockchain_verifier <blockchain_file.dat>

# Example:
./blockchain_verifier moonlight_chain.dat
```

## Output

The program will:
1. Load blockchain from binary file
2. Verify each block's integrity
3. Check chain linkage
4. Calculate financial balances
5. Generate audit trail in `artifacts/audit_trail.txt`

## Blockchain Structure

```c
typedef struct Block {
    int index;              // Block number
    time_t timestamp;       // Creation time
    char data[256];         // Transaction data
    unsigned long prev_hash; // Previous block hash
    unsigned long hash;      // This block's hash
    int nonce;              // Proof-of-work nonce
} Block;
```

## Hash Function (djb2)

```c
hash = 5381
for each character c:
    hash = (hash * 33) + c
```

## Transaction Format

```
TRANSFER:FROM:TO:AMOUNT
Example: TRANSFER:MOONLIGHT:V:50000.00
```

## Verification Checks

1. ✓ Hash integrity: Recalculated hash matches stored hash
2. ✓ Chain linkage: prev_hash matches previous block's hash
3. ✓ Genesis block: First block has prev_hash = 0
4. ✓ Index sequence: Blocks numbered sequentially

## Key Concepts

- **Hash Functions** - Deterministic, collision-resistant
- **Blockchain** - Linked chain of blocks with cryptographic hashes
- **Genesis Block** - First block in chain (prev_hash = 0)
- **Integrity Verification** - Detecting tampering through hash validation
- **Financial Ledger** - Transaction tracking and balance calculation

