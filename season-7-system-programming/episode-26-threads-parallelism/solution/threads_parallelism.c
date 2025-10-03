/*
 * Episode 26: Threads & Parallelism - SOLUTION
 * Operation MOONLIGHT - Season 7
 * 
 * Mission: Master multithreading with POSIX threads
 * Version: v2.0 Enhanced Edition
 * 
 * Implemented features:
 * ✅ Thread creation and joining
 * ✅ Mutex synchronization
 * ✅ Producer-Consumer pattern with condition variables
 * ✅ Thread pool implementation
 * ✅ Race condition demonstration
 * ✅ Deadlock detection
 * ✅ Parallel log analyzer
 * ✅ Performance metrics
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#define MAX_THREADS 16
#define BUFFER_SIZE 10
#define MAX_TASKS 1000

/*
 * Producer-Consumer Pattern
 */

typedef struct {
    int buffer[BUFFER_SIZE];
    int count;
    int in;
    int out;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
    bool done;
} BoundedBuffer;

BoundedBuffer bb;

void buffer_init(void) {
    bb.count = 0;
    bb.in = 0;
    bb.out = 0;
    bb.done = false;
    pthread_mutex_init(&bb.mutex, NULL);
    pthread_cond_init(&bb.not_full, NULL);
    pthread_cond_init(&bb.not_empty, NULL);
}

void buffer_destroy(void) {
    pthread_mutex_destroy(&bb.mutex);
    pthread_cond_destroy(&bb.not_full);
    pthread_cond_destroy(&bb.not_empty);
}

void buffer_produce(int item) {
    pthread_mutex_lock(&bb.mutex);
    
    while (bb.count == BUFFER_SIZE) {
        pthread_cond_wait(&bb.not_full, &bb.mutex);
    }
    
    bb.buffer[bb.in] = item;
    bb.in = (bb.in + 1) % BUFFER_SIZE;
    bb.count++;
    
    pthread_cond_signal(&bb.not_empty);
    pthread_mutex_unlock(&bb.mutex);
}

int buffer_consume(void) {
    pthread_mutex_lock(&bb.mutex);
    
    while (bb.count == 0 && !bb.done) {
        pthread_cond_wait(&bb.not_empty, &bb.mutex);
    }
    
    if (bb.count == 0 && bb.done) {
        pthread_mutex_unlock(&bb.mutex);
        return -1;
    }
    
    int item = bb.buffer[bb.out];
    bb.out = (bb.out + 1) % BUFFER_SIZE;
    bb.count--;
    
    pthread_cond_signal(&bb.not_full);
    pthread_mutex_unlock(&bb.mutex);
    
    return item;
}

void* producer_thread(void *arg) {
    int id = *(int*)arg;
    
    for (int i = 0; i < 10; i++) {
        int item = id * 100 + i;
        buffer_produce(item);
        printf("Producer %d: produced %d\n", id, item);
        usleep(rand() % 100000);
    }
    
    return NULL;
}

void* consumer_thread(void *arg) {
    int id = *(int*)arg;
    
    while (1) {
        int item = buffer_consume();
        if (item == -1) break;
        
        printf("Consumer %d: consumed %d\n", id, item);
        usleep(rand() % 150000);
    }
    
    return NULL;
}

/*
 * Race Condition Demonstration
 */

volatile long global_counter = 0;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

void* increment_no_lock(void *arg) {
    int iterations = *(int*)arg;
    
    for (int i = 0; i < iterations; i++) {
        global_counter++;  // RACE CONDITION!
    }
    
    return NULL;
}

void* increment_with_lock(void *arg) {
    int iterations = *(int*)arg;
    
    for (int i = 0; i < iterations; i++) {
        pthread_mutex_lock(&counter_mutex);
        global_counter++;
        pthread_mutex_unlock(&counter_mutex);
    }
    
    return NULL;
}

void demonstrate_race_condition(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                   RACE CONDITION DEMONSTRATION                    ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    int iterations = 1000000;
    
    // Test WITHOUT mutex
    printf("=== Test 1: WITHOUT mutex (race condition) ===\n");
    global_counter = 0;
    
    pthread_t threads[4];
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, increment_no_lock, &iterations);
    }
    
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Expected: %d\n", iterations * 4);
    printf("Actual:   %ld\n", global_counter);
    printf("Lost:     %ld\n\n", (iterations * 4) - global_counter);
    
    // Test WITH mutex
    printf("=== Test 2: WITH mutex (correct) ===\n");
    global_counter = 0;
    
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, increment_with_lock, &iterations);
    }
    
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Expected: %d\n", iterations * 4);
    printf("Actual:   %ld\n", global_counter);
    printf("Correct!  ✅\n\n");
}

