/*
 * OPERATION MOONLIGHT — s09e02 "Парсер и AST"
 * Эталонное решение: parser.c
 *
 * Концепт серии: токены (s09e01) — плоский список. Смысл задаёт СТРУКТУРА:
 *                дерево разбора (AST), где вложенность отражает приоритет и
 *                ассоциативность операторов. Метод рекурсивного спуска: каждому
 *                уровню приоритета — своя функция, вызывающая уровень выше.
 *                Грамматика (снизу вверх по силе связывания):
 *                  expr   := term   (('+' | '-') term)*
 *                  term   := factor (('*' | '/') factor)*
 *                  factor := NUM | '(' expr ')' | '-' factor
 *                Левая рекурсия в цикле, а не в рекурсии — иначе бесконечный спуск.
 * Задача: построить AST для выражений и напечатать его тремя способами.
 *
 * Память: узлы в статическом пуле — без malloc, чтобы не отвлекаться на
 * освобождение дерева (в реальном компиляторе это арена с одним free).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ---------- лексер (сокращённый, из s09e01) ---------- */
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

/* ---------- AST ---------- */
typedef enum { N_NUM, N_ADD, N_SUB, N_MUL, N_DIV, N_NEG } NodeKind;

typedef struct Node {
    NodeKind kind;
    long value;                 /* для N_NUM */
    struct Node *l, *r;         /* для N_NEG используется только l */
} Node;

#define POOL 256
static Node pool[POOL];
static int pool_used = 0;
static int parse_error = 0;

static Node *node_new(NodeKind k, Node *l, Node *r) {
    if (pool_used >= POOL) { parse_error = 1; return NULL; }
    Node *n = &pool[pool_used++];
    n->kind = k; n->l = l; n->r = r; n->value = 0;
    return n;
}
static Node *node_num(long v) {
    Node *n = node_new(N_NUM, NULL, NULL);
    if (n) n->value = v;
    return n;
}

/* ---------- рекурсивный спуск ---------- */
static Node *parse_expr(Lex *lx);

static Node *parse_factor(Lex *lx) {
    if (lx->tok == L_MINUS) {                 /* унарный минус: правоассоциативен */
        lex_next(lx);
        return node_new(N_NEG, parse_factor(lx), NULL);
    }
    if (lx->tok == L_NUM) {
        Node *n = node_num(lx->num);
        lex_next(lx);
        return n;
    }
    if (lx->tok == L_LP) {
        lex_next(lx);
        Node *n = parse_expr(lx);
        if (lx->tok != L_RP) { parse_error = 1; return n; }   /* нет закрывающей скобки */
        lex_next(lx);
        return n;
    }
    parse_error = 1;
    return node_num(0);
}

static Node *parse_term(Lex *lx) {            /* * и / — левоассоциативны */
    Node *n = parse_factor(lx);
    for (;;) {
        if (lx->tok == L_STAR)       { lex_next(lx); n = node_new(N_MUL, n, parse_factor(lx)); }
        else if (lx->tok == L_SLASH) { lex_next(lx); n = node_new(N_DIV, n, parse_factor(lx)); }
        else return n;
    }
}

static Node *parse_expr(Lex *lx) {            /* + и - — левоассоциативны */
    Node *n = parse_term(lx);
    for (;;) {
        if (lx->tok == L_PLUS)       { lex_next(lx); n = node_new(N_ADD, n, parse_term(lx)); }
        else if (lx->tok == L_MINUS) { lex_next(lx); n = node_new(N_SUB, n, parse_term(lx)); }
        else return n;
    }
}

/* ---------- обходы дерева ---------- */
static const char *op_str(NodeKind k) {
    switch (k) {
        case N_ADD: return "+";
        case N_SUB: return "-";
        case N_MUL: return "*";
        case N_DIV: return "/";
        case N_NEG: return "neg";
        default:    return "?";
    }
}

/* Скобочная запись: показывает структуру однозначно. */
static void print_sexpr(const Node *n) {
    if (!n) { printf("<null>"); return; }
    if (n->kind == N_NUM) { printf("%ld", n->value); return; }
    if (n->kind == N_NEG) { printf("(neg "); print_sexpr(n->l); printf(")"); return; }
    printf("(%s ", op_str(n->kind));
    print_sexpr(n->l);
    printf(" ");
    print_sexpr(n->r);
    printf(")");
}

/* Обратная польская запись = постфиксный обход. */
static void print_rpn(const Node *n) {
    if (!n) return;
    if (n->kind == N_NUM) { printf("%ld ", n->value); return; }
    print_rpn(n->l);
    print_rpn(n->r);
    printf("%s ", op_str(n->kind));
}

static int depth(const Node *n) {
    if (!n) return 0;
    int a = depth(n->l), b = depth(n->r);
    return 1 + (a > b ? a : b);
}

/* Вычисление — предварительный просмотр s09e03. */
static long eval(const Node *n) {
    if (!n) return 0;
    switch (n->kind) {
        case N_NUM: return n->value;
        case N_NEG: return -eval(n->l);
        case N_ADD: return eval(n->l) + eval(n->r);
        case N_SUB: return eval(n->l) - eval(n->r);
        case N_MUL: return eval(n->l) * eval(n->r);
        case N_DIV: { long d = eval(n->r); return d ? eval(n->l) / d : 0; }
    }
    return 0;
}

static void run(const char *src) {
    pool_used = 0;
    parse_error = 0;

    Lex lx = { src, 0, L_END, 0 };
    lex_next(&lx);
    Node *root = parse_expr(&lx);
    if (lx.tok != L_END) parse_error = 1;      /* остался мусор после выражения */

    printf("вход:   %s\n", src);
    if (parse_error) {
        printf("  ошибка разбора\n\n");
        return;
    }
    printf("  AST:  "); print_sexpr(root); printf("\n");
    printf("  RPN:  "); print_rpn(root);   printf("\n");
    printf("  глубина: %d, узлов: %d, значение: %ld\n\n", depth(root), pool_used, eval(root));
}

int main(void) {
    printf("=== парсер: токены -> AST ===\n\n");

    run("3 + 4 * 2");            /* приоритет: умножение связывает сильнее */
    run("(3 + 4) * 2");          /* скобки меняют структуру */
    run("100 - 20 - 5");         /* левая ассоциативность: (100-20)-5, не 100-(20-5) */
    run("-8 + 3 * -2");          /* унарный минус */
    run("2 * (3 + 4) * 5");
    run("7 + ");                 /* ошибка: нет правого операнда */
    run("(1 + 2");               /* ошибка: не закрыта скобка */

    printf("структура дерева — это и есть приоритет операций.\n");
    return 0;
}
