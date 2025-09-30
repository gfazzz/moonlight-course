# Episode 38: "System Architecture" 🏗️
## Season 10: FINAL MISSION | Episode 38/42

> *"Build the system. Connect everything."*

---

## 📋 Briefing

**ФИНАЛЬНАЯ МИССИЯ НАЧИНАЕТСЯ.**

Объединить ВСЁ в единую архитектуру MOONLIGHT.

**Задачи:**
1. Microservices design
2. Message bus (ZeroMQ)
3. Service discovery
4. API Gateway

---

## 📚 Теория

### ZeroMQ Message Bus

```c
#include <zmq.h>

// Publisher
void *context = zmq_ctx_new();
void *publisher = zmq_socket(context, ZMQ_PUB);
zmq_bind(publisher, "tcp://*:5555");

char msg[256];
snprintf(msg, sizeof(msg), "moonlight.event: %s", data);
zmq_send(publisher, msg, strlen(msg), 0);
```

---

## 🛠 Практика

**Задачи:**
1. Service mesh
2. Load balancer
3. Config management
4. Health monitoring

---

**Next:** [Episode 39: Intelligence Dashboard →](../episode-39-dashboard/)
