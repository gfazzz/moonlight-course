// Episode 16: Data Structures - Starter Templates
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============================================================
// 1. LINKED LIST
// ============================================================

typedef struct Node {
    int data;
    struct Node *next;
} Node;

// TODO: Реализовать функции
void push_front(Node **head, int data) {}
void push_back(Node **head, int data) {}
Node* find(Node *head, int data) { return NULL; }
void delete_node(Node **head, int data) {}
void print_list(Node *head) {}
void free_list(Node **head) {}

// ============================================================
// 2. STACK (LIFO)
// ============================================================

typedef struct Stack {
    int *data;
    int top;
    int capacity;
} Stack;

// TODO: Реализовать функции
Stack* create_stack(int capacity) { return NULL; }
void push(Stack *s, int value) {}
int pop(Stack *s) { return -1; }
int peek(Stack *s) { return -1; }
int is_empty(Stack *s) { return 1; }
void free_stack(Stack *s) {}

// ============================================================
// 3. QUEUE (FIFO)
// ============================================================

typedef struct Queue {
    int *data;
    int front;
    int rear;
    int size;
    int capacity;
} Queue;

// TODO: Реализовать функции
Queue* create_queue(int capacity) { return NULL; }
void enqueue(Queue *q, int value) {}
int dequeue(Queue *q) { return -1; }
int queue_is_empty(Queue *q) { return 1; }
void free_queue(Queue *q) {}

// ============================================================
// 4. BINARY SEARCH TREE
// ============================================================

typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// TODO: Реализовать функции
TreeNode* insert_bst(TreeNode *root, int data) { return NULL; }
TreeNode* search_bst(TreeNode *root, int data) { return NULL; }
TreeNode* delete_bst(TreeNode *root, int data) { return NULL; }
void inorder_traversal(TreeNode *root) {}
void free_tree(TreeNode *root) {}

// ============================================================
// 5. HASH TABLE
// ============================================================

#define TABLE_SIZE 1000

typedef struct HashNode {
    char key[64];
    void *value;
    struct HashNode *next;
} HashNode;

typedef struct HashTable {
    HashNode *buckets[TABLE_SIZE];
    int count;
} HashTable;

// Hash function (djb2)
unsigned long hash_function(const char *key) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TABLE_SIZE;
}

// TODO: Реализовать функции
HashTable* create_hash_table() { return NULL; }
void hash_insert(HashTable *ht, const char *key, void *value) {}
void* hash_search(HashTable *ht, const char *key) { return NULL; }
void hash_delete(HashTable *ht, const char *key) {}
float get_load_factor(HashTable *ht) { return 0.0; }
void free_hash_table(HashTable *ht) {}

// ============================================================
// 6. AVL TREE (BONUS)
// ============================================================

typedef struct AVLNode {
    int data;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

// TODO: Реализовать функции
int get_height(AVLNode *node) { return 0; }
int get_balance_factor(AVLNode *node) { return 0; }
AVLNode* rotate_left(AVLNode *x) { return NULL; }
AVLNode* rotate_right(AVLNode *y) { return NULL; }
AVLNode* insert_avl(AVLNode *root, int data) { return NULL; }
void free_avl(AVLNode *root) {}

// ============================================================
// MAIN - ДЕМОНСТРАЦИЯ
// ============================================================

int main() {
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║  Episode 16: Data Structures                 ║\n");
    printf("║  MOONLIGHT Database Engine                   ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");
    
    printf("--- LINKED LIST DEMO ---\n");
    printf("TODO: Реализовать linked list операции\n\n");
    
    printf("--- STACK DEMO ---\n");
    printf("TODO: Реализовать stack операции\n\n");
    
    printf("--- QUEUE DEMO ---\n");
    printf("TODO: Реализовать queue операции\n\n");
    
    printf("--- BST DEMO ---\n");
    printf("TODO: Реализовать binary search tree\n\n");
    
    printf("--- HASH TABLE DEMO ---\n");
    printf("TODO: Реализовать hash table\n\n");
    
    printf("--- AVL TREE DEMO (BONUS) ---\n");
    printf("TODO: Реализовать AVL tree с балансировкой\n\n");
    
    return 0;
}