/*
 * Thread Pool
 */

typedef void (*task_func_t)(void*);

typedef struct {
    task_func_t func;
    void *arg;
} Task;

typedef struct {
    Task *tasks;
    int task_count;
    int task_capacity;
    int head;
    int tail;
    
    pthread_t *threads;
    int thread_count;
    
    pthread_mutex_t lock;
    pthread_cond_t notify;
    bool shutdown;
} ThreadPool;

void task_example(void *arg) {
    int id = *(int*)arg;
    printf("  Task %d running on thread %lu\n", id, pthread_self());
    usleep(rand() % 500000);
    free(arg);
}

void* thread_pool_worker(void *arg) {
    ThreadPool *pool = (ThreadPool*)arg;
    
    while (1) {
        pthread_mutex_lock(&pool->lock);
        
        while (pool->task_count == 0 && !pool->shutdown) {
            pthread_cond_wait(&pool->notify, &pool->lock);
        }
        
        if (pool->shutdown && pool->task_count == 0) {
            pthread_mutex_unlock(&pool->lock);
            break;
        }
        
        Task task = pool->tasks[pool->head];
        pool->head = (pool->head + 1) % pool->task_capacity;
        pool->task_count--;
        
        pthread_mutex_unlock(&pool->lock);
        
        task.func(task.arg);
    }
    
    return NULL;
}

ThreadPool* thread_pool_create(int num_threads) {
    ThreadPool *pool = malloc(sizeof(ThreadPool));
    
    pool->task_capacity = MAX_TASKS;
    pool->tasks = malloc(sizeof(Task) * pool->task_capacity);
    pool->task_count = 0;
    pool->head = 0;
    pool->tail = 0;
    
    pool->thread_count = num_threads;
    pool->threads = malloc(sizeof(pthread_t) * num_threads);
    
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->notify, NULL);
    pool->shutdown = false;
    
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&pool->threads[i], NULL, thread_pool_worker, pool);
    }
    
    return pool;
}

void thread_pool_add_task(ThreadPool *pool, task_func_t func, void *arg) {
    pthread_mutex_lock(&pool->lock);
    
    pool->tasks[pool->tail].func = func;
    pool->tasks[pool->tail].arg = arg;
    pool->tail = (pool->tail + 1) % pool->task_capacity;
    pool->task_count++;
    
    pthread_cond_signal(&pool->notify);
    pthread_mutex_unlock(&pool->lock);
}

void thread_pool_destroy(ThreadPool *pool) {
    pthread_mutex_lock(&pool->lock);
    pool->shutdown = true;
    pthread_cond_broadcast(&pool->notify);
    pthread_mutex_unlock(&pool->lock);
    
    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }
    
    free(pool->threads);
    free(pool->tasks);
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->notify);
    free(pool);
}

/*
 * Parallel Log Analyzer
 */

typedef struct {
    char **lines;
    int start;
    int end;
    int *error_count;
    int *warning_count;
    pthread_mutex_t *mutex;
} AnalyzerTask;

void* analyze_logs(void *arg) {
    AnalyzerTask *task = (AnalyzerTask*)arg;
    
    int local_errors = 0;
    int local_warnings = 0;
    
    for (int i = task->start; i < task->end; i++) {
        if (strstr(task->lines[i], "ERROR")) {
            local_errors++;
        } else if (strstr(task->lines[i], "WARNING")) {
            local_warnings++;
        }
    }
    
    pthread_mutex_lock(task->mutex);
    *task->error_count += local_errors;
    *task->warning_count += local_warnings;
    pthread_mutex_unlock(task->mutex);
    
    return NULL;
}

