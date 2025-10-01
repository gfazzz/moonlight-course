# Solution for Episode 12: Encrypted Communications

## Complete Implementation - SEASON 3 FINALE

This solution integrates packet capture, TCP stream reconstruction, and XOR decryption to intercept and decrypt encrypted communications.

## Files

- `communication_interceptor.c` — Complete interceptor with:
  - TCP packet capture (from Episode 11)
  - Packet filtering by IP/port
  - TCP session reconstruction by sequence number
  - Frequency analysis for key detection
  - XOR decryption (from Episode 01)
  - Stream saving to files

## Features

✅ Raw packet capture with filtering  
✅ TCP session reconstruction  
✅ Automatic packet ordering by sequence number  
✅ Frequency analysis to find XOR key  
✅ Stream decryption  
✅ Save raw and decrypted streams  

## ⚠️ Requirements

**Must run as root/sudo** for raw socket access!

## Compilation

```bash
gcc -o communication_interceptor communication_interceptor.c -Wall
```

## Usage

```bash
# Capture all TCP traffic
sudo ./communication_interceptor

# Filter by IP
sudo ./communication_interceptor 192.168.1.100

# Filter by IP and port
sudo ./communication_interceptor 192.168.1.100 8080
```

Press `Ctrl+C` to stop capture and process the stream.

## How It Works

1. **Capture Phase**: Intercepts TCP packets with optional filtering
2. **Reconstruction Phase**: Sorts packets by sequence number and concatenates payloads
3. **Analysis Phase**: Uses frequency analysis to detect XOR key
4. **Decryption Phase**: Applies XOR decryption with detected key
5. **Output Phase**: Saves both raw and decrypted streams to `artifacts/`

## Output Files

- `artifacts/stream.dat` - Raw reconstructed TCP stream
- `artifacts/decrypted.txt` - Decrypted message

## Key Concepts Integration

- **Raw Sockets** (Episode 11)
- **XOR Encryption** (Episode 01)
- **Frequency Analysis** (cryptanalysis)
- **TCP Sequence Numbers** (proper stream reconstruction)
- **qsort()** for packet ordering

## 🎉 Season 3 Complete!

You've mastered network fundamentals, socket programming, packet analysis, and encrypted communications!

