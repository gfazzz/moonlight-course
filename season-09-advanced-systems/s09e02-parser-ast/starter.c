/*
 * OPERATION MOONLIGHT — s09e02 "Парсер и AST"
 * Каркас. Скопируй в artifacts/parser.c и реализуй.
 *
 *   cp starter.c artifacts/parser.c
 *   make test
 *
 * Подсказок мало (Season 9). Грамматика и требования — в mission.md,
 * точный формат вывода — в tests/expected.txt.
 *
 * Грамматика (по возрастанию силы связывания):
 *   expr   := term   (('+' | '-') term)*      левоассоциативно
 *   term   := factor (('*' | '/') factor)*    левоассоциативно
 *   factor := NUM | '(' expr ')' | '-' factor
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* --- лексер дан готовым: сосредоточься на разборе --- */
typedef enum { L_NUM, L_PLUS, L_MINUS, L_STAR, L_SLASH, L_LP, L_RP, L_END, L_BAD } LTok;
typedef struct { const char *s; size_t i; LTok tok; long num; } Lex;

static void lex_next(Lex *lx) {
    while (lx->s[lx->i] == ' ') lx->i++;
    char c = lx->s[lx->i];
    if (!c) { lx->tok = L_END; return; }
    if (isdigit((unsigned char)c)) {
        long v = 0;
        while (isdigit((unsigned char)lx->s[lx->i])) v = v * 10 + (lx->s[lx->i++] - '0');
        lx->num = v; lx->tok = L_NUM; return;
    }
    lx->i++;
    switch (c) {
        case '+': lx->tok = L_PLUS;  break;
        case '-': lx->tok = L_MINUS; break;
        case '*': lx->tok = L_STAR;  break;
        case '/': lx->tok = L_SLASH; break;
        case '(': lx->tok = L_LP;    break;
        case ')': lx->tok = L_RP;    break;
        default:  lx->tok = L_BAD;   break;
    }
}

/* --- обязательный интерфейс --- */
typedef enum { N_NUM, N_ADD, N_SUB, N_MUL, N_DIV, N_NEG } NodeKind;

typedef struct Node {
    NodeKind kind;
    long value;
    struct Node *l, *r;
} Node;

#define POOL 256
static Node pool[POOL];          /* узлы берём из пула: никакого malloc */
static int pool_used = 0;
static int parse_error = 0;

/* Реализуй: */
static Node *parse_expr(Lex *lx);      /* + и - */
static Node *parse_term(Lex *lx);      /* * и / */
static Node *parse_factor(Lex *lx);    /* число, скобки, унарный минус */
static void  print_sexpr(const Node *n);   /* (+ 3 (* 4 2)) */
static void  print_rpn(const Node *n);     /* 3 4 2 * +  (с пробелом после каждого) */
static int   depth(const Node *n);
static long  eval(const Node *n);          /* деление на 0 -> 0 */

static void run(const char *src) {
    pool_used = 0;
    parse_error = 0;

    Lex lx = { src, 0, L_END, 0 };
    lex_next(&lx);
    Node *root = parse_expr(&lx);
    if (lx.tok != L_END) parse_error = 1;

    printf("вход:   %s\n", src);
    if (parse_error) { printf("  ошибка разбора\n\n"); return; }
    printf("  AST:  "); print_sexpr(root); printf("\n");
    printf("  RPN:  "); print_rpn(root);   printf("\n");
    printf("  глубина: %d, узлов: %d, значение: %ld\n\n", depth(root), pool_used, eval(root));
}

int main(void) {
    printf("=== парсер: токены -> AST ===\n\n");

    run("3 + 4 * 2");
    run("(3 + 4) * 2");
    run("100 - 20 - 5");
    run("-8 + 3 * -2");
    run("2 * (3 + 4) * 5");
    run("7 + ");
    run("(1 + 2");

    printf("структура дерева — это и есть приоритет операций.\n");
    return 0;
}

/* --- Твоя реализация --- */
static Node *parse_expr(Lex *lx)   { (void)lx; parse_error = 1; return NULL; }
static Node *parse_term(Lex *lx)   { (void)lx; return NULL; }
static Node *parse_factor(Lex *lx) { (void)lx; return NULL; }
static void print_sexpr(const Node *n) { (void)n; }
static void print_rpn(const Node *n)   { (void)n; }
static int  depth(const Node *n)       { (void)n; return 0; }
static long eval(const Node *n)        { (void)n; return 0; }
