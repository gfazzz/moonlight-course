# Mission Brief: Big Data Processing
**Episode 29** | Operation MOONLIGHT — Season 8

---

## 🎯 Mission Objective

**PRIMARY GOAL:** Process massive datasets (247.8 MB network logs + sensor data) to prepare for AI training.

**CONTEXT:**  
6 days after Season 7 victory, we've detected reconnaissance activity. Enemy tested our systems with:
- SSH brute-force attacks (1,847 attempts)
- RDP scanning (892 attempts)
- HTTP flood attacks (654 requests/2s)
- Physical sensor anomalies (temperature spikes to 38.2°C)

**YOUR TASK:**  
Implement big data processing pipeline in C to analyze ALL collected data and identify attack patterns.

**DELIVERABLE:**  
AI-ready dataset for Episodes 30-32 (statistical analysis → neural networks → prediction).

---

## 📋 Technical Requirements

### 1. Memory-Mapped File Processing (mmap)

**Requirement:**  
Process `network_traffic.bin` (247.8 MB, 523,847 records) using memory-mapped I/O.

**Implementation:**
```c
// Open binary file
int fd = open("network_traffic.bin", O_RDONLY);

// Get file size
struct stat sb;
fstat(fd, &sb);

// Memory-map file (POSIX - works on Linux/macOS/FreeBSD)
void *mapped = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

// Optional: Advise kernel about sequential access
posix_madvise(mapped, sb.st_size, POSIX_MADV_SEQUENTIAL);

// Process as NetworkLog array
NetworkLog *logs = (NetworkLog *)mapped;
size_t count = sb.st_size / sizeof(NetworkLog);

// ... process data ...

// Cleanup
munmap(mapped, sb.st_size);
close(fd);
```

**Binary Format:**
```c
typedef struct {
    uint64_t timestamp;      // Unix microseconds
    uint32_t src_ip;         // Network byte order
    uint32_t dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t protocol;       // 6=TCP, 17=UDP
    uint32_t bytes;
    uint8_t flags;
    uint8_t status;          // 0=normal, 1=suspicious, 2=threat
} __attribute__((packed)) NetworkLog;
```

**Expected Analysis:**
- Total records: 523,847
- Suspicious entries: ~12,847 (2.45%)
- Threat entries: ~347 (0.066%)
- Total bytes transferred: ~1.85 GB

---

### 2. Stream Processing (CSV)

**Requirement:**  
Process `sensor_data.csv` using streaming (constant memory usage).

**Implementation:**
```c
FILE *fp = fopen("sensor_data.csv", "r");
char line[4096];

// Skip header
fgets(line, sizeof(line), fp);

// Stream process (online algorithm)
SensorData data;
while (fgets(line, sizeof(line), fp)) {
    if (parse_csv_line(line, &data) == 0) {
        // Update statistics incrementally (constant memory)
        update_statistics(&stats, data.temperature);
        if (data.anomaly) anomaly_count++;
    }
}

fclose(fp);
```

**CSV Format:**
```
timestamp,sensor_id,temperature,humidity,pressure,anomaly
1640001000.123,SENSOR_A01,22.5,45.2,1013.25,0
```

**Expected Analysis:**
- Total records: 50
- Anomalies: 11 (22%)
- Temperature range: 19.5°C - 38.2°C
- Critical events: SENSOR_B02 (38.2°C, 88.9% humidity)

---

### 3. Statistical Aggregation (Thread-Safe)

**Requirement:**  
Implement thread-safe statistics accumulator for parallel processing.

**Functions to Implement:**
```c
typedef struct {
    uint64_t count;
    double sum;
    double sum_squared;
    double min;
    double max;
    pthread_mutex_t lock;
} Statistics;

void init_statistics(Statistics *stats);
void update_statistics(Statistics *stats, double value);
double get_mean(Statistics *stats);
double get_variance(Statistics *stats);
double get_stddev(Statistics *stats);
```

**Thread Safety:**
```c
void update_statistics(Statistics *stats, double value) {
    pthread_mutex_lock(&stats->lock);
    
    stats->count++;
    stats->sum += value;
    stats->sum_squared += value * value;
    if (value < stats->min) stats->min = value;
    if (value > stats->max) stats->max = value;
    
    pthread_mutex_unlock(&stats->lock);
}
```

---

### 4. Parallel Processing (Optional Enhancement)

**Requirement:**  
Use 4 worker threads to process data in parallel.

