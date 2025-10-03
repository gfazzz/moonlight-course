# Episode 19: "Risk & Portfolio" 💼
## Season 5, Episode 3

> *"In risk management, the unknown unknowns are what kill you."*

---

## 🎬 BRIEFING

```
╔══════════════════════════════════════════════════════════╗
║  ЛОКАЦИЯ: City of London, Лондон, Великобритания        ║
║  ВРЕМЯ:   18:45 (GMT), четверг, 19 декабря              ║
║  СТАТУС:  🔴 CRITICAL PORTFOLIO RISK DETECTED            ║
║  GPS:     51.5074°N, 0.1278°W (City of London)          ║
╚══════════════════════════════════════════════════════════╝
```

**19 декабря, 14:15 EST. Нью-Йорк, финал Episode 18.**

Анализ завершён. V. присылает новое сообщение:

> *"Отличная работа. Но это ещё не всё. Лети в Лондон. Там я покажу тебе настоящий масштаб операции. Билет на British Airways отправлен. Вылет через 2 часа. — V."*

**Рейс BA112:** JFK, New York → LHR, London Heathrow  
**Вылет:** 16:30 EST (21:30 GMT)  
**Время в пути:** 7 часов  
**Прибытие:** 04:30 GMT (20 декабря, утро)

---

**19 декабря, 18:45 GMT. Лондон, City of London.**

Вы приземлились утром, но сразу взялись за работу. Теперь — вечер. За окном — классическая архитектура финансового района, Банк Англии вдали, собор Святого Павла подсвечен.

Временная квартира с видом на Темзу. Телефон взрывается сообщением от V.:

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED - PRIORITY: CRITICAL)  │
│  TIME: 18:42:31                             │
│  ALERT: PORTFOLIO AT RISK                   │
├─────────────────────────────────────────────┤
│                                             │
│  СРОЧНО. Ситуация изменилась.               │
│                                             │
│  Мы взломали их портфельную систему.        │
│  Результаты — катастрофа.                   │
│                                             │
│  ПОРТФЕЛЬ ОРГАНИЗАЦИИ:                      │
│  • Размер: $50,000,000                      │
│  • Активы: 70% crypto, 30% stocks           │
│  • Leverage: 3:1 (левередж!)                │
│  • Корреляция: 0.95 (почти идентичны)       │
│                                             │
│  При таком риске один плохой день может     │
│  обрушить всё. Нужен профессиональный       │
│  risk analysis. СЕЙЧАС.                     │
│                                             │
│  Данные в папке. Дедлайн: 23:30.            │
│  После — рынки закрываются на выходные.     │
│                                             │
│  — V.                                       │
└─────────────────────────────────────────────┘
```

**18:45. Открываете присланную папку.**

```
📁 PORTFOLIO_RISK/
├── portfolio_holdings.csv     [POSITIONS] $50M распределены
├── historical_returns.csv     [RETURNS] 2 года данных
├── correlation_matrix.dat     [BINARY] корреляции активов
├── stress_scenarios.txt       [SCENARIOS] 2008, 2020, Flash Crash
└── warning_v.txt              "VaR может быть > $15M. Проверь."
```

Вы открываете `portfolio_holdings.csv`:

```
ASSET,VALUE,WEIGHT,TYPE
BTC,$17,500,000,35.0%,CRYPTO
ETH,$12,250,000,24.5%,CRYPTO  
SOL,$5,250,000,10.5%,CRYPTO
AAPL,$4,500,000,9.0%,STOCK
TSLA,$4,250,000,8.5%,STOCK
NVDA,$3,125,000,6.25%,STOCK
MSFT,$3,125,000,6.25%,STOCK

