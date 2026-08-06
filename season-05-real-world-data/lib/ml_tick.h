/*
 * OPERATION MOONLIGHT — библиотека Season 5: бинарный тиковый формат
 * Извлечено из s05e06.
 *
 * Запись на диске не совпадает с записью в памяти: раскладка полей,
 * порядок байтов и содержимое дыр зависят от платформы. Поэтому вся
 * работа с файлом идёт побайтово, явным порядком, младшим байтом вперёд.
 *
 * Используется в s10e07.
 */
#ifndef ML_TICK_H
#define ML_TICK_H

#include <stdint.h>
#include <stddef.h>

/* ---- числа в буфере, младшим байтом вперёд ---- */

void     ml_put_u16(uint8_t *p, uint16_t v);
void     ml_put_u32(uint8_t *p, uint32_t v);
void     ml_put_u64(uint8_t *p, uint64_t v);
void     ml_put_i32(uint8_t *p, int32_t v);
uint16_t ml_get_u16(const uint8_t *p);
uint32_t ml_get_u32(const uint8_t *p);
uint64_t ml_get_u64(const uint8_t *p);
int32_t  ml_get_i32(const uint8_t *p);

/* Зигзаг: знаковое в беззнаковое так, чтобы малые по модулю оставались
   малыми. 0->0, -1->1, 1->2, -2->3. Нужен для дельт. */
uint32_t ml_zigzag(int32_t v);
int32_t  ml_unzigzag(uint32_t v);

/* ---- формат MLTK ---- */

#define ML_TICK_MAGIC     "MLTK"
#define ML_TICK_HDR_BYTES 16
#define ML_TICK_WIRE_V1   15
#define ML_TICK_WIRE_V2   16

typedef struct {
    uint32_t ts;
    int32_t  price;
    uint32_t qty;
    uint16_t symbol_id;
    uint8_t  flags;
    uint8_t  route_id;      /* появилось в версии 2 */
} ml_tick;

typedef struct {
    uint16_t version;
    uint16_t wire_bytes;    /* байт на запись — берётся отсюда, а не из sizeof */
    uint32_t count;
} ml_tick_header;

/* Заголовок. write возвращает число записанных байт (ML_TICK_HDR_BYTES),
   read возвращает 1 при верной сигнатуре и 0 иначе. */
size_t ml_tick_write_header(uint8_t *p, uint16_t version, uint32_t count);
int    ml_tick_read_header(const uint8_t *p, ml_tick_header *h);

/* Запись. Возвращают число обработанных байт: ML_TICK_WIRE_V1 или V2.
   Читателю размер записи следует брать из заголовка: тогда файл более
   новой версии разберётся без поломки, а лишние поля просто пропустятся. */
size_t ml_tick_write(uint8_t *p, const ml_tick *t, uint16_t version);
size_t ml_tick_read(const uint8_t *p, ml_tick *t, uint16_t version);

/* Сравнение по полям. Через memcmp нельзя: содержимое дыр не определено. */
int ml_tick_equal(const ml_tick *a, const ml_tick *b);

/* Дельта-кодирование метки времени и цены. Опорная точка кладётся целиком.
   Преобразование обратимо точно. encode возвращает число байт, decode — 1. */
size_t ml_tick_encode_delta(uint8_t *out, size_t outcap, const ml_tick *t, int n);
int    ml_tick_decode_delta(const uint8_t *in, size_t inlen, ml_tick *t, int n);

#endif /* ML_TICK_H */
