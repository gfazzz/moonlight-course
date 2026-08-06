/* OPERATION MOONLIGHT — s05e06 «Тиковый поток»
 * Каркас: скопируй в artifacts/tick.c и заполни TODO.
 *
 *     cp starter.c artifacts/tick.c
 *     make test
 *
 * Заполнять нужно перестановку полей структуры, четыре функции работы
 * с байтами, зигзаг и дельта-кодирование. Печать и сцены уже готовы.
 *
 * Нью-Йорк, 10 октября, 11:15.
 *
 * Тот же поток сделок: два гигабайта текстом, двести мегабайт в упакованном
 * виде. Ева показывает формат, которым фонд обменивается с брокером, — и в
 * заголовке есть поле версии, которого нет в документации.
 *
 * Концепт серии:
 *   - выравнивание структур и дыры между полями;
 *   - порядок байтов и почему структуру нельзя писать в файл целиком;
 *   - версия формата в заголовке;
 *   - дельта-кодирование временных рядов.
 *
 * Детерминизм: поток порождается ЛКГ с фиксированным зерном, файл пишется
 * байт за байтом в явном порядке. Величины, зависящие от машины, уходят
 * в stderr и в сравниваемый вывод не попадают. Собирается без -lm.
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* ---------- запись как её пишут в первый раз ---------- */

/* Поля расставлены в том порядке, в каком о них подумали. Компилятор
   обязан выровнять каждое поле по его размеру — и вставляет дыры. */
typedef struct {
    uint16_t symbol_id;
    uint32_t ts;          /* секунды от начала торгового дня */
    int32_t  price;       /* цена в сотых доллара            */
    uint32_t qty;
    uint8_t  flags;
} tick_naive;

/* TODO 1. Та же запись без дыр. Переставь поля так, чтобы между ними
   не осталось пустого места. Правило простое: по убыванию размера.
   Проверить себя можно через offsetof — смещение каждого поля должно
   совпадать с концом предыдущего. */
typedef struct {
    uint16_t symbol_id;
    uint32_t ts;
    int32_t  price;
    uint32_t qty;
    uint8_t  flags;
} tick_sorted;

/* ---------- логическая запись ---------- */

typedef struct {
    uint32_t ts;
    int32_t  price;
    uint32_t qty;
    uint16_t symbol_id;
    uint8_t  flags;
    uint8_t  route_id;    /* появилось в версии 2 формата */
} tick_t;

#define WIRE_V1 15        /* байт на запись в версии 1 */
#define WIRE_V2 16        /* в версии 2 добавлен route_id */

/* ---------- явный порядок байтов ---------- */

/* Структуру нельзя записать в файл целиком: раскладка зависит от
   компилятора и платформы. Пишем поле за полем, младшим байтом вперёд. */
/* TODO 2. Положить значение в буфер младшим байтом вперёд и достать
   обратно. Здесь нельзя ни memcpy структуры, ни приведения указателя
   к uint32_t*: первое зависит от машины, второе вдобавок нарушает
   выравнивание. Только сдвиги и маски — они определены стандартом
   и дают один и тот же файл на любой платформе.
   При чтении не забудь привести байт к нужному типу ДО сдвига:
   p[1] << 8 для uint16_t сработает случайно, p[3] << 24 — уже нет. */
static void put_u16(uint8_t *p, uint16_t v) {
    (void)p; (void)v;
}
static void put_u32(uint8_t *p, uint32_t v) {
    (void)p; (void)v;
}
static uint16_t get_u16(const uint8_t *p) {
    (void)p; return 0;
}
static uint32_t get_u32(const uint8_t *p) {
    (void)p; return 0;
}

/* Знаковое кладём через беззнаковое: сдвиг вправо отрицательного числа
   реализационно-зависим, а преобразование в uint32_t определено стандартом. */
static void put_i32(uint8_t *p, int32_t v) { put_u32(p, (uint32_t)v); }
static int32_t get_i32(const uint8_t *p)   { return (int32_t)get_u32(p); }

/* Зигзаг: знаковое в беззнаковое так, чтобы малые по модулю числа
   оставались малыми. -1 -> 1, 1 -> 2, -2 -> 3. Нужно для дельт. */
