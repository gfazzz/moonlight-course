/*
 * OPERATION MOONLIGHT — s09e03 "Интерпретатор"
 * Эталонное решение: interp.c
 *
 * Концепт серии: дерево (s09e02) само по себе ничего не делает. Интерпретатор
 *                обходит его и ВЫПОЛНЯЕТ, храня состояние программы в окружении
 *                (environment): таблица «имя -> значение». Появляются вещи,
 *                которых не было в арифметике:
 *                  - присваивание меняет состояние (побочный эффект);
 *                  - if/while требуют ОТЛОЖЕННОГО вычисления ветвей: нельзя
 *                    вычислить обе и выбрать — надо вычислять только нужную;
 *                  - ошибки времени выполнения (неизвестная переменная, деление
 *                    на ноль) отличаются от ошибок разбора.
 * Задача: интерпретатор языка MOON — переменные, if/else, while, print.
 *
 * Для компактности разбор и исполнение совмещены: это «интерпретатор на лету»
 * (одноприходный), классика для маленьких языков. Тонкость с while разобрана
 * в theory.md — тело приходится перечитывать, возвращая лексер назад.
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* ---------- окружение ---------- */
#define MAXVARS 32
typedef struct { char name[24]; long value; } Var;
static Var vars[MAXVARS];
static int nvars = 0;
static int runtime_error = 0;

static long *env_find(const char *name) {
    for (int i = 0; i < nvars; i++) if (strcmp(vars[i].name, name) == 0) return &vars[i].value;
    return NULL;
}
static void env_set(const char *name, long v) {
    long *p = env_find(name);
    if (p) { *p = v; return; }
    if (nvars < MAXVARS) { snprintf(vars[nvars].name, sizeof vars[0].name, "%s", name);
                           vars[nvars].value = v; nvars++; }
}

/* ---------- лексер ---------- */
typedef enum {
    K_NUM, K_IDENT, K_IF, K_ELSE, K_WHILE, K_PRINT,
    K_PLUS, K_MINUS, K_STAR, K_SLASH,
    K_ASSIGN, K_EQ, K_NE, K_LT, K_LE, K_GT, K_GE,
    K_LP, K_RP, K_LBRACE, K_RBRACE, K_SEMI, K_END, K_BAD
} Kind;

typedef struct {
    const char *src;
    size_t pos;
    Kind kind;
    long num;
    char text[24];
} Lex;

static void skip_ws(Lex *L) {
    for (;;) {
        while (isspace((unsigned char)L->src[L->pos])) L->pos++;
        if (L->src[L->pos] == '/' && L->src[L->pos + 1] == '/') {
            while (L->src[L->pos] && L->src[L->pos] != '\n') L->pos++;
            continue;
        }
        return;
    }
}

static void next(Lex *L) {
    skip_ws(L);
    L->text[0] = '\0';
    char c = L->src[L->pos];
    if (!c) { L->kind = K_END; return; }

    if (isdigit((unsigned char)c)) {
        long v = 0;
        while (isdigit((unsigned char)L->src[L->pos])) v = v * 10 + (L->src[L->pos++] - '0');
        L->num = v; L->kind = K_NUM; return;
    }
    if (isalpha((unsigned char)c) || c == '_') {
        size_t n = 0;
        while ((isalnum((unsigned char)L->src[L->pos]) || L->src[L->pos] == '_')
               && n < sizeof L->text - 1)
            L->text[n++] = L->src[L->pos++];
        L->text[n] = '\0';
        if      (strcmp(L->text, "if") == 0)    L->kind = K_IF;
        else if (strcmp(L->text, "else") == 0)  L->kind = K_ELSE;
        else if (strcmp(L->text, "while") == 0) L->kind = K_WHILE;
        else if (strcmp(L->text, "print") == 0) L->kind = K_PRINT;
        else                                    L->kind = K_IDENT;
        return;
    }

    char d = L->src[L->pos + 1];
    if (c == '=' && d == '=') { L->pos += 2; L->kind = K_EQ; return; }
    if (c == '!' && d == '=') { L->pos += 2; L->kind = K_NE; return; }
    if (c == '<' && d == '=') { L->pos += 2; L->kind = K_LE; return; }
    if (c == '>' && d == '=') { L->pos += 2; L->kind = K_GE; return; }

    L->pos++;
    switch (c) {
        case '+': L->kind = K_PLUS;   break;
        case '-': L->kind = K_MINUS;  break;
        case '*': L->kind = K_STAR;   break;
        case '/': L->kind = K_SLASH;  break;
        case '=': L->kind = K_ASSIGN; break;
        case '<': L->kind = K_LT;     break;
        case '>': L->kind = K_GT;     break;
        case '(': L->kind = K_LP;     break;
        case ')': L->kind = K_RP;     break;
        case '{': L->kind = K_LBRACE; break;
        case '}': L->kind = K_RBRACE; break;
        case ';': L->kind = K_SEMI;   break;
        default:  L->kind = K_BAD;    break;
    }
}

/* ---------- выражения (как в s09e02, но с переменными) ---------- */
static long expr(Lex *L, int exec);

static long factor(Lex *L, int exec) {
    if (L->kind == K_MINUS) { next(L); return -factor(L, exec); }
    if (L->kind == K_NUM)   { long v = L->num; next(L); return v; }
    if (L->kind == K_IDENT) {
        char name[24];
        snprintf(name, sizeof name, "%s", L->text);
        next(L);
        long *p = env_find(name);
        if (!p) {
            if (exec) { printf("  ошибка: переменная '%s' не определена\n", name); runtime_error = 1; }
            return 0;
        }
        return *p;
    }
    if (L->kind == K_LP) { next(L); long v = expr(L, exec); if (L->kind == K_RP) next(L); return v; }
    next(L);
    return 0;
}

