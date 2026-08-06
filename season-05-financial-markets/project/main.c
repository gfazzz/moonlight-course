/*
 * OPERATION MOONLIGHT — SEASON PROJECT: trading_system
 *
 * Конвейер обработки журнала сделок, собранный из модулей двух сезонов.
 * Ни одна функция здесь не реализована заново: всё, что делает программа,
 * приходит из библиотек, написанных в сериях.
 *
 *   ml_hash  (Season 4)  индекс контрагентов: поиск за константу
 *   ml_csv   (s05e03)    разбор выгрузки конечным автоматом
 *   ml_money (s05e02)    суммы целыми, комиссия с округлением
 *   ml_time  (s05e04)    метки местного времени биржи в моменты
 *   ml_text  (s05e08)    ширина колонок и поиск двойников
 *   ml_tick  (s05e06)    выгрузка в бинарный формат
 *
 * Задача конвейера: прочитать журнал, свести суммы, разобрать метки,
 * найти в списке контрагентов пару, неразличимую на экране, и выгрузить
 * результат в двоичном виде.
 *
 * Детерминизм: обращений к системным часам нет, все величины считаются
 * по данным. Собирается без -lm.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ml_hash.h"     /* Season 4 */
#include "ml_csv.h"
#include "ml_money.h"
#include "ml_time.h"
#include "ml_text.h"
#include "ml_tick.h"

#define MAX_PARTIES 32
#define FEE_PPM   1500          /* комиссия 0.15% */

/* ---------- журнал: строки ровно в том виде, в каком приходит выгрузка ---------- */

static const char *JOURNAL =
    "trade_id,ts_local,counterparty,amount,qty\n"
    "100001,2024-06-14 14:30:00,\"Meridian Capital, LLC\",1240.00,1200\r\n"
    "100002,2024-06-14 15:05:00,\"Nordwall Trading\",2875.50,300\n"
    "100003,2024-11-03 01:30:00,\"Meridian Capital, LLC\",990.25,5000\n"
    "100004,2024-03-10 02:30:00,\"\xD0\x9C" "eridian Capital, LLC\",20030.00,800\n"
    "100005,2024-06-14 16:40:00,\"Sokolov, Ivanov & Partners\",734.80,150\n"
    "100006,2024-12-19 11:20:00,\"\xD0\x9C" "eridian Capital, LLC\",3305.15,400\n"
    "100007,2024-06-14 17:55:00,\"Nordwall Trading\",6120.40,50\n"
    "100008,2024-11-03 01:50:00,\"Meridian Capital, LLC\",812.60,700\n";

/* ---------- накопитель по контрагентам ---------- */

typedef struct {
    char        name[96];
    ml_money_t  turnover;
    ml_money_t  fee;
    int         trades;
} party_t;

typedef struct {
    party_t party[MAX_PARTIES];
    int     nparty;

    int         rows, broken;
    int         ambiguous, nonexistent;
    ml_money_t  total_turnover, total_fee;
    ml_tick     ticks[MAX_PARTIES * 4];
    int         nticks;
} state_t;

/* Индекс по названию: хеш из Season 4 вместо посимвольного сравнения
   с каждым известным контрагентом на каждой строке журнала. */
static int party_index(state_t *st, const char *name) {
    unsigned long h = ml_hash_djb2(name);
    for (int i = 0; i < st->nparty; i++)
        if (ml_hash_djb2(st->party[i].name) == h &&
            strcmp(st->party[i].name, name) == 0)
            return i;

    if (st->nparty >= MAX_PARTIES) return -1;
    int k = st->nparty++;
    snprintf(st->party[k].name, sizeof st->party[k].name, "%s", name);
    st->party[k].turnover = 0;
    st->party[k].fee      = 0;
    st->party[k].trades   = 0;
    return k;
}

/* Метка вида "2024-11-03 01:30:00" -> гражданское время. Разбор ручной:
   структура важнее удобства, и "2024-13-45" должно быть отвергнуто. */