TOTAL: $50,000,000
LEVERAGE: 3:1 (реально $150M экспозиция!)
```

**Красные флаги повсюду:**

1. **70% в crypto** — экстремальная волатильность
2. **Корреляция 0.95** — все активы движутся вместе (нет диверсификации!)
3. **Leverage 3:1** — утроенный риск
4. **Tech stocks** — AAPL, TSLA, NVDA тоже коррелируют с crypto

**Быстрый расчет в уме:**

Если BTC упадет на 20% (бывало и хуже):
- Прямой убыток: $17.5M × 20% = $3.5M
- С leverage 3:1: $3.5M × 3 = **$10.5M убытка**
- Остальной crypto тоже упадет (корреляция 0.95)
- **Общий убыток может достичь $15-20M за день**

**V. был прав. Это катастрофа.**

Вы открываете `warning_v.txt`:

> *"Они играют в русскую рулетку с $50M. VaR(95%, 1 day) может быть > $15M. Если рынок развернется — крах. Рассчитай точные риски. Построй эффективную границу. Покажи им, насколько они уязвимы. У нас 4 часа 45 минут."*

**Часы показывают 18:45. До 23:30 — менее 5 часов.**

**Ваша миссия:**
1. Вычислить Value at Risk (VaR) — Historical, Parametric, Monte Carlo
2. Рассчитать Sharpe Ratio и Sortino Ratio
3. Построить correlation matrix (найти истинную диверсификацию)
4. Portfolio optimization по Марковицу (эффективная граница)
5. Stress testing (сценарии 2008, 2020, Flash Crash)

**Это самый важный анализ Season 5. От него зависит $50M.**

**Начинаем.**

---

## 🎯 Цели обучения

### Теория (Шиллер + Modern Portfolio Theory)
- **Value at Risk (VaR):** максимальный убыток с заданной вероятностью
- **Portfolio Optimization:** теория Марковица, эффективная граница
- **Risk Metrics:** Sharpe ratio, Sortino ratio, beta, correlation
- **Monte Carlo Simulation:** моделирование случайных сценариев

### Практика
- Расчет VaR (Historical, Parametric, Monte Carlo)
- Построение эффективной границы портфеля
- Корреляционный анализ активов
- Stress testing и scenario analysis

---

## 📖 Теоретическая часть

### 1. Value at Risk (VaR)

**VaR** — максимальный убыток с вероятностью X% за период Y.

**Пример:** VaR(95%, 1 день) = $10,000  
Означает: с вероятностью 95% убыток не превысит $10K за 1 день.

### ⚡ FinTech Integration: Seasons 1-4 Skills

**Season 5 использует ВСЁ из предыдущих сезонов!**

```
┌──────────────────────────────────────────────────────┐
│  Season 2: Memory Management                         │
│  ├─ Dynamic arrays для portfolio holdings            │
│  ├─ Efficient matrix operations (correlation)        │
│  └─ Monte Carlo: 10,000 simulations = smart malloc   │
├──────────────────────────────────────────────────────┤
│  Season 3: Network Programming                       │
│  ├─ Portfolio API communication (REST/WebSocket)     │
│  ├─ Low-latency data feeds (< 100ms acceptable)      │
│  └─ Multi-exchange price aggregation                 │
├──────────────────────────────────────────────────────┤
│  Season 4: Cryptography & Algorithms                 │
│  ├─ Secure trading keys (AES encryption)             │
│  ├─ Hash tables: O(1) asset lookup                   │
│  ├─ Sorting: VaR quantile calculation (qsort)        │
│  └─ SHA-256: transaction verification                │
└──────────────────────────────────────────────────────┘
```

**Real-world example:**
```c
// Season 3: Secure API connection (simplified)
#include <sys/socket.h>
#include <netinet/in.h>

int connect_to_portfolio_api(const char *host, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };
    
    // Connect to portfolio management API
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        return -1;
    }
    
    return sockfd;
}

// Season 4: Encrypt trading keys before transmission
#include <openssl/aes.h>  // Conceptual

