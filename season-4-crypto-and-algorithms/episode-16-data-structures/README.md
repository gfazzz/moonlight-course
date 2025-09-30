# Episode 16: Data Structures 📦

> "Bad programmers worry about the code. Good programmers worry about data structures and their relationships." — Linus Torvalds

## 📋 Описание

После анализа и сортировки логов в Episode 15, **V.** требует создать эффективную систему хранения и быстрого поиска данных операции MOONLIGHT. Простых массивов недостаточно — нужны динамические структуры данных.

**В этом эпизоде вы научитесь:**
- Реализовывать linked lists (односвязные и двусвязные)
- Создавать stacks и queues
- Работать с binary trees
- Применять hash tables для быстрого поиска

---

## 🎯 Цели обучения

### Теория
- **Linked Lists:** односвязные, двусвязные, циклические
- **Stacks & Queues:** LIFO, FIFO, реализация через массивы и списки
- **Trees:** binary trees, BST (Binary Search Tree)
- **Hash Tables:** хеш-функции, collision resolution

### Практика
- Динамическое управление памятью
- Реализация основных операций (insert, delete, search)
- Выбор правильной структуры для задачи
- Анализ сложности операций

---

## 📖 Теоретическая часть

### 1. Linked List

**Односвязный список:**
```c
typedef struct Node {
    int data;
    struct Node *next;
} Node;

// Добавление в начало - O(1)
void push_front(Node **head, int data) {
    Node *new_node = malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = *head;
    *head = new_node;
}
```

**Двусвязный список:**
```c
typedef struct DNode {
    int data;
    struct DNode *prev;
    struct DNode *next;
} DNode;
```

**Сравнение:**
| Операция | Массив | Linked List |
|----------|--------|-------------|
| Доступ по индексу | O(1) | O(n) |
| Вставка в начало | O(n) | O(1) |
| Вставка в конец | O(1) | O(1)* |
| Удаление | O(n) | O(1)* |

*с указателем на элемент

### 2. Stack (Стек) — LIFO

```c
typedef struct Stack {
    int *data;
    int top;
    int capacity;
} Stack;

void push(Stack *s, int value) {
    if (s->top < s->capacity - 1) {
        s->data[++s->top] = value;
    }
}

int pop(Stack *s) {
    if (s->top >= 0) {
        return s->data[s->top--];
    }
    return -1;  // Error
}
```

**Применение:** undo/redo, вычисление выражений, обход графов

### 3. Queue (Очередь) — FIFO

```c
typedef struct Queue {
    int *data;
    int front;
    int rear;
    int size;
    int capacity;
} Queue;

void enqueue(Queue *q, int value);
int dequeue(Queue *q);
```

**Применение:** обработка задач, BFS, планировщики

### 4. Binary Search Tree

```c
typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode* insert_bst(TreeNode *root, int data) {
    if (root == NULL) {
        TreeNode *node = malloc(sizeof(TreeNode));
        node->data = data;
        node->left = node->right = NULL;
        return node;
    }
    
    if (data < root->data)
        root->left = insert_bst(root->left, data);
    else
        root->right = insert_bst(root->right, data);
    
    return root;
}
```

**Сложность:** O(log n) average, O(n) worst case

---

## 💻 Задания

### Задание 1: Linked List for Log Storage ⭐⭐⭐☆☆

Реализуйте linked list для хранения логов операции:

```c
typedef struct LogEntry {
    time_t timestamp;
    char message[256];
    int priority;
    struct LogEntry *next;
} LogEntry;

// TODO: Реализовать
void add_log(LogEntry **head, time_t ts, const char *msg, int priority);
LogEntry* find_log_by_time(LogEntry *head, time_t ts);
void delete_log(LogEntry **head, time_t ts);
void print_logs(LogEntry *head);
```

### Задание 2: Stack for Command History ⭐⭐⭐☆☆

Создайте stack для истории команд с undo:

```c
typedef struct {
    char **commands;
    int top;
    int capacity;
} CommandStack;

// TODO: Реализовать
void push_command(CommandStack *s, const char *cmd);
char* pop_command(CommandStack *s);  // Undo
char* peek_command(CommandStack *s);
```

### Задание 3: Queue for Task Processing ⭐⭐⭐⭐☆

Реализуйте queue для обработки задач операции:

```c
typedef struct Task {
    int id;
    char description[128];
    int priority;
} Task;

// TODO: Circular queue
void enqueue_task(Task queue[], int *front, int *rear, Task t);
Task dequeue_task(Task queue[], int *front, int *rear);
```

### Задание 4: Binary Search Tree for Fast Lookup ⭐⭐⭐⭐⭐

Постройте BST для быстрого поиска по ID:

```c
typedef struct Agent {
    int id;
    char name[64];
    char status[32];
} Agent;

// TODO: BST для агентов
TreeNode* insert_agent(TreeNode *root, Agent agent);
Agent* search_agent(TreeNode *root, int id);
void print_agents_inorder(TreeNode *root);
```

---

## 🎬 Сюжет

**Дата:** 17 декабря, 18:30  
**Локация:** Штаб операции MOONLIGHT

> **V.:** "У нас тысячи агентов, миллионы логов, сотни активных задач. Массивы не справляются. Нужна система — добавлять, удалять, искать за секунды. Построй архитектуру данных."

**Данные:**
- agents.dat — 10,000 записей агентов
- logs.txt — 1,000,000 логов
- tasks.dat — 500 активных задач

**Миссия:** Создать эффективную систему управления данными операции.

---

## 🏆 Бонусные задания

### Bonus 1: Hash Table ⭐⭐⭐⭐⭐
Реализуйте hash table для O(1) поиска агентов по ID.

### Bonus 2: Doubly Linked List ⭐⭐⭐☆☆
Двусвязный список для навигации вперед-назад по истории.

### Bonus 3: AVL Tree ⭐⭐⭐⭐⭐
Самобалансирующееся дерево для гарантированного O(log n).

---

## 📊 Что вы узнали

После Episode 16 вы умеете:

- ✅ Создавать и управлять linked lists
- ✅ Реализовывать stacks и queues
- ✅ Работать с binary trees
- ✅ Выбирать правильную структуру данных для задачи
- ✅ Анализировать сложность операций

---

## 🔜 Следующий сезон

В **Season 5: Financial Markets** вы примените все полученные знания для анализа финансовых рынков:
- Торговые алгоритмы
- Risk management
- Криптовалюты и DeFi

Архитектура данных готова. Пора применить её к реальным финансовым данным организации...

---

<div align="center">

**MOONLIGHT OPERATION**  
*Episode 16 of 32*

[← Episode 15](../episode-15-sorting-search/) | [Season 4](../README.md) | [Season 5 →](../../season-5-financial-markets/)

</div>
