# Episode 16: Financial Data Structures 📊

> "Risk comes from not knowing what you're doing." — Warren Buffett

## 📋 Описание

После анализа торговых алгоритмов организации в Episode 15, необходимо создать систему мониторинга финансовых рисков в реальном времени. Используя продвинутые структуры данных, вы построите risk management engine.

**В этом эпизоде вы научитесь:**
- Реализовывать linked lists для tick data
- Создавать stacks для transaction history
- Использовать queues для order processing
- Вычислять финансовые метрики (VaR, Sharpe ratio)

---

## 🎯 Цели обучения

### Теория (Шиллер + Quantitative Finance)
- **Risk Management:** VaR, stress testing
- **Portfolio Theory:** diversification, Sharpe ratio
- **Data Structures:** lists, stacks, queues, trees
- **Real-time processing:** streaming data analysis

### Практика
- Linked list для market data streaming
- Stack для undo/redo transactions
- Queue для order processing pipeline
- Risk metrics calculation

---

## 📖 Теоретическая часть

### 1. Linked List для Tick Data

```c
typedef struct TickNode {
    double price;
    int volume;
    time_t timestamp;
    struct TickNode *next;
} TickNode;

// Добавление в конец (O(1) with tail pointer)
void append_tick(TickNode **head, TickNode **tail, double price, int volume);
```

### 2. Risk Metrics

**VaR (Value at Risk)** — максимальный убыток с заданной вероятностью:
```c
double calculate_var(double *returns, int n, double confidence);
```

**Sharpe Ratio** — доходность с учетом риска:
```
Sharpe = (Return - RiskFreeRate) / Volatility
```

---

## 💻 Задания

### Задание 1: Tick Data Stream ⭐⭐⭐☆☆
Linked list для хранения потока цен.

### Задание 2: Transaction Stack ⭐⭐⭐☆☆
Stack для истории транзакций с undo.

### Задание 3: Risk Calculator ⭐⭐⭐⭐⭐
Вычислить VaR и Sharpe ratio портфеля MOONLIGHT.

---

## 🎬 Сюжет

> **V.:** "Портфель операции MOONLIGHT под риском. Рассчитай VaR. Если превышает $500K — экстренная ликвидация."

---

<div align="center">

**MOONLIGHT OPERATION**  
*Episode 16 of 32*

[← Episode 15](../episode-15-trading-algorithms/) | [Season 4](../README.md) | [Season 5 →](../../season-5-financial-markets/)

</div>