void encrypt_trading_key(const char *key, char *encrypted_out) {
    // AES-256 encryption (Season 4 crypto knowledge!)
    // Protect API keys during network transmission
    // Implementation: use OpenSSL or similar
}

// Season 2: Efficient portfolio matrix operations
double** allocate_correlation_matrix(int n_assets) {
    // Smart memory allocation for NxN matrix
    double **matrix = malloc(n_assets * sizeof(double*));
    for (int i = 0; i < n_assets; i++) {
        matrix[i] = calloc(n_assets, sizeof(double));
    }
    return matrix;
}
```

**Historical VaR:**
```c
double calculate_historical_var(double *returns, int n, double confidence) {
    // Сортируем returns по возрастанию
    qsort(returns, n, sizeof(double), compare_double);
    
    // Находим (1 - confidence) квантиль
    int index = (int)((1.0 - confidence) * n);
    
    return -returns[index];  // Убыток = отрицательный return
}

// Пример: VaR(95%) из 1000 returns
double var_95 = calculate_historical_var(returns, 1000, 0.95);
printf("VaR(95%%): $%.2f\n", var_95 * portfolio_value);
```

**Monte Carlo VaR:**
```c
double monte_carlo_var(double mean, double std_dev, int simulations, double confidence) {
    double *simulated_returns = malloc(simulations * sizeof(double));
    
    for (int i = 0; i < simulations; i++) {
        // Генерируем случайный return из нормального распределения
        simulated_returns[i] = mean + std_dev * random_normal();
    }
    
    double var = calculate_historical_var(simulated_returns, simulations, confidence);
    free(simulated_returns);
    
    return var;
}
```

### ⚡ FinTech Optimization: Parallel Monte Carlo

**Проблема:** 10,000 симуляций = медленно на single thread  
**Решение:** Multi-threading (POSIX threads — можно изучить в Season 7!)

```c
// Conceptual: Parallel Monte Carlo (10,000 paths split across 4 cores)
#include <pthread.h>

typedef struct {
    double mean;
    double std_dev;
    int sim_count;
    double *results;
    int start_idx;
} MonteCarloThread;

void* monte_carlo_worker(void *arg) {
    MonteCarloThread *mt = (MonteCarloThread*)arg;
    
    for (int i = 0; i < mt->sim_count; i++) {
        mt->results[mt->start_idx + i] = 
            mt->mean + mt->std_dev * random_normal();
    }
    
    return NULL;
}

// Run 10K simulations on 4 threads = 4x faster!
double parallel_monte_carlo_var(double mean, double std_dev) {
    const int SIMULATIONS = 10000;
    const int THREADS = 4;
    const int PER_THREAD = SIMULATIONS / THREADS;
    
    double *all_results = malloc(SIMULATIONS * sizeof(double));
    pthread_t threads[THREADS];
    MonteCarloThread args[THREADS];
    
    // Spawn threads
    for (int i = 0; i < THREADS; i++) {
        args[i] = (MonteCarloThread){
            .mean = mean,
            .std_dev = std_dev,
            .sim_count = PER_THREAD,
            .results = all_results,
            .start_idx = i * PER_THREAD
        };
        pthread_create(&threads[i], NULL, monte_carlo_worker, &args[i]);
    }
    
    // Wait for completion
    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    double var = calculate_historical_var(all_results, SIMULATIONS, 0.95);
    free(all_results);
    
    return var;
}
```

**Performance:**
- Single-threaded: ~500ms for 10K simulations
- 4 threads: ~125ms (**4x speedup!**)
- 8 threads (on 8-core CPU): ~65ms (**8x speedup!**)

### 2. Sharpe Ratio

**Sharpe Ratio** = (Return - RiskFreeRate) / Volatility

Показывает доходность с учетом риска.

```c
double calculate_sharpe_ratio(double *returns, int n, double risk_free_rate) {
    double mean_return = 0.0;
    for (int i = 0; i < n; i++) {
        mean_return += returns[i];
    }
    mean_return /= n;
    
    double std_dev = calculate_std_dev(returns, n);
    
    // Annualized Sharpe (если returns дневные)
    double annual_return = mean_return * 252;  // 252 торговых дня
    double annual_vol = std_dev * sqrt(252);
    
    return (annual_return - risk_free_rate) / annual_vol;
}
```

**Интерпретация:**
- Sharpe > 1: хорошо
- Sharpe > 2: отлично  
- Sharpe > 3: исключительно

### 3. Portfolio Optimization (Markowitz)

**Цель:** Найти веса активов для минимального риска при заданной доходности.

```c
typedef struct {
    double *weights;
    int n_assets;
    double expected_return;
    double volatility;
    double sharpe_ratio;
} Portfolio;

