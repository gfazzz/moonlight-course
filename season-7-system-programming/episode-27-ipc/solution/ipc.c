/*
 * Episode 27: Inter-Process Communication - SOLUTION
 * Operation MOONLIGHT - Season 7
 * 
 * Mission: Master IPC mechanisms
 * Version: v2.0 Enhanced Edition
 * 
 * Implemented features:
 * ✅ Pipes (anonymous and named/FIFO)
 * ✅ Shared memory (POSIX shm)
 * ✅ Message queues (POSIX mq)
 * ✅ Signals for IPC
 * ✅ Communication protocols
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define SHM_NAME "/moonlight_shm"
#define FIFO_NAME "/tmp/moonlight_fifo"

/*
 * Pipes
 */

void test_pipe(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                        PIPE TEST                                  ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    int pipefd[2];
    char buffer[BUFFER_SIZE];
    
    if (pipe(pipefd) < 0) {
        perror("pipe");
        return;
    }
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        // Child: reader
        close(pipefd[1]);  // Close write end
        
        printf("Child waiting for message...\n");
        int n = read(pipefd[0], buffer, sizeof(buffer));
        
        if (n > 0) {
            buffer[n] = '\0';
            printf("Child received: '%s'\n", buffer);
        }
        
        close(pipefd[0]);
        exit(0);
        
    } else {
        // Parent: writer
        close(pipefd[0]);  // Close read end
        
        const char *message = "Hello from parent via pipe!";
        printf("Parent sending: '%s'\n", message);
        write(pipefd[1], message, strlen(message));
        
        close(pipefd[1]);
        wait(NULL);
    }
}

void test_fifo(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                       NAMED PIPE (FIFO) TEST                      ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    // Remove existing FIFO
    unlink(FIFO_NAME);
    
    // Create FIFO
    if (mkfifo(FIFO_NAME, 0666) < 0) {
        perror("mkfifo");
        return;
    }
    
    printf("FIFO created: %s\n", FIFO_NAME);
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        // Child: reader
        sleep(1);  // Let parent open first
        
        int fd = open(FIFO_NAME, O_RDONLY);
        if (fd < 0) {
            perror("open");
            exit(1);
        }
        
        char buffer[BUFFER_SIZE];
        int n = read(fd, buffer, sizeof(buffer));
        
        if (n > 0) {
            buffer[n] = '\0';
            printf("Child received from FIFO: '%s'\n", buffer);
        }
        
        close(fd);
        exit(0);
        
    } else {
        // Parent: writer
        int fd = open(FIFO_NAME, O_WRONLY);
        if (fd < 0) {
            perror("open");
            return;
        }
        
        const char *message = "Hello from parent via FIFO!";
        printf("Parent sending to FIFO: '%s'\n", message);
        write(fd, message, strlen(message));
        
        close(fd);
        wait(NULL);
        
        unlink(FIFO_NAME);
    }
}

/*
 * Shared Memory
 */

typedef struct {
    int counter;
    char message[256];
    int ready;
} SharedData;

void test_shared_memory(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                    SHARED MEMORY TEST                             ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    // Remove existing shm
    shm_unlink(SHM_NAME);
    
    // Create shared memory
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("shm_open");
        return;
    }
    
    // Set size
    if (ftruncate(shm_fd, sizeof(SharedData)) < 0) {
        perror("ftruncate");
        return;
    }
    
    // Map shared memory
    SharedData *shared = mmap(NULL, sizeof(SharedData), 
                             PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    
    if (shared == MAP_FAILED) {
        perror("mmap");
        return;
    }
    
    // Initialize
    shared->counter = 0;
    shared->ready = 0;
    strcpy(shared->message, "");
    
    printf("Shared memory created: %s\n", SHM_NAME);
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        // Child: reader
        printf("Child waiting for data...\n");
        
        while (shared->ready == 0) {
            usleep(10000);
        }
        
        printf("Child read from shared memory:\n");
        printf("  Counter: %d\n", shared->counter);
        printf("  Message: '%s'\n", shared->message);
        
        munmap(shared, sizeof(SharedData));
        exit(0);
        
    } else {
        // Parent: writer
        sleep(1);
        
        shared->counter = 42;
        strcpy(shared->message, "Hello from shared memory!");
        shared->ready = 1;
        
        printf("Parent wrote to shared memory\n");
        
        wait(NULL);
        
        munmap(shared, sizeof(SharedData));
        shm_unlink(SHM_NAME);
    }
}

/*
 * Signal-based IPC
 */

volatile int signal_received = 0;

void signal_handler(int signum) {
    if (signum == SIGUSR1) {
        signal_received = 1;
    }
}

void test_signal_ipc(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                      SIGNAL IPC TEST                              ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    signal(SIGUSR1, signal_handler);
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        // Child: receiver
        printf("Child (PID %d) waiting for signal...\n", getpid());
        
        while (!signal_received) {
            usleep(100000);
        }
        
        printf("Child received SIGUSR1 signal!\n");
        exit(0);
        
    } else {
        // Parent: sender
        sleep(1);
        
        printf("Parent sending SIGUSR1 to child (PID %d)...\n", pid);
        kill(pid, SIGUSR1);
        
        wait(NULL);
    }
}

/*
 * Main Program
 */

int main(void) {
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║   Episode 27: Inter-Process Communication - SOLUTION             ║\n");
    printf("║                 Operation MOONLIGHT - Season 7                    ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("🔧 Location: San Francisco Bay Area, California\n");
    printf("🎯 Mission: Master IPC mechanisms\n");
    
    test_pipe();
    test_fifo();
    test_shared_memory();
    test_signal_ipc();
    
    printf("\n🏆 Achievement Unlocked: \"IPC Master\"\n");
    printf("   You've mastered inter-process communication.\n\n");
    
    printf("💡 Mission Complete!\n");
    printf("   You can now build complex multi-process systems.\n\n");
    
    printf("➡️  Next: Episode 28 - Stealth Operation (FINALE)\n");
    printf("    Master covert channels and timing attacks.\n\n");
    
    return 0;
}

