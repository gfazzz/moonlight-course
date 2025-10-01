// Episode 16: Data Structures - SOLUTION
// Complete implementation of essential data structures

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

void push_front(Node **head, int data) {
    Node *new_node = malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = *head;
    *head = new_node;
}

void push_back(Node **head, int data) {
    Node *new_node = malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;
    
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

Node* find(Node *head, int data) {
    Node *current = head;
    while (current != NULL) {
        if (current->data == data) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void delete_node(Node **head, int data) {
    if (*head == NULL) return;
    
    if ((*head)->data == data) {
        Node *temp = *head;
        *head = (*head)->next;
        free(temp);
        return;
    }
    
    Node *current = *head;
    while (current->next != NULL && current->next->data != data) {
        current = current->next;
    }
    
    if (current->next != NULL) {
        Node *temp = current->next;
        current->next = current->next->next;
        free(temp);
    }
}

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

void free_list(Node **head) {
    Node *current = *head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

// ============================================================
// 2. STACK (LIFO)
// ============================================================

typedef struct Stack {
    int *data;
    int top;
    int capacity;
} Stack;

Stack* create_stack(int capacity) {
    Stack *s = malloc(sizeof(Stack));
    s->capacity = capacity;
    s->top = -1;
    s->data = malloc(capacity * sizeof(int));
    return s;
}

void push(Stack *s, int value) {
    if (s->top >= s->capacity - 1) {
        fprintf(stderr, "Stack overflow!\n");
        return;
    }
    s->data[++s->top] = value;
}

int pop(Stack *s) {
    if (s->top < 0) {
        fprintf(stderr, "Stack underflow!\n");
        return -1;
    }
    return s->data[s->top--];
}

int peek(Stack *s) {
    if (s->top < 0) {
        return -1;
    }
    return s->data[s->top];
}

int is_empty(Stack *s) {
    return s->top < 0;
}

void free_stack(Stack *s) {
    free(s->data);
    free(s);
}

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

Queue* create_queue(int capacity) {
    Queue *q = malloc(sizeof(Queue));
    q->capacity = capacity;
    q->front = 0;
    q->rear = -1;
    q->size = 0;
    q->data = malloc(capacity * sizeof(int));
    return q;
}

void enqueue(Queue *q, int value) {
    if (q->size >= q->capacity) {
        fprintf(stderr, "Queue overflow!\n");
        return;
    }
    q->rear = (q->rear + 1) % q->capacity;
    q->data[q->rear] = value;
    q->size++;
}

int dequeue(Queue *q) {
    if (q->size == 0) {
        fprintf(stderr, "Queue underflow!\n");
        return -1;
    }
    int value = q->data[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return value;
}

int queue_is_empty(Queue *q) {
    return q->size == 0;
}

void free_queue(Queue *q) {
    free(q->data);
    free(q);
}

// ============================================================
// 4. BINARY SEARCH TREE
// ============================================================

typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode* create_tree_node(int data) {
    TreeNode *node = malloc(sizeof(TreeNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

TreeNode* insert_bst(TreeNode *root, int data) {
    if (root == NULL) {
        return create_tree_node(data);
    }
    
    if (data < root->data) {
        root->left = insert_bst(root->left, data);
    } else if (data > root->data) {
        root->right = insert_bst(root->right, data);
    }
    
    return root;
}

TreeNode* search_bst(TreeNode *root, int data) {
    if (root == NULL || root->data == data) {
        return root;
    }
    
    if (data < root->data) {
        return search_bst(root->left, data);
    }
    
    return search_bst(root->right, data);
}

TreeNode* find_min(TreeNode *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

TreeNode* delete_bst(TreeNode *root, int data) {
    if (root == NULL) {
        return NULL;
    }
    
    if (data < root->data) {
        root->left = delete_bst(root->left, data);
    } else if (data > root->data) {
        root->right = delete_bst(root->right, data);
    } else {
        // Node found
        if (root->left == NULL) {
            TreeNode *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            TreeNode *temp = root->left;
            free(root);
            return temp;
        }
        
        // Node with two children
        TreeNode *temp = find_min(root->right);
        root->data = temp->data;
        root->right = delete_bst(root->right, temp->data);
    }
    
    return root;
}

void inorder_traversal(TreeNode *root) {
    if (root != NULL) {
        inorder_traversal(root->left);
        printf("%d ", root->data);
        inorder_traversal(root->right);
    }
}

void free_tree(TreeNode *root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

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

HashTable* create_hash_table() {
    HashTable *ht = malloc(sizeof(HashTable));
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->buckets[i] = NULL;
    }
    ht->count = 0;
    return ht;
}

void hash_insert(HashTable *ht, const char *key, void *value) {
    unsigned long index = hash_function(key);
    
    // Check if key exists
    HashNode *current = ht->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;  // Update existing
            return;
        }
        current = current->next;
    }
    
    // Insert new node
    HashNode *new_node = malloc(sizeof(HashNode));
    strcpy(new_node->key, key);
    new_node->value = value;
    new_node->next = ht->buckets[index];
    ht->buckets[index] = new_node;
    ht->count++;
}

void* hash_search(HashTable *ht, const char *key) {
    unsigned long index = hash_function(key);
    HashNode *current = ht->buckets[index];
    
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    
    return NULL;
}

void hash_delete(HashTable *ht, const char *key) {
    unsigned long index = hash_function(key);
    HashNode *current = ht->buckets[index];
    HashNode *prev = NULL;
    
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                ht->buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            ht->count--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

float get_load_factor(HashTable *ht) {
    return (float)ht->count / TABLE_SIZE;
}

void free_hash_table(HashTable *ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *current = ht->buckets[i];
        while (current != NULL) {
            HashNode *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(ht);
}

// ============================================================
// 6. AVL TREE (BONUS)
// ============================================================

typedef struct AVLNode {
    int data;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int get_height(AVLNode *node) {
    if (node == NULL) return 0;
    return node->height;
}

int get_balance_factor(AVLNode *node) {
    if (node == NULL) return 0;
    return get_height(node->left) - get_height(node->right);
}

AVLNode* create_avl_node(int data) {
    AVLNode *node = malloc(sizeof(AVLNode));
    node->data = data;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

AVLNode* rotate_right(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    
    return x;
}

AVLNode* rotate_left(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    
    return y;
}

AVLNode* insert_avl(AVLNode *node, int data) {
    if (node == NULL) {
        return create_avl_node(data);
    }
    
    if (data < node->data) {
        node->left = insert_avl(node->left, data);
    } else if (data > node->data) {
        node->right = insert_avl(node->right, data);
    } else {
        return node;  // Duplicate
    }
    
    node->height = 1 + max(get_height(node->left), get_height(node->right));
    
    int balance = get_balance_factor(node);
    
    // Left-Left case
    if (balance > 1 && data < node->left->data) {
        return rotate_right(node);
    }
    
    // Right-Right case
    if (balance < -1 && data > node->right->data) {
        return rotate_left(node);
    }
    
    // Left-Right case
    if (balance > 1 && data > node->left->data) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    
    // Right-Left case
    if (balance < -1 && data < node->right->data) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }
    
    return node;
}

void free_avl(AVLNode *root) {
    if (root != NULL) {
        free_avl(root->left);
        free_avl(root->right);
        free(root);
    }
}

// ============================================================
// MAIN - DEMONSTRATION
// ============================================================

int main() {
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║  Episode 16: Data Structures                 ║\n");
    printf("║  MOONLIGHT Database Engine                   ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");
    
    // ===== LINKED LIST DEMO =====
    printf("=== LINKED LIST DEMO ===\n");
    Node *list = NULL;
    push_back(&list, 10);
    push_back(&list, 20);
    push_front(&list, 5);
    push_back(&list, 30);
    
    printf("List: ");
    print_list(list);
    
    printf("Finding 20: %s\n", find(list, 20) ? "Found" : "Not found");
    
    delete_node(&list, 20);
    printf("After deleting 20: ");
    print_list(list);
    
    free_list(&list);
    printf("\n");
    
    // ===== STACK DEMO =====
    printf("=== STACK DEMO ===\n");
    Stack *stack = create_stack(10);
    
    push(stack, 1);
    push(stack, 2);
    push(stack, 3);
    printf("Pushed: 1, 2, 3\n");
    
    printf("Pop: %d\n", pop(stack));
    printf("Peek: %d\n", peek(stack));
    printf("Pop: %d\n", pop(stack));
    
    free_stack(stack);
    printf("\n");
    
    // ===== QUEUE DEMO =====
    printf("=== QUEUE DEMO ===\n");
    Queue *queue = create_queue(10);
    
    enqueue(queue, 10);
    enqueue(queue, 20);
    enqueue(queue, 30);
    printf("Enqueued: 10, 20, 30\n");
    
    printf("Dequeue: %d\n", dequeue(queue));
    printf("Dequeue: %d\n", dequeue(queue));
    
    free_queue(queue);
    printf("\n");
    
    // ===== BST DEMO =====
    printf("=== BINARY SEARCH TREE DEMO ===\n");
    TreeNode *bst = NULL;
    
    bst = insert_bst(bst, 50);
    bst = insert_bst(bst, 30);
    bst = insert_bst(bst, 70);
    bst = insert_bst(bst, 20);
    bst = insert_bst(bst, 40);
    bst = insert_bst(bst, 60);
    bst = insert_bst(bst, 80);
    
    printf("Inorder traversal: ");
    inorder_traversal(bst);
    printf("\n");
    
    printf("Search 40: %s\n", search_bst(bst, 40) ? "Found" : "Not found");
    
    bst = delete_bst(bst, 30);
    printf("After deleting 30: ");
    inorder_traversal(bst);
    printf("\n");
    
    free_tree(bst);
    printf("\n");
    
    // ===== HASH TABLE DEMO =====
    printf("=== HASH TABLE DEMO ===\n");
    HashTable *ht = create_hash_table();
    
    int val1 = 100, val2 = 200, val3 = 300;
    hash_insert(ht, "MOONLIGHT", &val1);
    hash_insert(ht, "AGENT_V", &val2);
    hash_insert(ht, "OPERATION", &val3);
    
    printf("Inserted 3 key-value pairs\n");
    printf("Count: %d\n", ht->count);
    printf("Load factor: %.4f\n", get_load_factor(ht));
    
    int *found = (int*)hash_search(ht, "AGENT_V");
    printf("Search 'AGENT_V': %s (value: %d)\n", found ? "Found" : "Not found", found ? *found : 0);
    
    hash_delete(ht, "AGENT_V");
    printf("After deleting 'AGENT_V', count: %d\n", ht->count);
    
    free_hash_table(ht);
    printf("\n");
    
    // ===== AVL TREE DEMO =====
    printf("=== AVL TREE DEMO (Self-Balancing) ===\n");
    AVLNode *avl = NULL;
    
    avl = insert_avl(avl, 10);
    avl = insert_avl(avl, 20);
    avl = insert_avl(avl, 30);
    avl = insert_avl(avl, 40);
    avl = insert_avl(avl, 50);
    
    printf("AVL tree root: %d (height: %d)\n", avl->data, avl->height);
    printf("Balance factor: %d\n", get_balance_factor(avl));
    
    free_avl(avl);
    printf("\n");
    
    printf("✓ All data structures tested successfully!\n");
    printf("✓ SEASON 4 COMPLETE!\n");
    
    return 0;
}

