# Mission Brief: Inter-Process Communication
## Episode 27 — Network Silence Mode

---

## 🎯 Objective

**CRITICAL:** Shutdown all network activity after Episode 26 discovery.  
**MISSION:** Establish covert IPC channels for process communication WITHOUT network.  
**CHALLENGE:** Zero network packets (enemy monitoring must see NOTHING).  
**DEADLINE:** 8.5 hours until deep scan (Dec 25, 03:00)

---

## 📋 Mission Requirements

### 1. Anonymous Pipes
- ✅ Create pipe pairs (parent-child communication)
- ✅ Bidirectional pipes (2 pipes for request-response)
- ✅ File descriptor management (close unused ends)
- ✅ Handle pipe capacity (65KB Linux default)
- ✅ Blocking behavior (PIPE_BUF atomicity)

### 2. Named Pipes (FIFOs)
- ✅ Create FIFOs in filesystem (mkfifo)
- ✅ Multi-process communication (any process can access)
- ✅ Stealth FIFOs (Linux: /dev/shm/, macOS/BSD: /tmp/ + random names)
- ✅ Auto-deletion after use (no traces)
- ✅ Permissions (0600, owner-only)

### 3. Shared Memory
- ✅ POSIX shared memory (shm_open, mmap)
- ✅ Large datasets (50 MB real-time activity log)
- ✅ Process-shared mutex (PTHREAD_PROCESS_SHARED)
- ✅ Race condition prevention (mutex synchronization)
- ✅ Memory-mapped I/O (direct access, no copying)

### 4. Message Queues
- ✅ POSIX message queues (mq_open, mq_send, mq_receive)
- ✅ Structured messages (IntelligenceMessage struct)
- ✅ Priority-based delivery (critical messages first)
- ✅ Non-blocking I/O (O_NONBLOCK, mq_timedreceive)
- ✅ Asynchronous notification (SIGEV_SIGNAL)

### 5. Signals
- ✅ Standard signals (SIGUSR1, SIGUSR2)
- ✅ Realtime signals (SIGRTMIN-SIGRTMAX)
- ✅ Signal payload (sival_int, sival_ptr)
- ✅ Covert encoding (bit sequences via signal types)
- ✅ Signal queuing (realtime signals queued, standard not)

### 6. Performance Analysis
- ✅ Throughput comparison (pipes vs shared memory vs network)
- ✅ Latency measurement (sub-millisecond IPC)
- ✅ Covert channel bandwidth (signals: ~20 bytes/sec)
- ✅ Trade-offs (speed vs stealth vs complexity)

---

## 🧪 Testing Criteria

### Pipe Tests
```bash
# Anonymous pipe (parent-child)
./ipc_test --pipe-anonymous

# Named pipe (FIFO)
./ipc_test --pipe-named /tmp/test_fifo

# Bidirectional pipes
./ipc_test --pipe-bidirectional

# Pipe capacity test (blocking)
./ipc_test --pipe-capacity 100000  # 100 KB write (should block at 65 KB)
```

### Shared Memory Tests
```bash
# Create shared memory
./ipc_test --shm-create /moonlight_test 1048576  # 1 MB

# Attach multiple processes
./ipc_test --shm-attach /moonlight_test &  # Process 1
./ipc_test --shm-attach /moonlight_test &  # Process 2

# Race condition test (without mutex - should corrupt)
./ipc_test --shm-race-unsafe

# Mutex synchronization (with mutex - should work)
./ipc_test --shm-race-safe
```

### Message Queue Tests
```bash
# Create message queue
./ipc_test --mq-create /test_queue

# Priority test (send low prio, then high prio)
./ipc_test --mq-send /test_queue "Low priority" 1
./ipc_test --mq-send /test_queue "HIGH PRIORITY" 9
./ipc_test --mq-receive /test_queue  # Should receive HIGH PRIORITY first

# Non-blocking receive (empty queue)
./ipc_test --mq-receive-nonblock /test_queue  # Should return EAGAIN immediately
```

