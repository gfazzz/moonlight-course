/*
 * OPERATION MOONLIGHT — библиотека Season 5: текст в UTF-8
 * Реализация ml_text.h. Извлечено из s05e08.
 */
#include "ml_text.h"
#include <stdio.h>
#include <string.h>

/* ---------- разбор UTF-8 ---------- */

/* Декодирует одну кодовую точку. Возвращает число съеденных байт,
   0 — если последовательность неверна. Длина определяется старшими
   битами первого байта, продолжающие обязаны иметь вид 10xxxxxx. */
int ml_utf8_decode(const char *s, uint32_t *cp) {
    const uint8_t *p = (const uint8_t *)s;
    if (p[0] < 0x80)          { *cp = p[0]; return 1; }
    if ((p[0] & 0xE0) == 0xC0) {
        if ((p[1] & 0xC0) != 0x80) return 0;
        *cp = ((uint32_t)(p[0] & 0x1F) << 6) | (p[1] & 0x3F);
        return *cp < 0x80 ? 0 : 2;          /* переусложнённая запись — ошибка */
    }
    if ((p[0] & 0xF0) == 0xE0) {
        if ((p[1] & 0xC0) != 0x80 || (p[2] & 0xC0) != 0x80) return 0;
        *cp = ((uint32_t)(p[0] & 0x0F) << 12) | ((uint32_t)(p[1] & 0x3F) << 6)
            | (p[2] & 0x3F);
        return *cp < 0x800 ? 0 : 3;
    }
    if ((p[0] & 0xF8) == 0xF0) {
        if ((p[1] & 0xC0) != 0x80 || (p[2] & 0xC0) != 0x80 || (p[3] & 0xC0) != 0x80)
            return 0;
        *cp = ((uint32_t)(p[0] & 0x07) << 18) | ((uint32_t)(p[1] & 0x3F) << 12)
            | ((uint32_t)(p[2] & 0x3F) << 6) | (p[3] & 0x3F);
        return (*cp < 0x10000 || *cp > 0x10FFFF) ? 0 : 4;
    }
    return 0;
}

/* ---------- три разные величины ---------- */

size_t ml_len_points(const char *s) {
    size_t n = 0; uint32_t cp;
    for (int k; *s; s += k) { k = ml_utf8_decode(s, &cp); if (!k) return n; n++; }
    return n;
}

/* Ширина кодовой точки в знакоместах терминала.
   Ноль — комбинирующие знаки: они пририсовываются к предыдущей букве.
   Два — иероглифы и знаки полной ширины. Единица — всё остальное.
   Таблица сокращённая: полная живёт в базе Unicode и занимает килобайты. */
int ml_cp_width(uint32_t cp) {
    if (cp == 0) return 0;
    if ((cp >= 0x0300 && cp <= 0x036F) ||     /* комбинирующие диакритики */
        (cp >= 0x0483 && cp <= 0x0489) ||
        (cp >= 0x20D0 && cp <= 0x20F0) ||
        (cp >= 0xFE00 && cp <= 0xFE0F))       /* селекторы начертания     */
        return 0;
    if ((cp >= 0x1100 && cp <= 0x115F) ||     /* хангыль                  */
        (cp >= 0x2E80 && cp <= 0xA4CF) ||     /* иероглифы и кана         */
        (cp >= 0xAC00 && cp <= 0xD7A3) ||
        (cp >= 0xF900 && cp <= 0xFAFF) ||
        (cp >= 0xFE30 && cp <= 0xFE6F) ||
        (cp >= 0xFF00 && cp <= 0xFF60) ||
        (cp >= 0xFFE0 && cp <= 0xFFE6))
        return 2;
    return 1;
}

size_t ml_len_columns(const char *s) {
    size_t w = 0; uint32_t cp;
    for (int k; *s; s += k) { k = ml_utf8_decode(s, &cp); if (!k) return w; w += (size_t)ml_cp_width(cp); }
    return w;
}

