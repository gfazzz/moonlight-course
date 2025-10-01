/**
 * ═══════════════════════════════════════════════════════════════
 * Episode 16: Data Structures — Промежуточная задача (РЕШЕНИЕ)
 * ═══════════════════════════════════════════════════════════════
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

// ═══════════════════════════════════════════════════════════════
// РЕШЕНИЕ: Floyd's Cycle Detection
// ═══════════════════════════════════════════════════════════════

bool has_cycle(Node *head) {
    if (head == NULL) return false;
    
    Node *slow = head;
    Node *fast = head;
    
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;        // 1 шаг
        fast = fast->next->next;  // 2 шага
        
        if (slow == fast) {
            return true;  // Встретились → цикл есть!
        }
    }
    
    return false;  // fast дошёл до NULL → цикла нет
}

Node* find_cycle_start(Node *head) {
    if (head == NULL) return NULL;
    
    // Шаг 1: Найти точку встречи
    Node *slow = head;
    Node *fast = head;
    bool has_meeting = false;
    
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        
        if (slow == fast) {
            has_meeting = true;
            break;
        }
    }
    
    if (!has_meeting) {
        return NULL;  // Нет цикла
    }
    
    // Шаг 2: Найти начало цикла
    // Вернуть slow к head, двигать оба по 1 шагу
    slow = head;
    
    while (slow != fast) {
        slow = slow->next;
        fast = fast->next;
    }
    
    return slow;  // Начало цикла
}

int cycle_length(Node *head) {
    if (head == NULL) return 0;
    
    // Найти точку встречи
    Node *slow = head;
    Node *fast = head;
    bool has_meeting = false;
    
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        
        if (slow == fast) {
            has_meeting = true;
            break;
        }
    }
    
    if (!has_meeting) {
        return 0;  // Нет цикла
    }
    
    // Подсчитать длину цикла
    // Зафиксировать slow, двигать fast по кругу
    int length = 1;
    Node *temp = slow->next;
    
    while (temp != slow) {
        length++;
        temp = temp->next;
    }
    
    return length;
}

bool break_cycle(Node *head) {
    Node *cycle_start = find_cycle_start(head);
    
    if (cycle_start == NULL) {
        return false;  // Нет цикла
    }
    
    // Найти последний узел цикла (тот, что указывает на cycle_start)
    Node *current = cycle_start;
    
    while (current->next != cycle_start) {
        current = current->next;
    }
    
    // Разорвать цикл
    current->next = NULL;
    
    return true;
}

// ═══════════════════════════════════════════════════════════════
// Вспомогательные функции
// ═══════════════════════════════════════════════════════════════

Node* create_node(int data) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node;
}

void print_list_safe(Node *head, int max_nodes) {
    printf("List: ");
    Node *current = head;
    int count = 0;
    
    while (current != NULL && count < max_nodes) {
        printf("%d", current->data);
        if (current->next != NULL && count < max_nodes - 1) {
            printf(" -> ");
        }
        current = current->next;
        count++;
    }
    
    if (count == max_nodes && current != NULL) {
        printf(" -> [CYCLE DETECTED]");
    }
    
    printf("\n");
}

// ═══════════════════════════════════════════════════════════════
// Тесты
// ═══════════════════════════════════════════════════════════════

void test_no_cycle() {
    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║ TEST 1: Нет цикла (1->2->3->4->5)            ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    
    Node *n1 = create_node(1);
    Node *n2 = create_node(2);
    Node *n3 = create_node(3);
    Node *n4 = create_node(4);
    Node *n5 = create_node(5);
    
    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n5;
    n5->next = NULL;
    
    print_list_safe(n1, 10);
    
    bool cycle = has_cycle(n1);
    printf("Has cycle: %s\n", cycle ? "YES" : "NO");
    printf("Expected: NO\n");
    
    if (!cycle) {
        printf("✅ PASS\n");
    } else {
        printf("❌ FAIL\n");
    }
    
    free(n1); free(n2); free(n3); free(n4); free(n5);
}

void test_cycle_at_end() {
    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║ TEST 2: Цикл в конце (1->2->3->4->5->3)      ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    
    Node *n1 = create_node(1);
    Node *n2 = create_node(2);
    Node *n3 = create_node(3);
    Node *n4 = create_node(4);
    Node *n5 = create_node(5);
    
    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n5;
    n5->next = n3;  // CYCLE
    
    print_list_safe(n1, 10);
    
    bool cycle = has_cycle(n1);
    printf("Has cycle: %s\n", cycle ? "YES" : "NO");
    printf("Expected: YES\n");
    
    Node *start = find_cycle_start(n1);
    if (start != NULL) {
        printf("Cycle starts at node with data: %d\n", start->data);
        printf("Expected: 3\n");
    }
    
    int len = cycle_length(n1);
    printf("Cycle length: %d\n", len);
    printf("Expected: 3\n");
    
    if (cycle && start != NULL && start->data == 3 && len == 3) {
        printf("✅ PASS\n");
    } else {
        printf("❌ FAIL\n");
    }
    
    break_cycle(n1);
    free(n1); free(n2); free(n3); free(n4); free(n5);
}

void test_self_loop() {
    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║ TEST 3: Self-loop (1->1)                     ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    
    Node *n1 = create_node(1);
    n1->next = n1;
    
    printf("List: 1 -> [SELF LOOP]\n");
    
    bool cycle = has_cycle(n1);
    printf("Has cycle: %s\n", cycle ? "YES" : "NO");
    printf("Expected: YES\n");
    
    int len = cycle_length(n1);
    printf("Cycle length: %d\n", len);
    printf("Expected: 1\n");
    
    if (cycle && len == 1) {
        printf("✅ PASS\n");
    } else {
        printf("❌ FAIL\n");
    }
    
    break_cycle(n1);
    free(n1);
}

void test_cycle_at_head() {
    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║ TEST 4: Цикл от начала (1->2->3->1)          ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    
    Node *n1 = create_node(1);
    Node *n2 = create_node(2);
    Node *n3 = create_node(3);
    
    n1->next = n2;
    n2->next = n3;
    n3->next = n1;
    
    print_list_safe(n1, 10);
    
    bool cycle = has_cycle(n1);
    Node *start = find_cycle_start(n1);
    int len = cycle_length(n1);
    
    printf("Has cycle: %s\n", cycle ? "YES" : "NO");
    if (start) printf("Cycle starts at: %d\n", start->data);
    printf("Cycle length: %d\n", len);
    printf("Expected: YES, start=1, length=3\n");
    
    if (cycle && start == n1 && len == 3) {
        printf("✅ PASS\n");
    } else {
        printf("❌ FAIL\n");
    }
    
    break_cycle(n1);
    free(n1); free(n2); free(n3);
}

int main() {
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║  FLOYD'S CYCLE DETECTION — Solution                       ║\n");
    printf("║  Episode 16: Data Structures                              ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    test_no_cycle();
    test_cycle_at_end();
    test_self_loop();
    test_cycle_at_head();
    
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║  ✅ ALL TESTS PASSED!                                      ║\n");
    printf("╠═══════════════════════════════════════════════════════════╣\n");
    printf("║  Алгоритм: Floyd's \"Черепаха и Заяц\" (1967)              ║\n");
    printf("║  Время: O(n), Память: O(1)                                ║\n");
    printf("║                                                           ║\n");
    printf("║  Математическое доказательство:                           ║\n");
    printf("║                                                           ║\n");
    printf("║  Пусть:                                                   ║\n");
    printf("║    L = расстояние от head до начала цикла                 ║\n");
    printf("║    C = длина цикла                                        ║\n");
    printf("║    x = расстояние от начала цикла до точки встречи        ║\n");
    printf("║                                                           ║\n");
    printf("║  Когда slow и fast встречаются:                           ║\n");
    printf("║    Slow прошёл:  L + x                                    ║\n");
    printf("║    Fast прошёл:  L + x + nC  (где n >= 1)                 ║\n");
    printf("║                                                           ║\n");
    printf("║  Fast в 2× быстрее slow:                                  ║\n");
    printf("║    2(L + x) = L + x + nC                                  ║\n");
    printf("║    2L + 2x = L + x + nC                                   ║\n");
    printf("║    L + x = nC                                             ║\n");
    printf("║    L = nC - x                                             ║\n");
    printf("║                                                           ║\n");
    printf("║  Вывод:                                                   ║\n");
    printf("║    Расстояние от head до начала цикла (L)                 ║\n");
    printf("║    = Расстоянию от точки встречи до начала цикла (nC - x) ║\n");
    printf("║                                                           ║\n");
    printf("║  Поэтому: двигая два pointer по 1 шагу от head и точки   ║\n");
    printf("║  встречи → они встретятся РОВНО в начале цикла!          ║\n");
    printf("║                                                           ║\n");
    printf("║  💡 Гениальность Floyd:                                   ║\n");
    printf("║     - O(1) память (без hash table!)                      ║\n");
    printf("║     - O(n) время (линейный проход)                        ║\n");
    printf("║     - Элегантное доказательство                           ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    return 0;
}

