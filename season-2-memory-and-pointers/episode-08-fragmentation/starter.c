/**
 * Episode 08: Fragmentation
 * STARTER VERSION - Fix the memory leaks!
 * 
 * TODO:
 * 1. Run valgrind/leaks to find all leaks
 * 2. Add cleanup functions
 * 3. Fix each leak identified
 * 4. Verify: 0 leaks remaining
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE 256

typedef struct {
    char *log_dir;
    int max_buffer_size;
    int rotation_size;
    int enable_timestamps;
} Config;

typedef struct LogEntry {
    char *message;
    time_t timestamp;
    struct LogEntry *next;
} LogEntry;

typedef struct {
    LogEntry *head;
    LogEntry *tail;
    int count;
} LogBuffer;

// Global state
static LogBuffer *buffer = NULL;
static Config *config = NULL;

// TODO: Add cleanup functions here

/**
 * TODO: Fix leak - free the line after use
 */
char* read_config_line(FILE *f) {
    char *line = malloc(MAX_LINE);
    if (!fgets(line, MAX_LINE, f)) {
        free(line);
        return NULL;
    }
    
    line[strcspn(line, "\n")] = 0;
    return line;
}

/**
 * TODO: Free config->log_dir and config itself
 */
Config* load_config(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        return NULL;
    }
    
    Config *cfg = malloc(sizeof(Config));
    cfg->log_dir = malloc(MAX_LINE);
    strcpy(cfg->log_dir, "/tmp/logs");
    cfg->max_buffer_size = 100;
    cfg->rotation_size = 1024;
    cfg->enable_timestamps = 1;
    
    char *line;
    while ((line = read_config_line(f)) != NULL) {
        if (strncmp(line, "log_dir=", 8) == 0) {
            strcpy(cfg->log_dir, line + 8);
        }
        // TODO: Free line here!
    }
    
    fclose(f);
    return cfg;
}

/**
 * TODO: Ensure entry->message is freed when entry is freed
 */
void add_log_entry(const char *message) {
    LogEntry *entry = malloc(sizeof(LogEntry));
    entry->message = malloc(strlen(message) + 1);
    strcpy(entry->message, message);
    entry->timestamp = time(NULL);
    entry->next = NULL;
    
    if (!buffer->head) {
        buffer->head = buffer->tail = entry;
    } else {
        buffer->tail->next = entry;
        buffer->tail = entry;
    }
    buffer->count++;
}

/**
 * TODO: Free old buffer before creating new one
 */
void rotate_logs() {
    printf("Rotating logs... ");
    
    // TODO: Free old buffer entries first!
    
    LogBuffer *new_buffer = malloc(sizeof(LogBuffer));
    new_buffer->head = new_buffer->tail = NULL;
    new_buffer->count = 0;
    
    buffer = new_buffer;
    
    printf("Done.\n");
}

/**
 * TODO: Free time_str after use
 */
void print_logs() {
    LogEntry *current = buffer->head;
    
    while (current) {
        if (config->enable_timestamps) {
            char *time_str = malloc(64);
            struct tm *tm_info = localtime(&current->timestamp);
            strftime(time_str, 64, "[%Y-%m-%d %H:%M:%S] ", tm_info);
            printf("%s%s\n", time_str, current->message);
            // TODO: Free time_str!
        } else {
            printf("%s\n", current->message);
        }
        current = current->next;
    }
}

void init_buffer() {
    buffer = malloc(sizeof(LogBuffer));
    buffer->head = buffer->tail = NULL;
    buffer->count = 0;
}

// TODO: Add cleanup functions:
// - cleanup_config()
// - cleanup_buffer()
// - cleanup_log_entry()

int main(int argc, char *argv[]) {
    config = load_config("config.txt");
    if (!config) {
        fprintf(stderr, "Failed to load config, using defaults\n");
        config = malloc(sizeof(Config));
        config->log_dir = malloc(MAX_LINE);
        strcpy(config->log_dir, "/tmp/logs");
        config->max_buffer_size = 100;
        config->rotation_size = 1024;
        config->enable_timestamps = 1;
    }
    
    init_buffer();
    
    printf("MOONLIGHT Logger (fixing version)\n");
    printf("Commands: log <msg>, print, rotate, quit\n\n");
    
    char command[MAX_LINE];
    while (1) {
        printf("> ");
        if (!fgets(command, MAX_LINE, stdin)) {
            break;
        }
        
        command[strcspn(command, "\n")] = 0;
        
        if (strncmp(command, "log ", 4) == 0) {
            add_log_entry(command + 4);
            printf("Logged.\n");
        } else if (strcmp(command, "print") == 0) {
            print_logs();
        } else if (strcmp(command, "rotate") == 0) {
            rotate_logs();
        } else if (strcmp(command, "quit") == 0) {
            break;
        } else {
            printf("Unknown command.\n");
        }
    }
    
    printf("Exiting...\n");
    
    // TODO: Add cleanup here!
    // cleanup_buffer(buffer);
    // cleanup_config(config);
    
    return 0;
}
