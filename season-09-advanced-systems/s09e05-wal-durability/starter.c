/*
 * OPERATION MOONLIGHT — s09e05 "Журнал и надёжность (WAL)"
 * Каркас. Скопируй в artifacts/wal.c и реализуй.
 *
 *   cp starter.c artifacts/wal.c
 *   make test
 *
 * Требования — в mission.md, формат вывода — в tests/expected.txt.
 *
 * Порядок операций в транзакции (это и есть суть WAL):
 *   1) BEGIN и записи «SET accN old -> new» в журнал (в памяти);
 *   2) log_sync()        <- журнал на диске ДО данных;
 *   3) COMMIT в журнал + log_sync();
 *   4) только теперь пишем страницы данных (политика no-force).
 *
 * Восстановление:
 *   COMMIT есть в СИНХРОНИЗИРОВАННОЙ части журнала -> redo (доиграть new);
 *   COMMIT нет -> undo (вернуть old, идя от конца журнала к началу).
 */
#include <stdio.h>
#include <string.h>

#define NACC   4
#define LOGCAP 64

typedef struct {
    long acc[NACC];
    char log[LOGCAP][64];
    int  nlog;
    int  log_synced;      /* сколько записей журнала РЕАЛЬНО на диске */
} Disk;

static Disk disk;
static int crash_in = -1;     /* 0 = сбой на ближайшей операции, -1 = без сбоя */
static int crashed  = 0;

/* tick() дан готовым: каждая операция ввода-вывода спрашивает у него, жив ли диск. */
static int tick(void) {
    if (crashed) return 1;
    if (crash_in < 0) return 0;
    if (crash_in == 0) { crashed = 1; return 1; }
    crash_in--;
    return 0;
}

/* Реализуй: */
static void log_append(const char *rec);      /* дописать в журнал (ещё не на диске) */
static void log_sync(void);                    /* fsync: tick(), затем log_synced = nlog */
static void data_write(int idx, long value);   /* tick(), затем запись страницы */
static void transfer(int from, int to, long amount);
static int  recover(void);                     /* 1 = redo (зафиксирована), 0 = undo */

static long total(void) {
    long t = 0;
    for (int i = 0; i < NACC; i++) t += disk.acc[i];
    return t;
}

static void scenario(const char *title, int crash_after) {
    memset(&disk, 0, sizeof disk);
    for (int i = 0; i < NACC; i++) disk.acc[i] = 100;
    crash_in = crash_after;
    crashed = 0;

    long before = total();
    transfer(0, 1, 30);

    int was_crashed = crashed;
    crashed = 0;                 /* «перезагрузились» */
    int committed = recover();

    printf("  %s\n", title);
    printf("    сбой: %s | восстановление: %s | acc0=%ld acc1=%ld | сумма=%ld -> %s\n\n",
           was_crashed ? "да" : "нет",
           committed ? "redo (транзакция зафиксирована)" : "undo (откат)",
           disk.acc[0], disk.acc[1], total(),
           total() == before ? "инвариант сохранён" : "ПОТЕРЯ ДЕНЕГ");
}

int main(void) {
    printf("=== WAL: журнал впереди данных ===\n");
    printf("4 счёта по 100, перевод 30 со счёта 0 на счёт 1; инвариант — сумма 400\n\n");

    scenario("без сбоя", -1);
    scenario("сбой при fsync журнала (до COMMIT)", 0);
    scenario("сбой при fsync COMMIT", 1);
    scenario("сбой после COMMIT, до записи страниц", 2);
    scenario("сбой между записями страниц", 3);

    printf("\nправило: транзакция зафиксирована тогда, когда COMMIT есть В ЖУРНАЛЕ,\n");
    printf("а не когда данные легли в файл. Всё остальное доигрывается или откатывается.\n");
    return 0;
}

/* --- Твоя реализация --- */
static void log_append(const char *rec) { (void)rec; }
static void log_sync(void) { }
static void data_write(int idx, long value) { (void)idx; (void)value; }
static void transfer(int from, int to, long amount) { (void)from; (void)to; (void)amount; }
static int  recover(void) { return 0; }