/* Печать с добивкой до ширины В ЗНАКОМЕСТАХ. Именно этого не умеет
   спецификатор "%-16s": он считает байты. */
void ml_pad_cols(const char *s, size_t width) {
    size_t w = ml_len_columns(s);
    fputs(s, stdout);
    for (; w < width; w++) putchar(' ');
}

/* ---------- алфавиты ---------- */

int ml_cp_script(uint32_t cp) {
    if ((cp >= 'A' && cp <= 'Z') || (cp >= 'a' && cp <= 'z')) return ML_SC_LATIN;
    if (cp >= 0x0400 && cp <= 0x04FF) return ML_SC_CYRILLIC;
    if (cp >= 0x4E00 && cp <= 0x9FFF) return ML_SC_HAN;
    return ML_SC_OTHER;
}

/* Набор алфавитов, встретившихся в строке. Смешение латиницы с кириллицей
   внутри одного слова — то, чего не бывает у честного названия. */
int ml_scripts_of(const char *s) {
    int mask = 0; uint32_t cp;
    for (int k; *s; s += k) { k = ml_utf8_decode(s, &cp); if (!k) break; mask |= ml_cp_script(cp); }
    return mask;
}

/* ---------- скелет строки ---------- */

/* Кириллические буквы, неотличимые от латинских на экране. Таблица
   Unicode называет такие пары confusables; здесь взята её часть. */
uint32_t ml_deconfuse(uint32_t cp) {
    switch (cp) {
    case 0x0410: return 'A'; case 0x0430: return 'a';   /* А а */
    case 0x0412: return 'B';                            /* В   */
    case 0x0415: return 'E'; case 0x0435: return 'e';   /* Е е */
    case 0x041A: return 'K';                            /* К   */
    case 0x041C: return 'M';                            /* М   */
    case 0x041D: return 'H';                            /* Н   */
    case 0x041E: return 'O'; case 0x043E: return 'o';   /* О о */
    case 0x0420: return 'P'; case 0x0440: return 'p';   /* Р р */
    case 0x0421: return 'C'; case 0x0441: return 'c';   /* С с */
    case 0x0422: return 'T';                            /* Т   */
    case 0x0425: return 'X'; case 0x0445: return 'x';   /* Х х */
    case 0x0443: return 'y';                            /* у   */
    default: return cp;
    }
}

/* Скелет: строка приведена к виду, в котором сравнение отвечает на вопрос
   «выглядят ли они одинаково». Заменяем двойников, отбрасываем
   комбинирующие знаки, приводим к нижнему регистру.
   Это не нормализация в смысле Unicode, а её практический заменитель:
   настоящая NFC/NFD требует таблиц разложения на несколько килобайт. */
void ml_skeleton(const char *s, char *out, size_t n) {
    size_t k = 0; uint32_t cp;
    for (int step; *s; s += step) {
        step = ml_utf8_decode(s, &cp);
        if (!step) break;
        if (ml_cp_width(cp) == 0) continue;               /* комбинирующий — прочь */
        cp = ml_deconfuse(cp);
        if (cp >= 'A' && cp <= 'Z') cp += 32;          /* в нижний регистр      */
        if (cp < 0x80) { if (k + 1 < n) out[k++] = (char)cp; }
        else           { if (k + 2 < n) { out[k++] = '?'; } }
    }
    out[k] = '\0';
}

int ml_looks_same(const char *a, const char *b) {
    char sa[512], sb[512];
    ml_skeleton(a, sa, sizeof sa);
    ml_skeleton(b, sb, sizeof sb);
    return strcmp(sa, sb) == 0;
}

int ml_suspicious_scripts(const char *s) {
    int m = ml_scripts_of(s);
    /* Японский законно смешивает несколько письменностей, поэтому
       проверяем не «больше одной», а конкретные сочетания. */
    return (m & ML_SC_LATIN) && (m & ML_SC_CYRILLIC);
}