/* TODO 3. Зигзаг и обратное преобразование.
   Прямое: сдвинуть влево на 1 и сложить по модулю два со знаковой маской
   (v >> 31 даёт все единицы для отрицательных и нули для остальных).
   Обратное: сдвинуть вправо на 1 и сложить с маской, построенной из
   младшего бита. */
static uint32_t zigzag(int32_t v)   { (void)v; return 0; }
static int32_t  unzigzag(uint32_t v){ (void)v; return 0; }

/* ---------- поток ---------- */

#define NTICKS 64

static uint32_t rng_state;
static uint32_t rng_next(void) {
    rng_state = rng_state * 1664525u + 1013904223u;
    return rng_state >> 16;
}

static void generate(tick_t *t, int n) {
    rng_state = 10102025u;
    uint32_t ts = 34200;          /* 09:30:00 от полуночи */
    int32_t  px = 18742;          /* 187.42 */

    for (int i = 0; i < n; i++) {
        ts += 1 + rng_next() % 5;              /* тик каждые 1..5 секунд   */
        px += (int32_t)(rng_next() % 9) - 4;   /* шаг цены -4..+4 сотых    */
        t[i].ts        = ts;
        t[i].price     = px;
        t[i].qty       = 100 * (1 + rng_next() % 20);
        t[i].symbol_id = 1;
        t[i].flags     = (uint8_t)(rng_next() % 2);
        /* Каждая шестая сделка идёт нестандартным маршрутом. */
        t[i].route_id  = (i % 6 == 5) ? 7 : 0;
    }
}

/* ---------- заголовок файла ---------- */

#define MAGIC "MLTK"
#define HDR_BYTES 16

static size_t write_header(uint8_t *buf, uint16_t version, uint32_t count) {
    memcpy(buf, MAGIC, 4);
    put_u16(buf + 4, version);
    put_u16(buf + 6, (uint16_t)(version == 1 ? WIRE_V1 : WIRE_V2));
    put_u32(buf + 8, count);
    put_u32(buf + 12, 0);         /* запас: заголовок кратен 16 байтам */
    return HDR_BYTES;
}

static size_t write_tick(uint8_t *p, const tick_t *t, uint16_t version) {
    put_u32(p + 0,  t->ts);
    put_i32(p + 4,  t->price);
    put_u32(p + 8,  t->qty);
    put_u16(p + 12, t->symbol_id);
    p[14] = t->flags;
    if (version == 1) return WIRE_V1;
    p[15] = t->route_id;
    return WIRE_V2;
}

static size_t read_tick(const uint8_t *p, tick_t *t, uint16_t version) {
    t->ts        = get_u32(p + 0);
    t->price     = get_i32(p + 4);
    t->qty       = get_u32(p + 8);
    t->symbol_id = get_u16(p + 12);
    t->flags     = p[14];
    if (version == 1) { t->route_id = 0; return WIRE_V1; }
    t->route_id = p[15];
    return WIRE_V2;
}

/* ---------- дельта-кодирование ---------- */

/* Метки времени растут, цена ходит рядом с прежней. Разности умещаются
   в один-два байта там, где абсолютные значения требуют четырёх. */
/* TODO 4. Первую запись положить целиком (это опорная точка), дальше —
   только разности с предыдущей: шаг времени байтом, шаг цены байтом
   через зигзаг. Вернуть число записанных байт.
   Декодер обязан восстановить ряд точно: дельта-кодирование не теряет
   ничего, оно только меняет представление. */
static size_t encode_delta(uint8_t *out, const tick_t *t, int n) {
    (void)out; (void)t; (void)n;
    (void)zigzag; (void)unzigzag;   /* строка уйдёт с первым вызовом */
    return 0;
}

static int decode_delta(const uint8_t *in, tick_t *t, int n) {
    (void)in; (void)t; (void)n;
    return 0;
}

/* ---------- отчёт ---------- */

static void hexdump(const uint8_t *p, size_t n) {
    for (size_t i = 0; i < n; i++) printf(i ? " %02X" : "%02X", p[i]);
}

