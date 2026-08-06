/*
 * OPERATION MOONLIGHT — библиотека Season 5: время
 * Извлечено из s05e04.
 *
 * Календарь и часовые пояса своими силами, без <time.h>. Причина не в
 * принципе, а в воспроизводимости: системная база часовых поясов
 * обновляется вместе с дистрибутивом, и результат, зависящий от неё,
 * меняется без единой правки в коде.
 *
 * Используется в s10e03 (расчёт окна связи).
 */
#ifndef ML_TIME_H
#define ML_TIME_H

#include <stdint.h>
#include <stddef.h>

typedef int64_t ml_time_t;            /* секунды от 1970-01-01 00:00:00 UTC */

typedef struct { int y, mo, d, h, mi, s; } ml_civil;

/* ---- календарь ---- */

/* Дней от эпохи по григорианскому календарю. Работает и до 1970 года. */
int64_t ml_days_from_civil(int y, int m, int d);

/* Обратное преобразование. Для любого z результат, поданный обратно
   в ml_days_from_civil, даёт тот же z. */
void ml_civil_from_days(int64_t z, int *y, int *m, int *d);

/* День недели: 0 — воскресенье. */
int ml_weekday(int64_t days_from_epoch);

/* Високосный ли год. */
int ml_is_leap(int y);

/* Дата n-го воскресенья месяца, в днях от эпохи.
   n > 0 — от начала месяца, n < 0 — от конца (-1 = последнее). */
int64_t ml_nth_sunday(int y, int m, int n);

/* ---- моменты ---- */

ml_time_t ml_secs_from_civil(ml_civil c);
ml_civil  ml_civil_from_secs(ml_time_t t);

/* Запись вида "2024-11-03 01:50:00". Возвращает 1 при успехе. */
int ml_format(ml_time_t t, char *buf, size_t n);

/* ---- часовые пояса ---- */

typedef struct {
    const char *name;
    const char *abbr_std, *abbr_dst;
    int  std_off, dst_off;          /* секунды от UTC */
    int  has_dst;
    int  start_month, start_nth;    /* переход вперёд */
    int  end_month,   end_nth;      /* переход назад  */
    int  switch_hour_local;
} ml_tz;

extern const ml_tz ML_TZ_NEW_YORK;   /* EST/EDT, правила США с 2007 года */
extern const ml_tz ML_TZ_LONDON;     /* GMT/BST                          */
extern const ml_tz ML_TZ_MOSCOW;     /* MSK, перехода нет с 2014 года    */
extern const ml_tz ML_TZ_UTC;

/* Смещение, действующее в данный момент. */
int ml_tz_offset_at(const ml_tz *z, ml_time_t utc);

/* Сокращение пояса, действующее в данный момент. */
const char *ml_tz_abbr_at(const ml_tz *z, ml_time_t utc);

/* Момент -> местное настенное время (как секунды настенной шкалы). */
ml_time_t ml_local_from_utc(const ml_tz *z, ml_time_t utc);

/* Результат обратного преобразования. */
enum { ML_LT_UNIQUE = 0, ML_LT_AMBIGUOUS = 1, ML_LT_NONEXISTENT = 2 };

/* Местное настенное время -> момент.
   ML_LT_AMBIGUOUS: час прожит дважды, *early и *late различны.
   ML_LT_NONEXISTENT: такого местного времени не было, указатели не трогаются.
   Ни то ни другое не является ошибкой ввода: это свойство календаря. */
int ml_utc_from_local(const ml_tz *z, ml_time_t local,
                      ml_time_t *early, ml_time_t *late);

#endif /* ML_TIME_H */
