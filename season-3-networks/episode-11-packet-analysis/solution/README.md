# Solution for Episode 11: Packet Analysis

## Complete Implementation

This solution implements a raw packet sniffer that captures and analyzes TCP/UDP/ICMP traffic.

## Files

- `packet_sniffer.c` — Complete packet sniffer with:
  - Raw socket creation (AF_INET, SOCK_RAW)
  - IP header parsing
  - TCP/UDP header parsing
  - Hex dump of payloads
  - Packet statistics

## Features

✅ Raw socket packet capture  
✅ IP header parsing (version, protocol, addresses)  
✅ TCP header parsing (ports, flags, sequence numbers)  
✅ UDP header parsing  
✅ Hex dump of packet payloads  
✅ Real-time statistics  
✅ Graceful shutdown with Ctrl+C  

## ⚠️ Requirements

**Must run as root/sudo** because raw sockets require elevated privileges!

## Compilation

```bash
gcc -o packet_sniffer packet_sniffer.c -Wall
```

## Usage

```bash
sudo ./packet_sniffer
```

Press `Ctrl+C` to stop capture and view statistics.

## Key Concepts

- **Raw Sockets** - Capture packets at IP layer
- **struct iphdr** - IP header structure
- **struct tcphdr** - TCP header structure
- **struct udphdr** - UDP header structure
- **ntohs()/ntohl()** - Network to host byte order
- **IPPROTO_TCP/UDP** - Protocol identifiers

## Statistics Tracked

- Total packets captured
- TCP/UDP/ICMP/Other packet counts
- Total bytes captured