static int parse_stamp(const char *s, ml_civil *c) {
    int v[6] = { 0 };
    int k = 0;
    for (const char *p = s; *p && k < 6; ) {
        if (*p < '0' || *p > '9') { p++; continue; }
        int n = 0;
        while (*p >= '0' && *p <= '9') { n = n * 10 + (*p - '0'); p++; }
        v[k++] = n;
    }
    if (k != 6) return 0;
    c->y = v[0]; c->mo = v[1]; c->d = v[2];
    c->h = v[3]; c->mi = v[4]; c->s = v[5];
    return (c->mo >= 1 && c->mo <= 12 && c->d >= 1 && c->d <= 31
            && c->h < 24 && c->mi < 60 && c->s < 60);
}

/* Обработчик записи: вызывается разборщиком CSV на каждой строке. */
static int on_row(const ml_csv_row *row, void *user) {
    state_t *st = (state_t *)user;

    if (row->status != ML_CSV_OK || row->nfield != 5) { st->broken++; return 1; }
    if (strcmp(row->field[0], "trade_id") == 0) return 1;      /* шапка */

    ml_civil c;
    if (!parse_stamp(row->field[1], &c)) { st->broken++; return 1; }

    ml_time_t early = 0, late = 0;
    int kind = ml_utc_from_local(&ML_TZ_NEW_YORK, ml_secs_from_civil(c), &early, &late);
    if (kind == ML_LT_AMBIGUOUS)        st->ambiguous++;
    else if (kind == ML_LT_NONEXISTENT) st->nonexistent++;

    ml_money_t amount;
    if (!ml_money_parse(row->field[3], 100, &amount)) { st->broken++; return 1; }

    /* Комиссия банковским округлением: усечение дало бы односторонний
       сдвиг, и итог перестал бы сходиться с выпиской (s05e02). */
    ml_money_t fee = ml_money_mul_ppm(amount, FEE_PPM, ML_ROUND_HALF_EVEN);

    int k = party_index(st, row->field[2]);
    if (k < 0) { st->broken++; return 1; }

    ml_money_add(st->party[k].turnover, amount, &st->party[k].turnover);
    ml_money_add(st->party[k].fee,      fee,    &st->party[k].fee);
    st->party[k].trades++;

    ml_money_add(st->total_turnover, amount, &st->total_turnover);
    ml_money_add(st->total_fee,      fee,    &st->total_fee);
    st->rows++;

    /* Момент для выгрузки: у неоднозначной метки берём более ранний,
       но помечаем это флагом, а не выбираем молча. */
    if (st->nticks < (int)(sizeof st->ticks / sizeof st->ticks[0])) {
        ml_tick *t = &st->ticks[st->nticks++];
        t->ts        = (uint32_t)(kind == ML_LT_NONEXISTENT ? 0 : early % 86400);
        t->price     = (int32_t)amount;
        t->qty       = (uint32_t)atoi(row->field[4]);
        t->symbol_id = (uint16_t)(k + 1);
        t->flags     = (uint8_t)kind;
        t->route_id  = (uint8_t)(ml_suspicious_scripts(row->field[2]) ? 7 : 0);
    }
    return 1;
}

