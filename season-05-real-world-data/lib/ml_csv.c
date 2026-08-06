/*
 * OPERATION MOONLIGHT — библиотека Season 5: разбор CSV
 * Реализация ml_csv.h. Извлечено из s05e03.
 */
#include "ml_csv.h"

enum { S_START, S_PLAIN, S_QUOTED, S_AFTER_QUOTE };

static void row_reset(ml_csv_row *r, long off) {
    r->nfield = 0;
    r->status = ML_CSV_OK;
    r->offset = off;
    r->field[0] = r->buf;
}

/* len хранится отдельно: наружу отдаётся только массив полей. */
static void row_putc(ml_csv_row *r, size_t *len, char c) {
    if (*len + 1 >= ML_CSV_ROW_BYTES) { r->status = ML_CSV_OVERFLOW; return; }
    r->buf[(*len)++] = c;
}

static void row_end_field(ml_csv_row *r, size_t *len) {
    if (*len + 1 >= ML_CSV_ROW_BYTES) { r->status = ML_CSV_OVERFLOW; return; }
    r->buf[(*len)++] = '\0';
    if (r->nfield + 1 < ML_CSV_MAX_FIELDS) r->field[r->nfield + 1] = r->buf + *len;
    r->nfield++;
}

long ml_csv_parse(FILE *f, char delim, ml_csv_cb cb, void *user) {
    ml_csv_row r;
    size_t len = 0;
    long   off = 0, row_off = 0, count = 0;
    int    state = S_START, pending = 0, c;

    row_reset(&r, 0);

    while ((c = fgetc(f)) != EOF) {
        off++;
        if (state == S_QUOTED) {
            /* Внутри кавычек управляющий символ ровно один — сама кавычка. */
            if (c == '"') state = S_AFTER_QUOTE;
            else          row_putc(&r, &len, (char)c);
            continue;
        }

        if (c == '"' && state == S_START) { state = S_QUOTED; pending = 1; continue; }
        if (c == '"' && state == S_AFTER_QUOTE) {           /* "" -> " */
            row_putc(&r, &len, '"'); state = S_QUOTED; continue;
        }
        if (c == delim) { row_end_field(&r, &len); state = S_START; pending = 1; continue; }
        if (c == '\r')  { continue; }                        /* голова или хвост CRLF */
        if (c == '\n') {
            row_end_field(&r, &len);
            count++;
            if (cb && !cb(&r, user)) return count;
            len = 0; row_off = off; pending = 0; state = S_START;
            row_reset(&r, row_off);
            continue;
        }
        row_putc(&r, &len, (char)c);
        state = S_PLAIN;
        pending = 1;
    }

    if (state == S_QUOTED) {
        r.status = ML_CSV_UNTERMINATED;
        row_end_field(&r, &len);
        count++;
        if (cb) cb(&r, user);
        return count;
    }
    if (pending || len > 0 || r.nfield > 0) {
        row_end_field(&r, &len);
        count++;
        if (cb) cb(&r, user);
    }
    return count;
}

int ml_csv_split_line(char *line, char delim, char **field, int maxfield) {
    if (!line || !field || maxfield <= 0) return 0;

    int n = 0, quoted = 0;
    char *src = line, *dst = line;
    field[n++] = dst;

    for (; *src; src++) {
        if (*src == '"') {
            if (quoted && src[1] == '"') { *dst++ = '"'; src++; }
            else quoted = !quoted;
        } else if (*src == delim && !quoted) {
            *dst++ = '\0';
            if (n >= maxfield) return n;
            field[n++] = dst;
        } else if (*src == '\r' || *src == '\n') {
            /* Хвост строки: в этом варианте переводов внутри полей нет. */
        } else {
            *dst++ = *src;
        }
    }
    *dst = '\0';
    return n;
}
