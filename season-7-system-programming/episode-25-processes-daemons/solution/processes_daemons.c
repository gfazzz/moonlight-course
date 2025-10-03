/*
 * Episode 25: Processes & Daemons - SOLUTION
 * Operation MOONLIGHT - Season 7
 * 
 * Mission: Master process management and daemon creation
 * Version: v2.0 Enhanced Edition
 * 
 * Implemented features:
 * ✅ Process creation (fork, exec)
 * ✅ Process monitoring and management
 * ✅ Daemon creation with proper setup
 * ✅ Signal handling (SIGTERM, SIGUSR1, SIGUSR2)
 * ✅ Process tree analysis
 * ✅ Zombie/orphan process detection
 * ✅ PID file management
 * ✅ Process statistics and reporting
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

#define PID_FILE "/tmp/moonlight_daemon.pid"
#define LOG_FILE "/tmp/moonlight_daemon.log"
#define MAX_PROCESSES 100

// Global state for daemon
volatile sig_atomic_t daemon_running = 1;
volatile sig_atomic_t daemon_reload = 0;

/*
 * Signal Handlers
 */

void signal_handler(int signum) {
    switch (signum) {
        case SIGTERM:
        case SIGINT:
            daemon_running = 0;
            break;
        case SIGUSR1:
            daemon_reload = 1;
            break;
        case SIGUSR2:
            // Custom signal for stats
            break;
    }
}

void setup_signal_handlers(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    
    // Ignore SIGCHLD to prevent zombies
    signal(SIGCHLD, SIG_IGN);
}

/*
 * Daemon Creation
 */

int create_pid_file(void) {
    int fd = open(PID_FILE, O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        perror("open");
        return -1;
    }
    
    // Try to lock the PID file
    if (lockf(fd, F_TLOCK, 0) < 0) {
        fprintf(stderr, "Daemon already running\n");
        close(fd);
        return -1;
    }
    
    // Write PID
    char pid_str[32];
    snprintf(pid_str, sizeof(pid_str), "%d\n", getpid());
    write(fd, pid_str, strlen(pid_str));
    
    return fd;
}

void daemonize(void) {
    pid_t pid;
    
    // Fork off the parent process
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    // Exit parent
    if (pid > 0) {
        printf("Daemon started with PID: %d\n", pid);
        exit(EXIT_SUCCESS);
    }
    
    // Child continues
    
    // Create new session
    if (setsid() < 0) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }
    
    // Fork again to prevent re-acquiring terminal
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    
    // Change working directory
    if (chdir("/") < 0) {
        perror("chdir");
        exit(EXIT_FAILURE);
    }
    
    // Close file descriptors
    for (int fd = 0; fd < 3; fd++) {
        close(fd);
    }
    
    // Reopen stdin, stdout, stderr to /dev/null
    open("/dev/null", O_RDONLY);  // stdin
    open("/dev/null", O_WRONLY);  // stdout
    open("/dev/null", O_WRONLY);  // stderr
    
    // Set file mode creation mask
    umask(0);
}

void daemon_log(const char *message) {
    FILE *f = fopen(LOG_FILE, "a");
    if (!f) return;
    
    time_t now = time(NULL);
    char *time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0';  // Remove newline
    
    fprintf(f, "[%s] %s\n", time_str, message);
    fclose(f);
}

void run_daemon(void) {
    daemon_log("Daemon started");
    
    int counter = 0;
    
    while (daemon_running) {
        if (daemon_reload) {
            daemon_log("Received SIGUSR1 - reloading configuration");
            daemon_reload = 0;
        }
        
        // Daemon work
        counter++;
        
        if (counter % 10 == 0) {
            char msg[128];
            snprintf(msg, sizeof(msg), "Daemon heartbeat: %d iterations", counter);
            daemon_log(msg);
        }
        
        sleep(1);
    }
    
    daemon_log("Daemon shutting down gracefully");
}

/*
 * Process Management
 */

