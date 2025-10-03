/*
 * Episode 27: Inter-Process Communication
 * Operation MOONLIGHT - Season 7
 * 
 * Mission: Master IPC mechanisms
 * 
 * Your tasks:
 * 1. Use anonymous pipes
 * 2. Create named pipes (FIFO)
 * 3. Use POSIX shared memory
 * 4. Implement signal-based IPC
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>

#define BUFFER_SIZE 1024
#define SHM_NAME "/moonlight_shm"
#define FIFO_NAME "/tmp/moonlight_fifo"

/*
 * TODO: Implement these functions
 */

// Test anonymous pipe
void test_pipe(void) {
    // TODO:
    // 1. Create pipe with pipe()
    // 2. Fork process
    // 3. Parent writes to pipe
    // 4. Child reads from pipe
}

// Test named pipe (FIFO)
void test_fifo(void) {
    // TODO:
    // 1. Create FIFO with mkfifo()
    // 2. Fork process
    // 3. Parent opens for writing
    // 4. Child opens for reading
    // 5. Transfer data
    // 6. Cleanup FIFO
}

// Shared data structure
typedef struct {
    int counter;
    char message[256];
    int ready;
} SharedData;

// Test shared memory
void test_shared_memory(void) {
    // TODO:
    // 1. Create shared memory with shm_open()
    // 2. Set size with ftruncate()
    // 3. Map memory with mmap()
    // 4. Fork process
    // 5. Parent writes to shared memory
    // 6. Child reads from shared memory
    // 7. Cleanup with shm_unlink()
}

// Signal handler for IPC
void signal_handler(int signum) {
    // TODO: Handle SIGUSR1
}

// Test signal-based IPC
void test_signal_ipc(void) {
    // TODO:
    // 1. Install signal handler
    // 2. Fork process
    // 3. Parent sends SIGUSR1 to child
    // 4. Child receives and handles signal
}

int main(void) {
    printf("=== Episode 27: Inter-Process Communication ===\n\n");
    
    // TODO: Test different IPC mechanisms
    // test_pipe();
    // test_fifo();
    // test_shared_memory();
    // test_signal_ipc();
    
    printf("\n=== IPC Tests Complete ===\n");
    
    return 0;
}

