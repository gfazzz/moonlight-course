# Mission Brief: Strange Message

## Objective
Decrypt Victor's message using XOR cipher.

## Given
- **Encrypted data**: Array of bytes (hex format)
- **XOR Key**: `0x42`
- **Method**: Simple XOR encryption

## Tasks

### 1. warmup.c
- [ ] Read a byte from user (0-255)
- [ ] Apply XOR with key 0x42
- [ ] Output result in hex format

### 2. decoder.c (MAIN)
- [ ] Store encrypted bytes in array
- [ ] Calculate array length
- [ ] Create decrypted array (+1 for '\0')
- [ ] Loop through and XOR each byte
- [ ] Add null terminator
- [ ] Print decrypted message

### 3. validator.c (BONUS)
- [ ] Count letters, digits, spaces, others
- [ ] Print statistics
- [ ] Calculate checksum

## Success Criteria
- ✅ Code compiles without errors
- ✅ Decrypted message is readable text
- ✅ No strange characters (except newlines)
- ✅ Automated tests pass

## Compile & Run
```bash
# Compile
gcc -Wall -Wextra -std=c11 -o decoder artifacts/decoder.c

# Run
./decoder

# Test
cd tests && ./test.sh
```

## Time Estimate
- Warmup: 15-20 min
- Decoder: 30-40 min
- Validator: 20-30 min
- Total: ~1.5 hours

## Hints
<details>
<summary>Hint 1: Array length</summary>

```c
int length = sizeof(encrypted) / sizeof(encrypted[0]);
```
</details>

<details>
<summary>Hint 2: XOR in loop</summary>

```c
for (int i = 0; i < length; i++) {
    decrypted[i] = encrypted[i] ^ KEY;
}
```
</details>

<details>
<summary>Hint 3: Null terminator</summary>

Strings in C MUST end with '\0':
```c
decrypted[length] = '\0';
```
</details>

## What You'll Learn
- ✅ Variables and types
- ✅ Arrays
- ✅ Loops
- ✅ XOR operator
- ✅ printf/scanf
- ✅ Compilation

---

**Good luck, agent!** 🎯