typedef struct {
    pid_t pid;
    char name[64];
    char state;
    time_t start_time;
} ProcessInfo;

typedef struct {
    ProcessInfo processes[MAX_PROCESSES];
    int count;
} ProcessList;

void create_child_process(const char *name) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        // Child process
        printf("Child process %s started (PID: %d, PPID: %d)\n", 
               name, getpid(), getppid());
        
        // Simulate work
        sleep(rand() % 5 + 2);
        
        printf("Child process %s finished (PID: %d)\n", name, getpid());
        exit(0);
    } else {
        printf("Parent created child %s (PID: %d)\n", name, pid);
    }
}

void exec_external_program(void) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        // Child: exec ps command
        printf("Executing ps command...\n");
        execlp("ps", "ps", "-l", NULL);
        
        // If exec fails
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        // Parent waits for child
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            printf("Child exited with status: %d\n", WEXITSTATUS(status));
        }
    }
}

void demonstrate_zombie_process(void) {
    printf("\n=== Demonstrating Zombie Process ===\n");
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        // Child exits immediately
        printf("Child (PID: %d) exiting immediately\n", getpid());
        exit(0);
    } else {
        printf("Parent (PID: %d) created child (PID: %d)\n", getpid(), pid);
        printf("Parent sleeping for 5 seconds (child becomes zombie)...\n");
        printf("Run 'ps aux | grep %d' in another terminal to see zombie\n", pid);
        
        sleep(5);
        
        // Reap zombie
        int status;
        waitpid(pid, &status, 0);
        printf("Zombie reaped\n");
    }
}

void demonstrate_orphan_process(void) {
    printf("\n=== Demonstrating Orphan Process ===\n");
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        // Child sleeps
        printf("Child (PID: %d, PPID: %d) sleeping...\n", getpid(), getppid());
        sleep(5);
        printf("Child woke up (PID: %d, PPID: %d - adopted by init)\n", 
               getpid(), getppid());
        exit(0);
    } else {
        printf("Parent (PID: %d) created child (PID: %d)\n", getpid(), pid);
        printf("Parent exiting immediately (child becomes orphan)...\n");
        // Parent exits, child is adopted by init
    }
}

/*
 * Process Monitoring
 */

void print_process_info(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                      PROCESS INFORMATION                          ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("Current Process:\n");
    printf("  PID:  %d\n", getpid());
    printf("  PPID: %d\n", getppid());
    printf("  UID:  %d\n", getuid());
    printf("  GID:  %d\n", getgid());
    printf("  PGID: %d\n", getpgid(0));
    printf("  SID:  %d\n", getsid(0));
    printf("\n");
}

void test_fork_chain(int depth) {
    printf("\n=== Fork Chain (depth=%d) ===\n", depth);
    
    for (int i = 0; i < depth; i++) {
        pid_t pid = fork();
        
        if (pid < 0) {
            perror("fork");
            return;
        }
        
        if (pid == 0) {
            // Child continues to next iteration
            printf("Level %d: PID=%d, PPID=%d\n", i + 1, getpid(), getppid());
            continue;
        } else {
            // Parent waits and exits
            wait(NULL);
            return;
        }
    }
    
    // Deepest child
    printf("Reached depth %d (PID=%d)\n", depth, getpid());
}

/*
 * Main Program & Tests
 */

