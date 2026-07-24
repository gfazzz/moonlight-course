/*
 * OPERATION MOONLIGHT — s06e06 "Датчики: кольцевой буфер и автомат"
 * Каркас. Скопируй в artifacts/sensors.c и закрой TODO.
 *
 *   cp starter.c artifacts/sensors.c
 *   make test
 *
 * Концепт: последние K отсчётов в кольцевом буфере (без malloc) -> скользящее среднее;
 *   автомат NORMAL->WARNING->ALARM с гистерезисом (пороги вверх и вниз разные).
 */
#include <stdio.h>

#define K 4

static double buf[K];
static int head = 0, count = 0;
static double sum = 0.0;

/* TODO 1: push(x) — кольцевой буфер + скользящее среднее за O(1):
     если count==K: sum -= buf[head]; иначе count++;
     buf[head]=x; sum+=x; head=(head+1)%K; вернуть sum/count. */
static double push(double x);

enum { NORMAL, WARNING, ALARM };
static const char *name[] = {"NORMAL", "WARNING", "ALARM"};

/* TODO 2: step_fsm(s, avg) с гистерезисом:
     NORMAL:  avg>=60 -> WARNING
     WARNING: avg>=80 -> ALARM; иначе avg<=50 -> NORMAL
     ALARM:   avg<=70 -> WARNING
   вернуть новое состояние. */
static int step_fsm(int s, double avg);

int main(void) {
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

static double push(double x) { (void)x; return 0.0; /* TODO 1 */ }
static int step_fsm(int s, double avg) { (void)avg; return s; /* TODO 2 */ }