/* Сравнивать записи через memcmp нельзя: в дыры между полями компилятор
   не обязан ничего класть, и их содержимое не определено. Совпадение
   на своей машине здесь ничего не доказывает. */
static int tick_equal(const tick_t *a, const tick_t *b) {
    return a->ts == b->ts && a->price == b->price && a->qty == b->qty
        && a->symbol_id == b->symbol_id && a->flags == b->flags
        && a->route_id == b->route_id;
}

int main(int argc, char **argv) {
    const char *path = (argc > 1) ? argv[1] : "ticks.bin";

    printf("=== тиковый поток: бинарный формат ===\n\n");

    /* ---------- 1. Дыры в структуре ---------- */
    printf("--- как структура лежит в памяти ---\n");
    printf("поле        смещение\n");
    printf("symbol_id   %2zu\n", offsetof(tick_naive, symbol_id));
    printf("ts          %2zu\n", offsetof(tick_naive, ts));
    printf("price       %2zu\n", offsetof(tick_naive, price));
    printf("qty         %2zu\n", offsetof(tick_naive, qty));
    printf("flags       %2zu\n", offsetof(tick_naive, flags));

    size_t useful = sizeof(uint16_t) + 3 * sizeof(uint32_t) + sizeof(uint8_t);
    printf("\nsizeof(tick_naive):  %2zu\n", sizeof(tick_naive));
    printf("полезных байт:       %2zu\n", useful);
    printf("потеряно на дыры:    %2zu\n", sizeof(tick_naive) - useful);

    /* Дыра между полями есть, если смещение следующего поля больше,
       чем конец предыдущего. Хвостовое выравнивание — не дыра. */
    int holes_naive = (offsetof(tick_naive, ts) > offsetof(tick_naive, symbol_id) + 2);
    int holes_sorted = (offsetof(tick_sorted, price) > offsetof(tick_sorted, ts) + 4)
                    || (offsetof(tick_sorted, symbol_id) > offsetof(tick_sorted, qty) + 4)
                    || (offsetof(tick_sorted, flags) > offsetof(tick_sorted, symbol_id) + 2);
    printf("дыры между полями:   %s\n", holes_naive ? "есть" : "нет");

    printf("\nте же поля в порядке убывания размера:\n");
    printf("sizeof(tick_sorted): %2zu\n", sizeof(tick_sorted));
    printf("дыры между полями:   %s\n", holes_sorted ? "есть" : "нет");

    /* ---------- 2. Порядок байтов ---------- */
    printf("\n--- порядок байтов ---\n");
    uint8_t probe[4];
    put_u32(probe, 0x11223344u);
    printf("значение 0x11223344 в файле лежит как: ");
    hexdump(probe, 4);
    printf("\n");
    printf("младший байт первым:                   да\n");
    printf("раскладка не зависит от машины:        %s\n",
           get_u32(probe) == 0x11223344u ? "да" : "нет");

    /* Порядок байтов самой машины — величина, которая на разных платформах
       разная. В сравниваемый вывод такие вещи не попадают (см. s08e01). */
    uint32_t host = 0x11223344u;
    uint8_t hb[4];
    memcpy(hb, &host, 4);
    fprintf(stderr, "[stderr] в памяти этой машины: %02X %02X %02X %02X\n",
            hb[0], hb[1], hb[2], hb[3]);

    /* ---------- 3. Запись файла ---------- */
    static tick_t src[NTICKS], dst[NTICKS];
    generate(src, NTICKS);

    static uint8_t buf[HDR_BYTES + NTICKS * WIRE_V2];
    size_t k = write_header(buf, 2, NTICKS);
    for (int i = 0; i < NTICKS; i++) k += write_tick(buf + k, &src[i], 2);

    FILE *f = fopen(path, "wb");
    if (!f) { printf("FAIL: не создать %s\n", path); return 1; }
    fwrite(buf, 1, k, f);
    fclose(f);

    printf("\n--- заголовок файла ---\n");
    printf("сигнатура:        %.4s\n", (const char *)buf);
    printf("версия:           %u\n", get_u16(buf + 4));
    printf("байт на запись:   %u\n", get_u16(buf + 6));
    printf("записей:          %u\n", get_u32(buf + 8));
    printf("размер заголовка: %d\n", HDR_BYTES);
    printf("первая запись:    ");
    hexdump(buf + HDR_BYTES, WIRE_V2);
    printf("\n");

    /* ---------- 4. Чтение обратно ---------- */
    static uint8_t rbuf[sizeof buf];
    f = fopen(path, "rb");
    if (!f) { printf("FAIL: не открыть %s\n", path); return 1; }
    size_t got = fread(rbuf, 1, sizeof rbuf, f);
    fclose(f);
    remove(path);

    printf("\n--- чтение обратно ---\n");
    printf("байт прочитано:   %zu\n", got);
    printf("сигнатура верна:  %s\n", memcmp(rbuf, MAGIC, 4) == 0 ? "да" : "нет");

    uint16_t ver   = get_u16(rbuf + 4);
    uint16_t wire  = get_u16(rbuf + 6);
    uint32_t count = get_u32(rbuf + 8);

    /* Размер записи берётся из заголовка, а не из sizeof: читатель, собранный
       другим компилятором, обязан понять файл так же. */
    printf("размер записи из заголовка: %u\n", wire);

    size_t off = HDR_BYTES;
    for (uint32_t i = 0; i < count && i < NTICKS; i++)
        off += read_tick(rbuf + off, &dst[i], ver);

    int same = 1;
    for (int i = 0; i < NTICKS; i++) if (!tick_equal(&src[i], &dst[i])) same = 0;
    printf("все записи совпали: %s\n", same ? "да" : "нет");
    printf("байт разобрано:     %zu\n", off);

    /* ---------- 5. Размеры ---------- */
    printf("\n--- сколько это занимает ---\n");

    /* Текстовая строка вида "34205,18742,1300,1,0,0\n". */
    size_t as_text = 0;
    for (int i = 0; i < NTICKS; i++) {
        char line[64];
        as_text += (size_t)snprintf(line, sizeof line, "%u,%d,%u,%u,%u,%u\n",
                                    src[i].ts, src[i].price, src[i].qty,
                                    src[i].symbol_id, src[i].flags, src[i].route_id);
    }

    static uint8_t dbuf[NTICKS * 8];
    size_t as_delta = encode_delta(dbuf, src, NTICKS);

    printf("%-28s %6zu байт\n", "текстом:",              as_text);
    printf("%-28s %6zu байт\n", "структурой как есть:",  NTICKS * sizeof(tick_naive));
    printf("%-28s %6zu байт\n", "упакованной записью:",  (size_t)(NTICKS * WIRE_V2));
    printf("%-28s %6zu байт\n", "ts и price с дельтами:", as_delta);
    printf("\nво сколько раз дельты меньше текста: %zu\n", as_text / as_delta);

    /* ---------- 6. Дельты обратимы ---------- */
    static tick_t back[NTICKS];
    decode_delta(dbuf, back, NTICKS);
    int delta_ok = 1;
    for (int i = 0; i < NTICKS; i++)
        if (back[i].ts != src[i].ts || back[i].price != src[i].price) delta_ok = 0;
    printf("дельты восстановились точно: %s\n", delta_ok ? "да" : "нет");

    /* ---------- 7. Поле, которого нет в документации ---------- */
    printf("\n--- поле route_id ---\n");
    printf("в версии 1 запись занимает %d байт, в версии 2 — %d\n", WIRE_V1, WIRE_V2);
    printf("документация фонда описывает версию 1\n");

    int routes[8] = { 0 };
    for (int i = 0; i < NTICKS; i++)
        if (dst[i].route_id < 8) routes[dst[i].route_id]++;

    printf("\nзначение  сделок\n");
    for (int r = 0; r < 8; r++)
        if (routes[r]) printf("%8d  %6d\n", r, routes[r]);

    printf("\nдоля нестандартного маршрута: %d из %d\n", routes[7], NTICKS);
    printf("в документированной версии этих байт не видно: да\n");

    return 0;
}