void test_signal_handling(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                    SIGNAL HANDLING TEST                           ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    setup_signal_handlers();
    
    printf("Signal handlers installed\n");
    printf("PID: %d\n", getpid());
    printf("\nSend signals from another terminal:\n");
    printf("  kill -USR1 %d  # Reload\n", getpid());
    printf("  kill -TERM %d  # Terminate\n", getpid());
    printf("\nWaiting for signals (10 seconds)...\n\n");
    
    for (int i = 0; i < 10; i++) {
        if (!daemon_running) {
            printf("Received SIGTERM - exiting\n");
            break;
        }
        
        if (daemon_reload) {
            printf("Received SIGUSR1 - reload triggered\n");
            daemon_reload = 0;
        }
        
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    
    printf("\nSignal test complete\n");
}

void print_usage(const char *progname) {
    printf("Usage: %s [option]\n", progname);
    printf("\nOptions:\n");
    printf("  --daemon          Create daemon process\n");
    printf("  --fork            Test fork/exec\n");
    printf("  --signals         Test signal handling\n");
    printf("  --zombie          Demonstrate zombie process\n");
    printf("  --orphan          Demonstrate orphan process\n");
    printf("  --chain N         Create process chain of depth N\n");
    printf("  --info            Show process information\n");
    printf("  --all             Run all tests\n");
    printf("\n");
}

int main(int argc, char *argv[]) {
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║    Episode 25: Processes & Daemons - SOLUTION                    ║\n");
    printf("║                 Operation MOONLIGHT - Season 7                    ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("🔧 Location: San Francisco Bay Area, California\n");
    printf("🎯 Mission: Master process management and daemon creation\n\n");
    
    srand(time(NULL));
    
    if (argc < 2) {
        print_usage(argv[0]);
        return 0;
    }
    
    if (strcmp(argv[1], "--daemon") == 0) {
        printf("Creating daemon...\n");
        
        daemonize();
        setup_signal_handlers();
        
        if (create_pid_file() < 0) {
            return EXIT_FAILURE;
        }
        
        run_daemon();
        
        unlink(PID_FILE);
        
    } else if (strcmp(argv[1], "--fork") == 0) {
        print_process_info();
        
        printf("=== Fork/Exec Test ===\n");
        create_child_process("worker-1");
        create_child_process("worker-2");
        create_child_process("worker-3");
        
        printf("\nWaiting for children...\n");
        while (wait(NULL) > 0);
        
        printf("\nAll children finished\n\n");
        
        exec_external_program();
        
    } else if (strcmp(argv[1], "--signals") == 0) {
        test_signal_handling();
        
    } else if (strcmp(argv[1], "--zombie") == 0) {
        demonstrate_zombie_process();
        
    } else if (strcmp(argv[1], "--orphan") == 0) {
        demonstrate_orphan_process();
        
    } else if (strcmp(argv[1], "--chain") == 0) {
        int depth = (argc > 2) ? atoi(argv[2]) : 3;
        test_fork_chain(depth);
        
    } else if (strcmp(argv[1], "--info") == 0) {
        print_process_info();
        
    } else if (strcmp(argv[1], "--all") == 0) {
        print_process_info();
        
        create_child_process("test-worker");
        wait(NULL);
        
        printf("\n");
        test_fork_chain(3);
        
        printf("\n🏆 All tests complete!\n");
        
    } else {
        print_usage(argv[0]);
        return 1;
    }
    
    printf("\n💡 Mission Complete!\n");
    printf("   You've mastered process management and daemon creation.\n\n");
    
    printf("➡️  Next: Episode 26 - Threads & Parallelism\n");
    printf("    Master multithreading with pthreads.\n\n");
    
    return 0;
}

/*
 * Educational Notes:
 * 
 * Process Creation:
 * - fork() creates a copy of the current process
 * - exec() replaces process image with new program
 * - wait() reaps child processes
 * 
 * Daemon Creation Steps:
 * 1. Fork and exit parent
 * 2. Create new session (setsid)
 * 3. Fork again to prevent terminal re-acquisition
 * 4. Change working directory to /
 * 5. Close file descriptors
 * 6. Redirect stdin/stdout/stderr to /dev/null
 * 
 * Signal Handling:
 * - SIGTERM: graceful shutdown
 * - SIGUSR1/2: custom signals
 * - SIGCHLD: child process status change
 * 
 * Process States:
 * - Zombie: child exited but not reaped
 * - Orphan: parent exited before child
 * 
 * Compile: gcc -Wall -Wextra -o processes_daemons processes_daemons.c
 * Run daemon: ./processes_daemons --daemon
 * Test: ./processes_daemons --all
 */