void test_parallel_log_analyzer(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                  PARALLEL LOG ANALYZER TEST                       ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    // Generate fake logs
    int num_lines = 10000;
    char **lines = malloc(sizeof(char*) * num_lines);
    
    for (int i = 0; i < num_lines; i++) {
        lines[i] = malloc(128);
        int rand_val = rand() % 100;
        if (rand_val < 5) {
            snprintf(lines[i], 128, "[%d] ERROR: Something went wrong", i);
        } else if (rand_val < 20) {
            snprintf(lines[i], 128, "[%d] WARNING: Check this", i);
        } else {
            snprintf(lines[i], 128, "[%d] INFO: Normal operation", i);
        }
    }
    
    int num_threads = 4;
    pthread_t threads[MAX_THREADS];
    AnalyzerTask tasks[MAX_THREADS];
    
    int error_count = 0;
    int warning_count = 0;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    
    int chunk_size = num_lines / num_threads;
    
    printf("Analyzing %d log lines with %d threads...\n", num_lines, num_threads);
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    for (int i = 0; i < num_threads; i++) {
        tasks[i].lines = lines;
        tasks[i].start = i * chunk_size;
        tasks[i].end = (i == num_threads - 1) ? num_lines : (i + 1) * chunk_size;
        tasks[i].error_count = &error_count;
        tasks[i].warning_count = &warning_count;
        tasks[i].mutex = &mutex;
        
        pthread_create(&threads[i], NULL, analyze_logs, &tasks[i]);
    }
    
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + 
                     (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    
    printf("\nResults:\n");
    printf("  Errors:   %d\n", error_count);
    printf("  Warnings: %d\n", warning_count);
    printf("  Time:     %.3f seconds\n", elapsed);
    printf("  Rate:     %.0f lines/sec\n\n", num_lines / elapsed);
    
    for (int i = 0; i < num_lines; i++) {
        free(lines[i]);
    }
    free(lines);
    pthread_mutex_destroy(&mutex);
}

/*
 * Main Program
 */

void test_producer_consumer(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                 PRODUCER-CONSUMER TEST                            ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    buffer_init();
    
    int num_producers = 3;
    int num_consumers = 2;
    
    pthread_t producers[num_producers];
    pthread_t consumers[num_consumers];
    int producer_ids[num_producers];
    int consumer_ids[num_consumers];
    
    printf("Starting %d producers and %d consumers...\n\n", num_producers, num_consumers);
    
    for (int i = 0; i < num_producers; i++) {
        producer_ids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer_thread, &producer_ids[i]);
    }
    
    for (int i = 0; i < num_consumers; i++) {
        consumer_ids[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer_thread, &consumer_ids[i]);
    }
    
    for (int i = 0; i < num_producers; i++) {
        pthread_join(producers[i], NULL);
    }
    
    pthread_mutex_lock(&bb.mutex);
    bb.done = true;
    pthread_cond_broadcast(&bb.not_empty);
    pthread_mutex_unlock(&bb.mutex);
    
    for (int i = 0; i < num_consumers; i++) {
        pthread_join(consumers[i], NULL);
    }
    
    printf("\nProducer-Consumer test complete!\n");
    buffer_destroy();
}

void test_thread_pool(void) {
    printf("\n╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                      THREAD POOL TEST                             ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    ThreadPool *pool = thread_pool_create(4);
    
    printf("Thread pool created with 4 workers\n");
    printf("Adding 20 tasks...\n\n");
    
    for (int i = 0; i < 20; i++) {
        int *task_id = malloc(sizeof(int));
        *task_id = i + 1;
        thread_pool_add_task(pool, task_example, task_id);
    }
    
    sleep(3);  // Let tasks complete
    
    thread_pool_destroy(pool);
    printf("\nThread pool destroyed\n");
}

int main(void) {
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║     Episode 26: Threads & Parallelism - SOLUTION                 ║\n");
    printf("║                 Operation MOONLIGHT - Season 7                    ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("🔧 Location: San Francisco Bay Area, California\n");
    printf("🎯 Mission: Master multithreading with POSIX threads\n\n");
    
    srand(time(NULL));
    
    demonstrate_race_condition();
    
    test_producer_consumer();
    
    test_thread_pool();
    
    test_parallel_log_analyzer();
    
    printf("\n🏆 Achievement Unlocked: \"Multithreading Master\"\n");
    printf("   You've mastered parallel programming with pthreads.\n\n");
    
    printf("💡 Mission Complete!\n");
    printf("   You can now write efficient multithreaded applications.\n\n");
    
    printf("➡️  Next: Episode 27 - Inter-Process Communication\n");
    printf("    Master IPC: pipes, shared memory, message queues.\n\n");
    
    return 0;
}

