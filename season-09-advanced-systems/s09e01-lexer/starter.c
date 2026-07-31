/*
 * OPERATION MOONLIGHT — s09e01 "Лексер"
 * Каркас. Скопируй в artifacts/lexer.c и реализуй.
 *
 *   cp starter.c artifacts/lexer.c
 *   make test
 *
 * ВНИМАНИЕ: с Season 9 подсказок меньше. Здесь только ИНТЕРФЕЙС и требования —
 * пошаговых TODO больше нет. Полное ТЗ и критерии приёмки — в mission.md,
 * ожидаемый вывод (он же спецификация формата) — в tests/expected.txt.
 * Эталон в solution/ открывай только после честной попытки.
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* --- Обязательный интерфейс (менять нельзя: на него опирается тест) --- */

typedef enum {
    T_NUM, T_IDENT, T_KEYWORD,
    T_PLUS, T_MINUS, T_STAR, T_SLASH,
    T_ASSIGN, T_EQ, T_NE, T_LT, T_LE, T_GT, T_GE,
    T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE, T_SEMI,
    T_EOF, T_ERROR
} TokType;

static const char *TOKNAME[] = {
    "NUM", "IDENT", "KEYWORD",
    "PLUS", "MINUS", "STAR", "SLASH",
    "ASSIGN", "EQ", "NE", "LT", "LE", "GT", "GE",
    "LPAREN", "RPAREN", "LBRACE", "RBRACE", "SEMI",
    "EOF", "ERROR"
};

typedef struct {
    TokType type;
    char    text[32];
    long    value;      /* заполняется только для T_NUM */
    int     line, col;  /* позиция начала токена; строки и столбцы с 1 */
} Token;

static const char *KEYWORDS[] = {"if", "else", "while", "print", NULL};

typedef struct {
    const char *src;
    size_t pos;
    int line, col;
} Lexer;

/* Реализуй: */
static void  lex_init(Lexer *lx, const char *src);
static Token next_token(Lexer *lx);      /* возвращает T_EOF в конце, T_ERROR на мусоре */

int main(void) {
    const char *program =
        "// счётчик угроз\n"
        "x = 12;\n"
        "if (x >= 10) {\n"
        "  threat = x * 2 + 1;\n"
        "  print threat;\n"
        "}\n"
        "while (x != 0) { x = x - 1; }\n"
        "y = 3 @ 4;\n";

    printf("=== лексер языка MOON ===\n\n");
    printf("%-6s %-8s %-10s %s\n", "line", "type", "text", "value");
    printf("---------------------------------------\n");

    Lexer lx;
    lex_init(&lx, program);

    int counts[T_ERROR + 1] = {0};
    int total = 0, errors = 0;

    for (;;) {
        Token t = next_token(&lx);
        counts[t.type]++;
        total++;

        if (t.type == T_ERROR) {
            printf("%3d:%-2d %-8s %-10s <-- неизвестный символ\n",
                   t.line, t.col, TOKNAME[t.type], t.text);
            errors++;
        } else if (t.type == T_NUM) {
            printf("%3d:%-2d %-8s %-10s %ld\n", t.line, t.col, TOKNAME[t.type], t.text, t.value);
        } else {
            printf("%3d:%-2d %-8s %-10s\n", t.line, t.col, TOKNAME[t.type], t.text);
        }

        if (t.type == T_EOF) break;
    }

    printf("\nвсего токенов: %d (включая EOF)\n", total);
    printf("чисел: %d, идентификаторов: %d, ключевых слов: %d\n",
           counts[T_NUM], counts[T_IDENT], counts[T_KEYWORD]);
    printf("ошибочных символов: %d\n", errors);
    printf("\nлексер не знает грамматики — он лишь режет поток на слова.\n");
    return 0;
}

/* --- Твоя реализация --- */
static void lex_init(Lexer *lx, const char *src) { (void)lx; (void)src; }
static Token next_token(Lexer *lx) {
    (void)lx;
    Token t; memset(&t, 0, sizeof t);
    t.type = T_EOF; strcpy(t.text, "<eof>"); t.line = 1; t.col = 1;
    return t;
}