// Упрощенная версия (без оптимизации)
Portfolio optimize_portfolio(
    double *expected_returns,
    double **covariance_matrix,
    int n_assets,
    double target_return
) {
    Portfolio port = {0};
    port.n_assets = n_assets;
    port.weights = malloc(n_assets * sizeof(double));
    
    // TODO: Quadratic programming для оптимизации
    // Минимизировать: w^T * Cov * w
    // При ограничениях: sum(w) = 1, w^T * E(R) >= target_return
    
    return port;
}
```

### 4. Correlation Matrix

**Корреляция** показывает, как движутся активы друг относительно друга.

```c
double calculate_correlation(double *asset1, double *asset2, int n) {
    double mean1 = 0.0, mean2 = 0.0;
    for (int i = 0; i < n; i++) {
        mean1 += asset1[i];
        mean2 += asset2[i];
    }
    mean1 /= n;
    mean2 /= n;
    
    double covariance = 0.0, var1 = 0.0, var2 = 0.0;
    for (int i = 0; i < n; i++) {
        double diff1 = asset1[i] - mean1;
        double diff2 = asset2[i] - mean2;
        covariance += diff1 * diff2;
        var1 += diff1 * diff1;
        var2 += diff2 * diff2;
    }
    
    return covariance / sqrt(var1 * var2);
}

// Correlation matrix
void build_correlation_matrix(double **returns, int n_assets, int n_periods, 
                               double **corr_matrix) {
    for (int i = 0; i < n_assets; i++) {
        for (int j = 0; j < n_assets; j++) {
            corr_matrix[i][j] = calculate_correlation(
                returns[i], returns[j], n_periods
            );
        }
    }
}
```

---

## 💻 Задания

### Задание 1: VaR Calculator ⭐⭐⭐⭐☆

Реализуйте VaR тремя методами:

```c
double historical_var(double *returns, int n, double confidence);
double parametric_var(double mean, double std, double confidence);
double monte_carlo_var(double mean, double std, int sims, double conf);
```

### Задание 2: Risk Metrics ⭐⭐⭐☆☆

Рассчитайте метрики риска:

```c
double sharpe_ratio(double *returns, int n, double rf_rate);
double sortino_ratio(double *returns, int n, double rf_rate);
double max_drawdown(double *equity_curve, int n);
```

### Задание 3: Portfolio Optimizer ⭐⭐⭐⭐⭐

Оптимизация портфеля:

```c
Portfolio optimize_min_variance(
    double **covariance_matrix,
    int n_assets
);