### Signal Tests
```bash
# Basic signals
./ipc_test --signal-basic <PID>  # Send SIGUSR1, SIGUSR2

# Realtime signals with payload
./ipc_test --signal-rtmin <PID> 4789  # Send SIGRTMIN with value 4789

# Signal encoding (send string via bit sequence)
./ipc_test --signal-encode <PID> "ALERT"  # 40 signals (5 bytes × 8 bits)
```

### Performance Tests
```bash
# Pipe throughput
time ./ipc_test --perf-pipe 52428800  # 50 MB transfer
# Expected: ~19.5 MB/s

# Shared memory throughput
time ./ipc_test --perf-shm 52428800  # 50 MB transfer
# Expected: ~561.8 MB/s (28.8x faster)

# Signal bandwidth
time ./ipc_test --perf-signal 1000  # 1000 bytes via signals
# Expected: ~50 seconds (~20 bytes/sec)
```

---

## 📦 Deliverables

### Artifacts (3 files, 1,090 lines):
- ✅ `pipe_communication.log` (238 lines)
  - Daemon shutdown sequence (network silence)
  - Anonymous pipe creation & data transmission
  - Named pipes (FIFOs) for multi-process
  - Stealth FIFOs (Linux: /dev/shm/, macOS/BSD: /tmp/, random names, auto-delete)
  - Bidirectional pipes (2-pipe setup)
  - Signal-based IPC (bonus: low-bandwidth covert channel)
  
- ✅ `shared_memory.log` (435 lines)
  - POSIX shared memory creation (shm_open, ftruncate, mmap)
  - Process attachment (3 processes sharing 50 MB)
  - Data structure layout (ActivityLog with 10,000 entries)
  - Real-time monitoring (47 events in 1 minute)
  - Performance comparison (28.8x faster than pipes)
  - Race condition demo & mutex synchronization
  - Process-shared mutex configuration
  
- ✅ `signals_and_message_queues.log` (417 lines)
  - Signals for covert communication (SIGUSR1/2, SIGRTMIN)
  - Realtime signals with payload (int transmission)
  - Signal-based covert encoding (multi-bit, "ALERT" example)
  - Signal queue reliability (standard vs realtime)
  - POSIX message queues (mq_open, mq_send, mq_receive)
  - Priority-based delivery (high-priority first)
  - Non-blocking I/O & timeouts
  - Asynchronous notification (SIGEV_SIGNAL)

### Code Deliverables:
- ✅ `solution/ipc.c` (280 lines)
- ✅ `starter.c` (166 lines)
- ✅ `solution/Makefile` + root `Makefile`

---

## 📊 Success Metrics

- [x] Network traffic: ZERO (daemon shut down, all sockets closed)
- [x] 5 IPC mechanisms deployed (pipes, FIFOs, shared memory, message queues, signals)
- [x] Covert channels established (signals = ultra-stealth, kernel-only)
- [x] Real-time monitoring (50 MB shared memory, 47 events logged)
- [x] Performance measured (shared memory 28.8x faster than pipes)
- [x] New threat identified (tracker2, PID 4789)
- [x] Intelligence gathered (47 events: 12 critical, 23 warnings, 12 info)
- [x] Data exfiltration detected (2.3 MB to C2 server)

---

## ⚠️ Mission Status

**ACCOMPLISHED** ✅

All network activity shut down. IPC channels operational.  
Enemy network monitoring: sees NOTHING.

**CRITICAL FINDING:**
- New enemy process: tracker2 (PID 4789) - upgraded surveillance tool
- Activity: /etc/passwd access, port scanning, ptrace attempts, data exfiltration
- Deep scan countdown: 5.5 hours remaining (Dec 25, 03:00)

**URGENT:** Proceed to Episode 28 immediately.  
Stealth operation required. Timing attacks. Final confrontation.

---

**Next Mission:** [Episode 28: Stealth Operation →](../episode-28-stealth-operation/)

---

*MOONLIGHT Protocol: Silent. Invisible. Untraceable. 📡*
