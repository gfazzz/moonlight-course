/*
 * OPERATION MOONLIGHT — s06e06 "Датчики: кольцевой буфер и автомат"
 * Эталонное решение: sensors.c
 *
 * Концепт серии: на контроллере память — крохи, а датчик сыплет поток измерений.
 *                Держим ПОСЛЕДНИЕ K отсчётов в кольцевом буфере фиксированного размера
 *                (без malloc), сглаживаем шум скользящим средним и гоняем конечный
 *                автомат NORMAL -> WARNING -> ALARM с ГИСТЕРЕЗИСОМ (разные пороги
 *                вверх и вниз), чтобы одиночный выброс не поднимал ложную тревогу.
 * Задача: обработать поток датчика температуры и вести состояние тревоги.
 */
#include <stdio.h>

#define K 4      /* окно сглаживания */

/* Кольцевой буфер фиксированного размера: никакой динамической памяти. */
static double buf[K];
static int head = 0, count = 0;
static double sum = 0.0;

static double push(double x) {
    if (count == K) sum -= buf[head];   /* вытесняем самый старый отсчёт */
    else count++;
    buf[head] = x;
    sum += x;
    head = (head + 1) % K;
    return sum / count;                 /* скользящее среднее за O(1) */
}

enum { NORMAL, WARNING, ALARM };
static const char *name[] = {"NORMAL", "WARNING", "ALARM"};

/* Гистерезис: подниматься по одним порогам, опускаться — по другим (ниже). */
static int step_fsm(int s, double avg) {
    switch (s) {
        case NORMAL:
            if (avg >= 60.0) s = WARNING;
            break;
        case WARNING:
            if (avg >= 80.0) s = ALARM;
            else if (avg <= 50.0) s = NORMAL;
            break;
        case ALARM:
            if (avg <= 70.0) s = WARNING;
            break;
    }
    return s;
}

int main(void) {
    /* Поток датчика (°C). На t=4 одиночный выброс 90 — сглаживание его гасит,
       ложной тревоги нет. Затем реальный устойчивый рост до ALARM и спад с гистерезисом. */
    double stream[] = {30,32,31,33,90,34,32,35,64,70,78,86,92,88,60,45,33,30};
    int n = (int)(sizeof(stream) / sizeof(stream[0]));

    printf("=== sensor stream: ring buffer + FSM (hysteresis) ===\n");
    printf("window K=%d, WARN>=60 ALARM>=80, back: <=70 / <=50\n\n", K);

    int state = NORMAL;
    for (int i = 0; i < n; i++) {
        double avg = push(stream[i]);
        int next = step_fsm(state, avg);
        printf("t=%2d raw=%5.1f avg=%5.1f  %-7s", i, stream[i], avg, name[next]);
        if (next != state) printf("  <-- %s", next > state ? "ESCALATE" : "recover");
        putchar('\n');
        state = next;
    }

    printf("\nfinal state: %s\n", name[state]);
    printf("note: the 90.0 spike at t=4 was absorbed — no false alarm.\n");
    return 0;
}
