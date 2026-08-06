/*
 * OPERATION MOONLIGHT — библиотека Season 5: время
 * Реализация ml_time.h. Извлечено из s05e04.
 */
#include "ml_time.h"
#include <stdio.h>

int64_t ml_days_from_civil(int y, int m, int d) {
    y -= (m <= 2);
    int64_t era = (y >= 0 ? y : y - 399) / 400;
    unsigned yoe = (unsigned)(y - era * 400);                 /* год в эре, 0..399  */
    unsigned doy = (unsigned)((153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1);
    unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;     /* день в эре         */
    return era * 146097 + (int64_t)doe - 719468;
}

void ml_civil_from_days(int64_t z, int *yy, int *mm, int *dd) {
    z += 719468;
    int64_t era = (z >= 0 ? z : z - 146096) / 146097;
    unsigned doe = (unsigned)(z - era * 146097);
    unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    int64_t  y   = (int64_t)yoe + era * 400;
    unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    unsigned mp  = (5 * doy + 2) / 153;
    unsigned d   = doy - (153 * mp + 2) / 5 + 1;
    unsigned m   = mp + (mp < 10 ? 3 : (unsigned)-9);
    *yy = (int)(y + (m <= 2)); *mm = (int)m; *dd = (int)d;
}

/* День недели: 0 — воскресенье. 1970-01-01 был четвергом, отсюда +11. */
int ml_weekday(int64_t z) { return (int)(((z % 7) + 11) % 7); }

int ml_is_leap(int y) { return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0); }

/* Дата n-го воскресенья месяца. n > 0 — считая с начала, n < 0 — с конца. */
int64_t ml_nth_sunday(int y, int m, int n) {
    if (n > 0) {
        int64_t first = ml_days_from_civil(y, m, 1);
        int64_t shift = (7 - ml_weekday(first)) % 7;
        return first + shift + 7 * (n - 1);
    }
    int mdays[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
    int last_d = mdays[m] + (m == 2 && ml_is_leap(y));
    int64_t last = ml_days_from_civil(y, m, last_d);
    return last - ml_weekday(last) - 7 * (-n - 1);
}

/* ---------- метка времени ---------- */


ml_time_t ml_secs_from_civil(ml_civil c) {
    return ml_days_from_civil(c.y, c.mo, c.d) * 86400
         + (int64_t)c.h * 3600 + (int64_t)c.mi * 60 + c.s;
}

ml_civil ml_civil_from_secs(int64_t t) {
    /* Деление к нулю неверно для дат до эпохи: −1 секунда лежит в 1969 году,
       а не в 1970. Приводим к делению вниз. */
    int64_t days = t / 86400, rem = t % 86400;
    if (rem < 0) { rem += 86400; days -= 1; }

    ml_civil c;
    ml_civil_from_days(days, &c.y, &c.mo, &c.d);
    c.h  = (int)(rem / 3600);
    c.mi = (int)(rem % 3600 / 60);
    c.s  = (int)(rem % 60);
    return c;
}

int ml_format(ml_time_t t, char *buf, size_t n) {
    ml_civil c = ml_civil_from_secs(t);
    int need = snprintf(buf, n, "%04d-%02d-%02d %02d:%02d:%02d",
                        c.y, c.mo, c.d, c.h, c.mi, c.s);
    return (need > 0 && (size_t)need < n) ? 1 : 0;
}

/* ---------- часовой пояс ---------- */

/* Правила свои, а не из системной базы: база обновляется вместе с
   дистрибутивом, и тест, зависящий от неё, однажды покраснеет без всякой
   правки кода. Для сцены достаточно правил, действующих в США с 2007 года. */

const ml_tz ML_TZ_NEW_YORK = {
    "America/New_York", "EST", "EDT",
    -5 * 3600, -4 * 3600, 1,
    3,  2,        /* второе воскресенье марта   */
    11, 1,        /* первое воскресенье ноября  */
    2
};

/* Момент весеннего перехода в UTC: местные часы показывают switch_hour,
   и в этот миг ещё действует зимнее смещение. */
static ml_time_t dst_start_utc(const ml_tz *z, int year) {
    int64_t day = ml_nth_sunday(year, z->start_month, z->start_nth);
    return day * 86400 + (int64_t)z->switch_hour_local * 3600 - z->std_off;
}

/* Момент осеннего перехода: в этот миг ещё действует летнее смещение. */
static ml_time_t dst_end_utc(const ml_tz *z, int year) {
    int64_t day = ml_nth_sunday(year, z->end_month, z->end_nth);
    return day * 86400 + (int64_t)z->switch_hour_local * 3600 - z->dst_off;
}

/* Смещение, действующее в данный момент UTC. */
int ml_tz_offset_at(const ml_tz *z, int64_t utc) {
    if (!z->has_dst) return z->std_off;
    ml_civil c = ml_civil_from_secs(utc);
    int64_t a = dst_start_utc(z, c.y), b = dst_end_utc(z, c.y);
    return (utc >= a && utc < b) ? z->dst_off : z->std_off;
}

const char *ml_tz_abbr_at(const ml_tz *z, int64_t utc) {
    return ml_tz_offset_at(z, utc) == z->dst_off ? z->abbr_dst : z->abbr_std;
}

/* UTC -> местное настенное время (как число секунд «настенной» шкалы). */
ml_time_t ml_local_from_utc(const ml_tz *z, int64_t utc) {
    return utc + ml_tz_offset_at(z, utc);
}

/* ---------- обратное преобразование: здесь и живёт вся сложность ---------- */

/* Местное настенное время -> UTC.
   Пробуем оба смещения и проверяем каждое на согласованность: если,
   вычтя смещение, мы попадаем в момент, где действует именно оно, —
   вариант настоящий.
     оба настоящие  -> час прожит дважды;
     ни одного      -> такого местного времени не существовало.
   Обе ситуации бывают ровно раз в год, и обе — данные, а не ошибка. */
int ml_utc_from_local(const ml_tz *z, ml_time_t local, ml_time_t *a, ml_time_t *b) {
    int64_t c1 = local - z->std_off;
    int64_t c2 = local - z->dst_off;
    int ok1 = (ml_tz_offset_at(z, c1) == z->std_off);
    int ok2 = z->has_dst && (ml_tz_offset_at(z, c2) == z->dst_off);

    if (ok1 && ok2 && c1 != c2) { *a = c2; *b = c1; return ML_LT_AMBIGUOUS; }
    if (ok1) { *a = *b = c1; return ML_LT_UNIQUE; }
    if (ok2) { *a = *b = c2; return ML_LT_UNIQUE; }
    return ML_LT_NONEXISTENT;
}

const ml_tz ML_TZ_LONDON = {
    "Europe/London", "GMT", "BST", 0, 3600, 1, 3, -1, 10, -1, 1
};

const ml_tz ML_TZ_MOSCOW = {
    "Europe/Moscow", "MSK", "MSK", 3 * 3600, 3 * 3600, 0, 0, 0, 0, 0, 0
};

const ml_tz ML_TZ_UTC = {
    "UTC", "UTC", "UTC", 0, 0, 0, 0, 0, 0, 0, 0
};