**Implementation:**
```c
#define THREAD_COUNT 4

void *parallel_worker(void *arg) {
    ThreadArg *targ = (ThreadArg *)arg;
    
    for (size_t i = targ->start_idx; i < targ->end_idx; i++) {
        update_statistics(targ->stats, (double)targ->logs[i].bytes);
    }
    
    return NULL;
}

// Create threads
pthread_t threads[THREAD_COUNT];
size_t chunk_size = total_records / THREAD_COUNT;

for (int i = 0; i < THREAD_COUNT; i++) {
    pthread_create(&threads[i], NULL, parallel_worker, &args[i]);
}

// Wait for completion
for (int i = 0; i < THREAD_COUNT; i++) {
    pthread_join(threads[i], NULL);
}
```

---

## 🧪 Testing Criteria

### Test 1: mmap Processing
```bash
cd solution
make
./big_data --mmap ../artifacts/network_traffic.bin
```

**Expected Output:**
```
📊 Network Traffic Statistics:
  Count: 523847
  Mean: 3527.4
  StdDev: 2184.9
  ⚠️  Suspicious: 12847 (2.45%)
  🚨 Threats: 347 (0.066%)
```

### Test 2: CSV Stream Processing
```bash
./big_data --stream ../artifacts/sensor_data.csv
```

**Expected Output:**
```
📊 Temperature Statistics:
  Count: 50
  Mean: 24.82
  StdDev: 4.15
  Min: 19.5
  Max: 38.2
  🔴 Anomalies: 11 (22.0%)
```

### Test 3: Full Pipeline
```bash
./big_data --all
```

**Should process both datasets and output comprehensive analysis.**

---

## 📦 Deliverables

### Files to Create:

1. **`solution/big_data.c`**  
   Complete implementation (~400-500 lines)

2. **`solution/Makefile`**  
   Cross-platform build (Linux/macOS/FreeBSD)

3. **`starter.c`**  
   Skeleton code with TODOs (~150 lines)

### Files Provided (in `artifacts/`):

1. **`network_traffic.bin`** (binary format, 247.8 MB)  
   - 523,847 NetworkLog records
   - Contains attack patterns (SSH, RDP, HTTP flood)

2. **`sensor_data.csv`** (text format, 3.2 KB)  
   - 50 sensor readings
   - 11 anomalies including critical events

3. **`analysis_results.json`**  
   - Expected analysis results
   - Validation data for your implementation

4. **`network_traffic_sample.txt`**  
   - Human-readable sample of binary logs
   - For understanding binary format

---

## 🎯 Success Criteria

✅ **Memory Efficiency:**  
- mmap uses constant memory (no matter file size)
- Streaming CSV parser uses <10 KB memory

✅ **Accuracy:**  
- Statistical calculations match `analysis_results.json`
- Anomaly detection: 11/50 (22%)
- Threat detection: 347/523847 (0.066%)

✅ **Performance:**  
- Process 247.8 MB in <5 seconds (with mmap)
- Parallel processing shows 2-4x speedup

✅ **Cross-Platform:**  
- Works on Linux, macOS, FreeBSD
- No platform-specific code (POSIX only)

✅ **AI Readiness:**  
- Data quality: GOOD (98.5% completeness)
- Dataset ready for Episode 30 (statistical analysis)

---

## 💡 Hints

1. **mmap vs read():**  
   - `mmap`: Constant memory, kernel manages paging
   - `read()`: Need to allocate memory for entire file

2. **Streaming vs Loading:**  
   - Streaming: Process line-by-line (constant memory)
   - Loading: Load entire file into memory (not scalable)

3. **Online Statistics:**  
   - Update mean/variance incrementally
   - Welford's algorithm for numerical stability

4. **Thread Safety:**  
   - Always lock mutex before updating shared state
   - Keep critical sections minimal

5. **Cross-Platform mmap:**  
   - POSIX standard: works everywhere
   - No need for platform detection

---

## 📊 Mission Success

**Upon completion, you will have:**
- Processed 247.8 MB of network logs
- Identified 347 active threats
- Analyzed 11 sensor anomalies
- Prepared dataset for AI training (Episodes 30-32)

**Viktor's Approval Message:**
```
Отличная работа, Agent. Dataset готов.
Episode 30 — statistical analysis. Stanford AI Lab ждёт.

V.
```

---

**Good luck, Agent!** 🚀  
**Remember:** In data we trust. Let the machines learn.
