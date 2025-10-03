# Mission Brief: Processes & Daemons
## Episode 25 — System Level War Begins

---

## 🎯 Objective

**CRITICAL:** Enemy surveillance detected in system (PIDs 3012, 3156).  
**MISSION:** Create counter-surveillance daemon, monitor enemy, gather intelligence.  
**DEADLINE:** 6-12 hours until discovery  
**STATUS:** Season 7 begins — war in the shadows

---

## 📋 Mission Requirements

### 1. Process Analysis
- ✅ Identify enemy processes (./tracker, surveillance_d)
- ✅ Analyze process tree (fork, exec, orphans)
- ✅ Understand daemon architecture
- ✅ Map network connections to C2 server

### 2. Daemon Creation
- ✅ Implement proper UNIX daemon (double fork, setsid)
- ✅ PID file management (/var/run/moonlight_monitor.pid)
- ✅ Detach from terminal (close stdin/stdout/stderr)
- ✅ Background execution (no controlling TTY)

### 3. Signal Handling
- ✅ SIGTERM handler (graceful shutdown + cleanup)
- ✅ SIGHUP handler (reload configuration)
- ✅ SIGUSR1/SIGUSR2 (verbosity control)
- ✅ SIGCHLD handler (reap zombie processes)
- ✅ Ignore SIGINT, SIGPIPE (robustness)

### 4. Stealth Operations
- ✅ Process name randomization (every 5 min)
- ✅ CPU usage < 0.5% (avoid detection)
- ✅ Mimic system process behavior
- ✅ Network traffic blending

### 5. Intelligence Gathering
- ✅ Capture network packets (324+ packets)
- ✅ Log enemy activity (system_processes.log)
- ✅ Identify C2 server (185.220.101.42:8080)
- ✅ Generate intelligence report

---

## 🧪 Testing Criteria

### Process Management
```bash
# Create daemon
./moonlight_daemon start

# Verify daemon running
ps aux | grep moonlight_monitor

# Send signals
kill -USR1 <PID>   # Increase verbosity
kill -HUP <PID>    # Reload config
kill -TERM <PID>   # Graceful shutdown

# Check no zombies
ps aux | grep defunct
```

### Stealth Verification
```bash
# Check CPU usage (should be < 0.5%)
top -p <PID>

# Verify process name rotation
watch -n 10 'ps aux | grep <PID>'

# Network traffic analysis
tcpdump -i eth0 host 185.220.101.42
```

---

## 📦 Deliverables

### Artifacts (3 files, 785 lines):
- ✅ `system_processes.log` (281 lines)
  - Full process monitoring log (14:00-17:30)
  - Enemy process detection (PID 3012, 3156)
  - Counter-daemon deployment timeline
  - Mexican standoff situation documented
  
- ✅ `signal_traces.txt` (374 lines)
  - Complete signal handling traces (47 signals)
  - SIGTERM, SIGHUP, SIGUSR1/2, SIGCHLD examples
  - Signal race condition handling
  - SIGKILL attack + recovery
  - Best practices & defensive measures
  
- ✅ `daemon_config.conf` (130 lines)
  - Professional daemon configuration
  - Target monitoring settings
  - Stealth mode parameters
  - Logging, alerting, watchdog config

### Code Deliverables:
- ✅ `solution/processes_daemons.c` (440 lines)
- ✅ `starter.c` (105 lines)
- ✅ `solution/Makefile` + root `Makefile`

---

## 📊 Success Metrics

- [x] Enemy processes identified and analyzed
- [x] Counter-daemon created (PID 3789)
- [x] Stealth mode operational (не обнаружен врагом)
- [x] Intelligence gathered (324 packets, C2 identified)
- [x] Signal handling robust (47 signals processed)
- [x] Zombie processes prevented (SIGCHLD handler)
- [x] 6-12 hours gained before discovery

---

## ⚠️ Mission Status

**ACCOMPLISHED** ✅

Counter-surveillance daemon deployed. Enemy monitored but not neutralized.  
Time until discovery: 6-12 hours.

**URGENT:** Proceed to Episode 26 immediately.  
Parallel log analysis required (threads, producer-consumer).

---

**Next Mission:** [Episode 26: Threads & Parallelism →](../episode-26-threads-parallelism/)

---

*MOONLIGHT Protocol: Ghost in the machine. Unseen. Unheard. Unstoppable.* 🔧