static long term(Lex *L, int exec) {
    long v = factor(L, exec);
    for (;;) {
        if (L->kind == K_STAR)       { next(L); v *= factor(L, exec); }
        else if (L->kind == K_SLASH) {
            next(L);
            long d = factor(L, exec);
            if (d == 0) {
                if (exec) { printf("  ошибка: деление на ноль\n"); runtime_error = 1; }
                return 0;
            }
            v /= d;
        } else return v;
    }
}

static long sum(Lex *L, int exec) {
    long v = term(L, exec);
    for (;;) {
        if (L->kind == K_PLUS)       { next(L); v += term(L, exec); }
        else if (L->kind == K_MINUS) { next(L); v -= term(L, exec); }
        else return v;
    }
}

/* Сравнения — самый слабый приоритет. */
static long expr(Lex *L, int exec) {
    long v = sum(L, exec);
    for (;;) {
        Kind k = L->kind;
        if (k == K_EQ || k == K_NE || k == K_LT || k == K_LE || k == K_GT || k == K_GE) {
            next(L);
            long r = sum(L, exec);
            switch (k) {
                case K_EQ: v = (v == r); break;
                case K_NE: v = (v != r); break;
                case K_LT: v = (v <  r); break;
                case K_LE: v = (v <= r); break;
                case K_GT: v = (v >  r); break;
                default:   v = (v >= r); break;
            }
        } else return v;
    }
}

/* ---------- операторы ---------- */
/* exec == 0 означает «разобрать, но НЕ выполнять» — так пропускается
   невыбранная ветвь if и тело while с ложным условием. */
static void statement(Lex *L, int exec);

static void block(Lex *L, int exec) {
    if (L->kind != K_LBRACE) { statement(L, exec); return; }
    next(L);
    while (L->kind != K_RBRACE && L->kind != K_END) statement(L, exec);
    if (L->kind == K_RBRACE) next(L);
}

static void statement(Lex *L, int exec) {
    if (runtime_error) { L->kind = K_END; return; }

    if (L->kind == K_PRINT) {
        next(L);
        long v = expr(L, exec);
        /* Если при вычислении случилась ошибка — печатать нечего. */
        if (exec && !runtime_error) printf("  print: %ld\n", v);
        if (L->kind == K_SEMI) next(L);
        return;
    }

    if (L->kind == K_IF) {
        next(L);
        if (L->kind == K_LP) next(L);
        long cond = expr(L, exec);
        if (L->kind == K_RP) next(L);
        block(L, exec && cond);                 /* тело выполняем только при истине */
        if (L->kind == K_ELSE) {
            next(L);
            block(L, exec && !cond);            /* ...и ровно одну ветвь */
        }
        return;
    }

    if (L->kind == K_WHILE) {
        next(L);
        size_t cond_pos = L->pos;               /* запоминаем, куда возвращаться */
        Kind cond_kind = L->kind;
        long guard = 0;
        for (;;) {
            L->pos = cond_pos; L->kind = cond_kind;   /* перечитываем условие */
            if (L->kind == K_LP) next(L);
            long cond = expr(L, exec);
            if (L->kind == K_RP) next(L);
            int run = exec && cond && !runtime_error;
            block(L, run);
            if (!run) return;                   /* цикл закончился (или мы в режиме пропуска) */
            if (++guard > 10000) { printf("  ошибка: слишком много итераций\n");
                                   runtime_error = 1; return; }
        }
    }

    if (L->kind == K_IDENT) {
        char name[24];
        snprintf(name, sizeof name, "%s", L->text);
        next(L);
        if (L->kind == K_ASSIGN) {
            next(L);
            long v = expr(L, exec);
            if (exec && !runtime_error) env_set(name, v);
            if (L->kind == K_SEMI) next(L);
            return;
        }
        if (L->kind == K_SEMI) next(L);
        return;
    }

    next(L);                                     /* пропускаем непонятное */
}

static void run_program(const char *title, const char *src) {
    printf("--- %s ---\n", title);
    nvars = 0;
    runtime_error = 0;

    Lex L = { src, 0, K_END, 0, {0} };
    next(&L);
    while (L.kind != K_END) statement(&L, 1);

    printf("  переменные:");
    if (nvars == 0) printf(" (нет)");
    for (int i = 0; i < nvars; i++) printf(" %s=%ld", vars[i].name, vars[i].value);
    printf("\n\n");
}

int main(void) {
    printf("=== интерпретатор языка MOON ===\n\n");

    run_program("присваивание и арифметика",
        "x = 3 + 4 * 2;\n"
        "y = (x - 1) * 2;\n"
        "print x; print y;\n");

    run_program("ветвление",
        "threat = 12;\n"
        "if (threat >= 10) { level = 2; print level; }\n"
        "else { level = 1; print level; }\n");

    run_program("цикл: сумма 1..5",
        "i = 1; total = 0;\n"
        "while (i <= 5) { total = total + i; i = i + 1; }\n"
        "print total;\n");

    run_program("вложенный цикл: факториал 5",
        "n = 5; f = 1;\n"
        "while (n > 1) { f = f * n; n = n - 1; }\n"
        "print f;\n");

    run_program("ошибка: неизвестная переменная",
        "a = 1;\n"
        "print a + b;\n");

    run_program("ошибка: деление на ноль",
        "z = 0;\n"
        "print 10 / z;\n");

    printf("интерпретатор = обход дерева + окружение + отложенное вычисление ветвей.\n");
    return 0;
}