int main(void) {
    static state_t st;
    char buf[32];

    printf("=== trading_system: конвейер журнала сделок ===\n");
    printf("модули: ml_hash (S4) + ml_csv + ml_money + ml_time + ml_text + ml_tick\n\n");

    /* ---------- 1. Разбор ---------- */
    FILE *f = fmemopen((void *)JOURNAL, strlen(JOURNAL), "rb");
    if (!f) { printf("FAIL: не открыть журнал\n"); return 1; }
    long seen = ml_csv_parse(f, ',', on_row, &st);
    fclose(f);

    printf("--- разбор ---\n");
    printf("записей в выгрузке:    %ld\n", seen);
    printf("сделок принято:        %d\n", st.rows);
    printf("записей отброшено:     %d\n", st.broken);

    /* ---------- 2. Время ---------- */
    printf("\n--- метки местного времени ---\n");
    printf("в удвоенном часу:      %d\n", st.ambiguous);
    printf("в несуществующем часу: %d\n", st.nonexistent);
    printf("метки, которых биржа не выдаёт: %s\n", st.nonexistent ? "есть" : "нет");

    /* ---------- 3. Деньги ---------- */
    printf("\n--- обороты по контрагентам ---\n");
    printf("контрагент                  сделок       оборот     комиссия\n");
    for (int i = 0; i < st.nparty; i++) {
        ml_pad_cols(st.party[i].name, 28);
        printf("%6d", st.party[i].trades);
        ml_money_str(st.party[i].turnover, 100, buf, sizeof buf);
        printf(" %12s", buf);
        ml_money_str(st.party[i].fee, 100, buf, sizeof buf);
        printf(" %12s\n", buf);
    }

    ml_money_str(st.total_turnover, 100, buf, sizeof buf);
    printf("\nоборот всего:   %s\n", buf);
    ml_money_str(st.total_fee, 100, buf, sizeof buf);
    printf("комиссия всего: %s\n", buf);

    /* Сверка: сумма по контрагентам обязана совпасть с общей до цента. */
    ml_money_t check = 0;
    for (int i = 0; i < st.nparty; i++)
        ml_money_add(check, st.party[i].turnover, &check);
    printf("сверка сходится: %s\n", check == st.total_turnover ? "да" : "нет");

    /* ---------- 4. Двойники ---------- */
    printf("\n--- проверка названий ---\n");
    int pairs = 0, suspicious = 0;
    for (int i = 0; i < st.nparty; i++)
        if (ml_suspicious_scripts(st.party[i].name)) suspicious++;

    for (int i = 0; i < st.nparty; i++)
        for (int j = i + 1; j < st.nparty; j++)
            if (ml_looks_same(st.party[i].name, st.party[j].name)) {
                printf("выглядят одинаково, но это разные записи:\n");
                printf("  [%d] %s\n", i, st.party[i].name);
                printf("  [%d] %s\n", j, st.party[j].name);
                pairs++;
            }

    printf("названий со смешанным алфавитом: %d\n", suspicious);
    printf("пар-двойников: %d\n", pairs);

    /* ---------- 5. Выгрузка ---------- */
    static uint8_t out[ML_TICK_HDR_BYTES + 128 * ML_TICK_WIRE_V2];
    size_t k = ml_tick_write_header(out, 2, (uint32_t)st.nticks);
    for (int i = 0; i < st.nticks; i++) k += ml_tick_write(out + k, &st.ticks[i], 2);

    ml_tick_header h;
    ml_tick_read_header(out, &h);

    printf("\n--- выгрузка в бинарный вид ---\n");
    printf("версия формата:  %u\n", h.version);
    printf("байт на запись:  %u\n", h.wire_bytes);
    printf("записей:         %u\n", h.count);
    printf("размер выгрузки: %zu байт\n", k);
    printf("тот же журнал текстом: %zu байт\n", strlen(JOURNAL));

    /* Чтение обратно: размер записи берётся из заголовка, а не из sizeof. */
    int restored = 0;
    size_t off = ML_TICK_HDR_BYTES;
    for (uint32_t i = 0; i < h.count; i++) {
        ml_tick t;
        off += ml_tick_read(out + off, &t, h.version);
        if (ml_tick_equal(&t, &st.ticks[i])) restored++;
    }
    printf("прочитано обратно без потерь: %s\n", restored == st.nticks ? "да" : "нет");

    int routed = 0;
    for (int i = 0; i < st.nticks; i++) if (st.ticks[i].route_id == 7) routed++;
    printf("сделок с пометкой маршрута:   %d из %d\n", routed, st.nticks);

    /* ---------- 6. Самопроверка ---------- */
    printf("\n--- самопроверка конвейера ---\n");
    printf("суммы сходятся до цента:      %s\n", check == st.total_turnover ? "да" : "нет");
    printf("двойник найден:               %s\n", pairs == 1 ? "да" : "нет");
    printf("несуществующая метка найдена: %s\n", st.nonexistent == 1 ? "да" : "нет");
    printf("выгрузка обратима:            %s\n", restored == st.nticks ? "да" : "нет");
    printf("сборка работоспособна: %s\n",
           (check == st.total_turnover && pairs == 1
            && st.nonexistent == 1 && restored == st.nticks) ? "да" : "нет");

    return 0;
}
