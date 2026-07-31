/*
 * OPERATION MOONLIGHT — s09e01 "Лексер"
 * Эталонное решение: lexer.c
 *
 * Концепт серии: первый этап любого компилятора — превратить поток СИМВОЛОВ
 *                в поток ТОКЕНОВ. Лексер не понимает структуру программы, он
 *                лишь распознаёт «слова»: числа, имена, операторы, скобки.
 *                Ключевые приёмы: максимальное совпадение (longest match),
 *                просмотр вперёд (lookahead) для двухсимвольных операторов,
 *                отделение ключевых слов от идентификаторов уже после чтения.
 * Задача: лексер для языка MOON — арифметика, сравнения, присваивание, if/while.
 *
 * Позиции (строка/столбец) сохраняются в каждом токене: без них компилятор
 * не сможет показать, ГДЕ ошибка.
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
    long    value;      /* для T_NUM */
    int     line, col;
} Token;

static const char *KEYWORDS[] = {"if", "else", "while", "print", NULL};

typedef struct {
    const char *src;
    size_t pos;
    int line, col;
} Lexer;

static void lex_init(Lexer *lx, const char *src) {
    lx->src = src; lx->pos = 0; lx->line = 1; lx->col = 1;
}

static char peek(const Lexer *lx)  { return lx->src[lx->pos]; }
static char peek2(const Lexer *lx) { return peek(lx) ? lx->src[lx->pos + 1] : '\0'; }

static char advance(Lexer *lx) {
    char c = lx->src[lx->pos++];
    if (c == '\n') { lx->line++; lx->col = 1; }
    else lx->col++;
    return c;
}

static void skip_space_and_comments(Lexer *lx) {
    for (;;) {
        char c = peek(lx);
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') { advance(lx); continue; }
        if (c == '/' && peek2(lx) == '/') {                  /* комментарий до конца строки */
            while (peek(lx) && peek(lx) != '\n') advance(lx);
            continue;
        }
        return;
    }
}

static int is_keyword(const char *s) {
    for (int i = 0; KEYWORDS[i]; i++) if (strcmp(s, KEYWORDS[i]) == 0) return 1;
    return 0;
}

/* Одиночный/двойной оператор: сначала пробуем длинный вариант (longest match). */
static int two_char_op(char a, char b, TokType *t) {
    if (a == '=' && b == '=') { *t = T_EQ; return 1; }
    if (a == '!' && b == '=') { *t = T_NE; return 1; }
    if (a == '<' && b == '=') { *t = T_LE; return 1; }
    if (a == '>' && b == '=') { *t = T_GE; return 1; }
    return 0;
}

static Token next_token(Lexer *lx) {
    skip_space_and_comments(lx);

    Token t;
    memset(&t, 0, sizeof t);
    t.line = lx->line;
    t.col  = lx->col;

    char c = peek(lx);
    if (c == '\0') { t.type = T_EOF; strcpy(t.text, "<eof>"); return t; }

    /* Число: максимальное совпадение по цифрам. */
    if (isdigit((unsigned char)c)) {
        size_t n = 0;
        while (isdigit((unsigned char)peek(lx)) && n < sizeof t.text - 1) {
            char d = advance(lx);
            t.text[n++] = d;
            t.value = t.value * 10 + (d - '0');
        }
        t.type = T_NUM;
        return t;
    }

    /* Идентификатор или ключевое слово: сначала читаем слово, потом решаем. */
    if (isalpha((unsigned char)c) || c == '_') {
        size_t n = 0;
        while ((isalnum((unsigned char)peek(lx)) || peek(lx) == '_') && n < sizeof t.text - 1)
            t.text[n++] = advance(lx);
        t.type = is_keyword(t.text) ? T_KEYWORD : T_IDENT;
        return t;
    }

    /* Двухсимвольные операторы — до одиночных. */
    TokType two;
    if (two_char_op(c, peek2(lx), &two)) {
        t.text[0] = advance(lx);
        t.text[1] = advance(lx);
        t.type = two;
        return t;
    }

    /* Одиночные символы. */
    t.text[0] = advance(lx);
    switch (t.text[0]) {
        case '+': t.type = T_PLUS;   break;
        case '-': t.type = T_MINUS;  break;
        case '*': t.type = T_STAR;   break;
        case '/': t.type = T_SLASH;  break;
        case '=': t.type = T_ASSIGN; break;
        case '<': t.type = T_LT;     break;
        case '>': t.type = T_GT;     break;
        case '(': t.type = T_LPAREN; break;
        case ')': t.type = T_RPAREN; break;
        case '{': t.type = T_LBRACE; break;
        case '}': t.type = T_RBRACE; break;
        case ';': t.type = T_SEMI;   break;
        default:  t.type = T_ERROR;  break;
    }
    return t;
}

int main(void) {
    const char *program =
        "// счётчик угроз\n"
        "x = 12;\n"
        "if (x >= 10) {\n"
        "  threat = x * 2 + 1;\n"
        "  print threat;\n"
        "}\n"
        "while (x != 0) { x = x - 1; }\n"
        "y = 3 @ 4;\n";          /* '@' — намеренно неизвестный символ */

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
