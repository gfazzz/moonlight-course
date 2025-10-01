# Solution for Episode 10: Socket Programming

## Complete Implementation

This solution provides a fully functional TCP client that connects to a server and exchanges messages.

## Files

- `socket_client.c` — Complete TCP client with:
  - Socket creation and connection
  - Send/receive functionality
  - XOR encryption support
  - Interactive command loop
  - Error handling

## Features

✅ TCP socket connection  
✅ Command-line interface  
✅ XOR encryption from Episode 01  
✅ Interactive loop for multiple commands  
✅ Proper error handling and cleanup  

## Compilation

```bash
gcc -o socket_client socket_client.c -Wall
```

## Usage

```bash
./socket_client <IP> <PORT>

# Example:
./socket_client 127.0.0.1 8080
```

## Commands

- `HELLO` - Get server greeting
- `GET_KEY` - Request encryption key
- `SEND <message>` - Send encrypted message
- `QUIT` - Close connection
- `help` - Show help menu

## Key Concepts

- **socket()** - Creates endpoint for communication
- **connect()** - Initiates connection to server
- **send()/recv()** - Data transmission
- **inet_pton()** - Converts IP addresses
- **close()** - Closes socket connection

