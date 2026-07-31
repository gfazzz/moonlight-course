/*
 * OPERATION MOONLIGHT — s09e03 "Интерпретатор"
 * Каркас. Скопируй в artifacts/interp.c и реализуй.
 *
 *   cp starter.c artifacts/interp.c
 *   make test
 *
 * Грамматика и требования — в mission.md, формат вывода — в tests/expected.txt.
 *
 * Ключевая идея реализации: функции разбора принимают флаг `exec`.
 *   exec == 1 -> разбирать И выполнять
 *   exec == 0 -> разбирать, но НЕ выполнять (пропуск невыбранной ветви)
 * Так поток токенов остаётся согласованным, а лишний код не исполняется.
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* --- окружение --- */
#define MAXVARS 32
typedef struct { char name[24]; long value; } Var;
static Var vars[MAXVARS];
static int nvars = 0;
static int runtime_error = 0;

/* --- лексер --- */
typedef enum {
    K_NUM, K_IDENT, K_IF, K_ELSE, K_WHILE, K_PRINT,
    K_PLUS, K_MINUS, K_STAR, K_SLASH,
    K_ASSIGN, K_EQ, K_NE, K_LT, K_LE, K_GT, K_GE,
    K_LP, K_RP, K_LBRACE, K_RBRACE, K_SEMI, K_END, K_BAD
} Kind;

typedef struct {
    const char *src;
    size_t pos;       /* позиция в тексте: сохраняй её, чтобы while мог вернуться */
    Kind kind;
    long num;
    char text[24];
} Lex;

/* Реализуй: */
static long *env_find(const char *name);
static void  env_set(const char *name, long v);
static void  next(Lex *L);
static long  expr(Lex *L, int exec);
static void  statement(Lex *L, int exec);
static void  block(Lex *L, int exec);

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

/* --- Твоя реализация --- */
static long *env_find(const char *name) { (void)name; return NULL; }
static void  env_set(const char *name, long v) { (void)name; (void)v; }
static void  next(Lex *L) { L->kind = K_END; }
static long  expr(Lex *L, int exec) { (void)L; (void)exec; return 0; }
static void  block(Lex *L, int exec) { (void)L; (void)exec; }
static void  statement(Lex *L, int exec) { (void)exec; L->kind = K_END; }
