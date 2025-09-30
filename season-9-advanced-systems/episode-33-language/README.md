# Episode 33: "Building a Language" 🔤
## Season 9: Advanced Systems | Episode 33/42

> *"Create your own language. Bend reality to your will."*

---

## 📋 Briefing

Построить собственный язык программирования: lexer, parser, interpreter.

**Задачи:**
1. Tokenization (lexer)
2. AST building (parser)
3. Interpreter/Eval
4. Garbage collection

---

## 📚 Теория

### Lexer (Tokenizer)

```c
typedef enum {
    TOKEN_NUMBER, TOKEN_PLUS, TOKEN_MINUS, 
    TOKEN_MUL, TOKEN_DIV, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    double value;
} Token;

Token next_token(const char **input) {
    while (isspace(**input)) (*input)++;
    
    if (isdigit(**input)) {
        double val = 0;
        while (isdigit(**input)) {
            val = val * 10 + (**input - '0');
            (*input)++;
        }
        return (Token){TOKEN_NUMBER, val};
    }
    
    if (**input == '+') { (*input)++; return (Token){TOKEN_PLUS, 0}; }
    // ... other tokens
}
```

---

## 🛠 Практика

**Задачи:**
1. Calculator language (arithmetic)
2. Variable support
3. Functions/procedures
4. Simple GC

---

**Next:** [Episode 34: Database from Scratch →](../episode-34-database/)
