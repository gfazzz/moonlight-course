/*
 * Episode 25: Processes & Daemons
 * Operation MOONLIGHT - Season 7
 * 
 * Mission: Master process management and daemon creation
 * 
 * Your tasks:
 * 1. Create child processes with fork()
 * 2. Execute external programs with exec()
 * 3. Create proper UNIX daemon
 * 4. Handle signals (SIGTERM, SIGUSR1, SIGCHLD)
 * 5. Manage zombie and orphan processes
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define PID_FILE "/tmp/moonlight_daemon.pid"
#define LOG_FILE "/tmp/moonlight_daemon.log"

/*
 * TODO: Implement these functions
 */

// Signal handler
void signal_handler(int signum) {
    // TODO: Handle SIGTERM, SIGUSR1, etc.
}

// Setup signal handlers
void setup_signal_handlers(void) {
    // TODO: Install signal handlers using sigaction
}

// Daemonize process
void daemonize(void) {
    // TODO: Implement proper daemon creation:
    // 1. Fork and exit parent
    // 2. Create new session (setsid)
    // 3. Fork again to prevent terminal re-acquisition
    // 4. Change working directory to /
    // 5. Close file descriptors
    // 6. Redirect stdin/stdout/stderr to /dev/null
}

// Create PID file
int create_pid_file(void) {
    // TODO: Create and lock PID file
    return 0;
}

// Log message to file
void daemon_log(const char *message) {
    // TODO: Append message with timestamp to LOG_FILE
}

// Run daemon main loop
void run_daemon(void) {
    // TODO: Main daemon loop with work and signal handling
}

// Create child process
void create_child_process(const char *name) {
    // TODO: Use fork() to create child
    // Child should do some work and exit
    // Parent should wait for child
}

// Execute external program
void exec_external_program(void) {
    // TODO: Fork and use exec() to run external command
    // Example: execlp("ps", "ps", "-l", NULL);
}

// Demonstrate zombie process
void demonstrate_zombie_process(void) {
    // TODO: Create child that exits immediately
    // Parent sleeps without reaping child (creates zombie)
}

// Demonstrate orphan process
void demonstrate_orphan_process(void) {
    // TODO: Create child that sleeps
    // Parent exits immediately (child becomes orphan, adopted by init)
}

int main(int argc, char *argv[]) {
    printf("=== Episode 25: Processes & Daemons ===\n\n");
    
    if (argc < 2) {
        printf("Usage: %s [--daemon|--fork|--zombie|--orphan]\n", argv[0]);
        return 1;
    }
    
    // TODO: Implement different test modes
    
    printf("\n=== Process Management Complete ===\n");
    
    return 0;
}

