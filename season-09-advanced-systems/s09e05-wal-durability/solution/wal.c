/*
 * OPERATION MOONLIGHT — s09e05 "Журнал и надёжность (WAL)"
 * Эталонное решение: wal.c
 *
 * Концепт серии: индекс (s09e04) быстрый, но беззащитный. Если питание пропадёт
 *                посреди записи, часть страниц окажется новой, часть — старой,
 *                и база превратится в мусор. Решение — Write-Ahead Logging:
 *                СНАЧАЛА записать намерение в журнал и сбросить его на диск,
 *                ТОЛЬКО ПОТОМ менять сами данные. Тогда после сбоя журнал
 *                позволяет доиграть незавершённое (redo) или откатить (undo).
 *                Транзакция считается зафиксированной, когда в журнале есть
 *                запись COMMIT — а не когда данные легли в файл.
 * Задача: смоделировать сбой на каждом шаге и показать, что данные не бьются.
 *
 * Симулируем и диск, и сбой: «питание» пропадает по счётчику операций.
 */
#include <stdio.h>
#include <string.h>

#define NACC   4        /* счета */
#define LOGCAP 64

/* ---------- симуляция диска ---------- */
typedef struct {
    long acc[NACC];             /* «страницы данных» */
    char log[LOGCAP][64];       /* журнал: последовательные записи */
    int  nlog;
    int  log_synced;            /* сколько записей журнала реально дошло до диска */
} Disk;

static Disk disk;

/* Счётчик до сбоя: 0 = сбой прямо сейчас, -1 = сбоя не будет. */
static int crash_in = -1;
static int crashed  = 0;

static int tick(void) {                       /* каждая операция ввода-вывода «тикает» */
    if (crashed) return 1;
    if (crash_in < 0) return 0;
    if (crash_in == 0) { crashed = 1; return 1; }
    crash_in--;
    return 0;
}

/* Дописать запись в журнал (ещё не на диске). */
static void log_append(const char *rec) {
    if (crashed || disk.nlog >= LOGCAP) return;
    snprintf(disk.log[disk.nlog], sizeof disk.log[0], "%s", rec);
    disk.nlog++;
}

/* fsync журнала: только после него запись считается устойчивой. */
static void log_sync(void) {
    if (tick()) return;                       /* сбой мог случиться прямо здесь */
    disk.log_synced = disk.nlog;
}

/* Запись страницы данных — может оборваться на середине. */
static void data_write(int idx, long value) {
    if (tick()) return;
    disk.acc[idx] = value;
}

/* ---------- транзакция: перевод средств ---------- */
static void transfer(int from, int to, long amount) {
    char rec[64];

    snprintf(rec, sizeof rec, "BEGIN");
    log_append(rec);

    /* WAL: сначала намерения (старое и новое значение) — для undo и redo. */
    snprintf(rec, sizeof rec, "SET acc%d %ld -> %ld", from, disk.acc[from], disk.acc[from] - amount);
    log_append(rec);
    snprintf(rec, sizeof rec, "SET acc%d %ld -> %ld", to, disk.acc[to], disk.acc[to] + amount);
    log_append(rec);

    log_sync();                                /* журнал на диске ДО данных */

    log_append("COMMIT");                      /* фиксация — в журнале */
    log_sync();

    /* Страницы данных пишутся ПОСЛЕ фиксации и лениво (политика no-force).
       Если сбой случится здесь — транзакция уже зафиксирована, и восстановление
       обязано ДОИГРАТЬ её по журналу (redo). */
    data_write(from, disk.acc[from] - amount);
    data_write(to,   disk.acc[to]   + amount);
}

/* ---------- восстановление ---------- */
/* Транзакция зафиксирована, только если COMMIT дошёл до диска (в пределах log_synced). */
static int recover(void) {
    int committed = 0;
    for (int i = 0; i < disk.log_synced; i++)
        if (strcmp(disk.log[i], "COMMIT") == 0) committed = 1;

    if (committed) {
        /* redo: доигрываем записи, которые могли не дойти до страниц данных. */
        for (int i = 0; i < disk.log_synced; i++) {
            int idx; long oldv, newv;
            if (sscanf(disk.log[i], "SET acc%d %ld -> %ld", &idx, &oldv, &newv) == 3)
                if (idx >= 0 && idx < NACC) disk.acc[idx] = newv;
        }
        return 1;
    }

    /* undo: откатываем частично применённые изменения к старым значениям. */
    for (int i = disk.log_synced - 1; i >= 0; i--) {
        int idx; long oldv, newv;
        if (sscanf(disk.log[i], "SET acc%d %ld -> %ld", &idx, &oldv, &newv) == 3)
            if (idx >= 0 && idx < NACC) disk.acc[idx] = oldv;
    }
    return 0;
}

static long total(void) {
    long t = 0;
    for (int i = 0; i < NACC; i++) t += disk.acc[i];
    return t;
}

static void scenario(const char *title, int crash_after) {
    /* Исходное состояние: сумма всегда 400. */
    memset(&disk, 0, sizeof disk);
    for (int i = 0; i < NACC; i++) disk.acc[i] = 100;
    crash_in = crash_after;
    crashed = 0;

    long before = total();
    transfer(0, 1, 30);

    int was_crashed = crashed;
    crashed = 0;                                /* «перезагрузились» */
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