Portfolio optimize_max_sharpe(
    double *expected_returns,
    double **covariance_matrix,
    int n_assets,
    double risk_free_rate
);
```

### Задание 4: Stress Testing ⭐⭐⭐⭐⭐

Стресс-тестирование:

```c
double stress_test_2008_crash(Portfolio *port);
double stress_test_covid_crash(Portfolio *port);
void scenario_analysis(Portfolio *port, double **scenarios, int n);
```

---

## 🎬 DEBRIEFING

**23:30. Анализ рисков завершён.**

На экране — критические цифры:

```
╔═══════════════════════════════════════════════════════════════════╗
║  PORTFOLIO RISK ANALYSIS — КРИТИЧЕСКИЕ РЕЗУЛЬТАТЫ                 ║
╠═══════════════════════════════════════════════════════════════════╣
║                                                                   ║
║  📊 Портфель организации:                                         ║
║  • Размер: $50,000,000                                           ║
║  • Активы: 7 позиций (70% crypto, 30% tech stocks)              ║
║  • Leverage: 3:1 ($150M реальная экспозиция)                     ║
║                                                                   ║
║  🚨 VALUE AT RISK (95%, 1 day):                                   ║
║  • Historical VaR: $14.7M                                        ║
║  • Parametric VaR: $16.2M                                        ║
║  • Monte Carlo VaR (10K sims): $15.8M                            ║
║  • Средний VaR: $15.6M (31% портфеля!)                           ║
║                                                                   ║
║  ⚠️ RISK METRICS:                                                 ║
║  • Sharpe Ratio: 0.87 (ниже 1 — плохо)                           ║
║  • Sortino Ratio: 1.12 (marginally acceptable)                   ║
║  • Max Drawdown: -38% (катастрофично)                            ║
║  • Correlation avg: 0.94 (нет диверсификации!)                   ║
║                                                                   ║
║  🔥 STRESS TEST RESULTS:                                          ║
║  • 2008 Crash scenario: -$42M (-84% портфеля!)                   ║
║  • 2020 COVID crash: -$28M (-56%)                                ║
║  • Flash Crash (2010): -$19M (-38%)                              ║
║                                                                   ║
║  ✅ ОПТИМИЗИРОВАННЫЙ ПОРТФЕЛЬ (Markowitz):                        ║
║  • VaR(95%): $4.2M (в 3.7 раза меньше!)                          ║
║  • Sharpe Ratio: 1.95 (отлично)                                  ║
║  • Diversification: 15 активов, correlation avg 0.35             ║
║                                                                   ║
║  💡 РЕКОМЕНДАЦИЯ: Немедленная реструктуризация портфеля          ║
║                                                                   ║
╚═══════════════════════════════════════════════════════════════════╝
```

**Что вы обнаружили:**

### 1. **КРИТИЧЕСКИЙ РИСК** ($15.6M VaR)
- С вероятностью 5% можно потерять **$15.6M за один день**
- Это 31% всего портфеля
- При leverage 3:1 — margin call неизбежен

### 2. **ОТСУТСТВИЕ ДИВЕРСИФИКАЦИИ**
- Correlation 0.94 между активами
- Все движется вместе (crypto + tech stocks)
- Когда BTC падает — падает всё

### 3. **STRESS TEST — КАТАСТРОФА**
- Сценарий 2008: потеря $42M (84%!)
- Portfolio не переживет рыночный крах
- Одна «черная пятница» — и всё пропало

### 4. **ОПТИМИЗИРОВАННОЕ РЕШЕНИЕ**
Вы построили эффективную границу Марковица:
- 15 активов (добавили bonds, commodities, international)
- Correlation снижена до 0.35
- VaR упал с $15.6M до $4.2M
- Sharpe Ratio вырос с 0.87 до 1.95

**23:15. Отправляете финальный отчёт V.**

Тишина. 1 минута. 2 минуты. 5 минут.

Это странно. V. всегда отвечает мгновенно.

**23:22. Наконец:**

```
┌─────────────────────────────────────────────┐
│  FROM: V. (ENCRYPTED - DELAYED 7 MINUTES)   │
│  TIME: 23:27:42                             │
│  STATUS: 🟡 CONNECTION ISSUES               │
│                                             │
│  Извини за задержку. Нас почти засекли.     │
│  Пришлось сменить 3 прокси.                 │
│                                             │
│  Невероятная работа. $15.6M VaR — это       │
│  русская рулетка с $50M. Monte Carlo        │
│  показал 2.7% шанс полного краха.           │
│                                             │
│  Твоя оптимизация снизила риск в 3.7 раза.  │
│  Это спасает их. НО МЫ ИМ НЕ СКАЖЕМ.        │
│                                             │
│  Организация не знает об этом риске.        │
│  Пусть играют вслепую с $150M leverage.     │
│  Один плохой день — и они банкроты.         │
│  Это наше оружие.                           │
│                                             │
│  Episode 20 (финал): crypto след.           │
│  Завтра, 02:30. ПОСЛЕДНИЙ РЫВОК.            │
│                                             │
│  Там они уязвимы. Там мы их закрываем.      │
│                                             │
│  P.S. Спи с одним глазом. После инцидента   │
│       они могут заподозрить утечку.         │
│       Будь осторожен.                       │
│                                             │
│  — V.                                       │
└─────────────────────────────────────────────┘
```

**23:30. Дедлайн выполнен.**

За окном — Лондон ночью. Темза отражает огни небоскрёбов City. Но в голове — только одна мысль:

***"Нас почти засекли. Финал — через 3 часа. Что-то пойдёт не так."***

**Достижение разблокировано:** 🏆 **"Risk Master"** — portfolio risk management освоен

**Cliffhanger:** Кто-то знает о вас. Финал будет опасным.

---

**РЕЗУЛЬТАТ EPISODE 19:**
- ✅ VaR рассчитан тремя методами ($15.6M average)
- ✅ Risk metrics вычислены (Sharpe 0.87, Sortino 1.12)
- ✅ Correlation matrix построена (0.94 — критично!)
- ✅ Portfolio optimization по Марковицу (риск снижен в 3.7 раза)
- ✅ Stress testing проведен (2008: -84%, нужна защита)
- ✅ $50M портфель спасён от краха

**Следующий шаг:** [Episode 20: Crypto & DeFi — ФИНАЛ →](../episode-20-crypto-defi/)

---

## 🥚 Easter Eggs Episode 19

<details>
<summary>🔍 Отсылки и секреты (спойлеры!)</summary>

### Финансовая теория:
- **Harry Markowitz** — Nobel Prize 1990, Modern Portfolio Theory (1952)
- **William Sharpe** — Nobel Prize 1990, Sharpe Ratio (1966)
- **VaR 95%** — Basel Committee стандарт для банков (1996)
- **Efficient Frontier** — визуализация risk/return trade-off
- **Leverage 3:1** — типичный margin для crypto exchanges

### Исторические кризисы:
- **2008 Crash** — Lehman Brothers, -37% S&P 500
- **2020 COVID** — March 2020, -34% за месяц
- **Flash Crash 2010** — May 6, Dow -9% за минуты
- **Black Monday 1987** — October 19, -22.6% за день
- **LTCM 1998** — hedge fund collapse, leverage disaster

### Числа риска:
- **$15.6M VaR** — 31% портфеля, критически высоко (норма <5%)
- **Correlation 0.94** — почти полная корреляция (диверсификация = 0)
- **Sharpe 0.87** — ниже 1 считается плохим
- **Max DD -38%** — сравним с 2008 (-37%), неприемлемо
- **Stress test -84%** — полная потеря при кризисе

### Monte Carlo:
- **10,000 симуляций** — стандарт для VaR расчетов
- **Random walk** — Brownian motion для моделирования цен
- **Normal distribution** — предположение (на деле fat tails)
- **Manhattan Project 1940s** — первое применение Monte Carlo

### Отсылки к курсу:
- **Sorting returns** — использует quicksort из Season 4
- **Correlation calculation** — статистика из S4E15
- **Matrix operations** — линейная алгебра basics
- **Random number generation** — stdlib rand() + Box-Muller

**Философия Episode 19**: *"Risk is what's left when you think you've thought of everything."* — Carl Richards

</details>

---

## 🏆 Бонусные задания

### Bonus 1: Network API Integration ⭐⭐⭐⭐⭐ (Season 3 skills!)
Подключитесь к mock portfolio API через sockets (REST simulation):
```c
// Season 3: TCP connection + HTTP request
int api_fd = connect_to_portfolio_api("localhost", 8080);
send(api_fd, "GET /portfolio/holdings HTTP/1.1\r\n\r\n", ...);
// Parse JSON response with holdings data
```

### Bonus 2: Encrypted Data Transmission ⭐⭐⭐⭐⭐ (Season 4 crypto!)
Шифруйте portfolio data перед отправкой по сети (AES-256).

### Bonus 3: Hash Table Asset Lookup ⭐⭐⭐⭐☆ (Season 4!)
Реализуйте O(1) поиск активов по символу (hash table from S4).

### Bonus 4: Parallel Monte Carlo ⭐⭐⭐⭐⭐ (Performance!)
Multi-threading для 10K simulations (4 threads = 4x faster).

### Bonus 5: Efficient Frontier ⭐⭐⭐⭐⭐
Визуализация эффективной границы портфеля (ASCII).

### Bonus 6: CVaR (Conditional VaR) ⭐⭐⭐⭐⭐
Expected Shortfall — средний убыток при превышении VaR.

---

## 📊 Что вы узнали

### 📚 Финансовые концепции
- ✅ Расчет Value at Risk (VaR) — Historical, Parametric, Monte Carlo
- ✅ Portfolio optimization (Markowitz)
- ✅ Risk metrics (Sharpe, Sortino, max drawdown)
- ✅ Correlation analysis
- ✅ Stress testing (2008, 2020, Flash Crash)

### ⚡ FinTech навыки + интеграция сезонов 1-4
- ✅ **Season 2 Memory:** Dynamic arrays, efficient matrix allocation
- ✅ **Season 3 Network:** Portfolio API communication (sockets, REST)
- ✅ **Season 4 Crypto:** AES encryption для trading keys
- ✅ **Season 4 Algorithms:** Hash tables (O(1) lookup), qsort (VaR quantiles)
- ✅ **Multi-threading:** Parallel Monte Carlo (4x+ speedup)
- ✅ **Box-Muller transform:** Proper random number generation

**Результат:** Production-grade risk management system, интегрирующий **все навыки из сезонов 1-4**!

---

## 🔜 Следующий эпизод

В **Episode 20: Crypto Trading & DeFi** вы научитесь:
- Анализировать криптовалютные рынки
- Работать с DeFi протоколами (концептуально)
- Находить арбитражные возможности
- Детектировать манипуляции на crypto exchanges

Финал Season 5 — раскрытие криптовалютной схемы организации на $10M+!

---

## 🔗 Навигация

- [← Episode 18: Trading Algorithms](../episode-18-trading-algorithms/)
- [↑ Season 5 Overview](../README.md)
- [→ Episode 20: Crypto & DeFi — ФИНАЛ](../episode-20-crypto-defi/)
- [📋 Общий план курса](../../CURRICULUM.md)
- [🏠 Главная страница](../../README.md)

---

<div align="center">

**🌙 MOONLIGHT OPERATION**  
*Episode 19 of 42 • Season 5, Episode 3*

**"It's not about being right. It's about surviving being wrong."** — Nassim Taleb

```
// EPISODE 19: RISK & PORTFOLIO
// STATUS: Enhanced (20% → 80%)
// DURATION: 4-5 hours
// ARTIFACTS: portfolio_manager
// FINDINGS: $15.6M VaR detected, portfolio saved
```

[← Episode 18](../episode-18-trading-algorithms/) | [Season 5](../README.md) | [Episode 20 →](../episode-20-crypto-defi/)

---

**Episode 19 Complete!** ✅  
VaR Calculation → Sharpe Ratio → Markowitz Optimization → Stress Testing

**Next:** Episode 20 — ФИНАЛ! Crypto scheme раскрытие ($10M+ evidence)

</div>
