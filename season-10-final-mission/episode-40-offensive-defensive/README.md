# Episode 40: "Offensive & Defensive Operations" ⚔️
## Season 10: FINAL MISSION | Episode 40/42

> *"Attack. Defend. Survive."*

---

## 📋 Briefing

Полный pentesting и защита: IDS/IPS, honeypots, exploit mitigation.

**Задачи:**
1. Vulnerability scanner
2. IDS/IPS system
3. Honeypot deployment
4. Exploit detection

---

## 📚 Теория

### Simple IDS

```c
typedef struct {
    char signature[256];
    int severity;
} ThreatSignature;

bool detect_threat(const char *packet, ThreatSignature *sigs, int n) {
    for (int i = 0; i < n; i++) {
        if (strstr(packet, sigs[i].signature)) {
            printf("THREAT DETECTED: %s (Severity: %d)\n", 
                   sigs[i].signature, sigs[i].severity);
            return true;
        }
    }
    return false;
}
```

---

## 🛠 Практика

**Задачи:**
1. Port scanner
2. Packet sniffer + IDS
3. Buffer overflow detector
4. Automated defense

---

**Next:** [Episode 41: Performance Optimization →](../episode-41-performance/)
