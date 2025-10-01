/**
 * ═══════════════════════════════════════════════════════════════
 * Episode 16: Data Structures — Промежуточная задача
 * ═══════════════════════════════════════════════════════════════
 * 
 * ЗАДАЧА: Floyd's Cycle Detection Algorithm
 * 
 * В Episode 06 (Season 2) вы столкнулись с "pointer chase":
 * две цепочки указателей, одна из которых была зациклена!
 * 
 * Теперь реализуем УНИВЕРСАЛЬНЫЙ детектор циклов в linked list.
 * 
 * Алгоритм: "Черепаха и Заяц" (Floyd, 1967)
 * Время: O(n), Память: O(1)
 * 
 * ═══════════════════════════════════════════════════════════════
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ═══════════════════════════════════════════════════════════════
// Структуры данных
// ═══════════════════════════════════════════════════════════════

typedef struct Node {
    int data;
    struct Node *next;
} Node;

// ═══════════════════════════════════════════════════════════════
// ЗАДАНИЕ 1: Floyd's Cycle Detection ⭐⭐⭐☆☆
// ═══════════════════════════════════════════════════════════════

/**
 * Определяет наличие цикла в linked list.
 * 
 * Алгоритм "Черепаха и Заяц":
 * - Slow pointer движется на 1 шаг
 * - Fast pointer движется на 2 шага
 * - Если есть цикл → встретятся
 * - Если нет цикла → fast дойдёт до NULL
 * 
 * Время: O(n)
 * Память: O(1)  ← ВАЖНО! Без доп. структур!
 * 
 * @param head  Голова списка
 * @return      true если есть цикл, false иначе
 */
bool has_cycle(Node *head) {
    // TODO: Реализуйте Floyd's algorithm
    // 
    // Подсказка:
    // Node *slow = head;
    // Node *fast = head;
    // 
    // while (fast != NULL && fast->next != NULL) {
    //     slow = slow->next;        // 1 шаг
    //     fast = fast->next->next;  // 2 шага
    //     
    //     if (slow == fast) {
    //         // Встретились! Есть цикл
    //     }
    // }
    
    return false;  // TODO: замените на реальную логику
}

// ═══════════════════════════════════════════════════════════════
// ЗАДАНИЕ 2: Найти начало цикла ⭐⭐⭐⭐☆
// ═══════════════════════════════════════════════════════════════

/**
 * Находит узел, где начинается цикл.
 * 
 * После обнаружения встречи slow и fast:
 * - Вернуть slow к head
 * - Двигать оба pointer по 1 шагу
 * - Где встретятся → начало цикла!
 * 
 * Математическое доказательство:
 * Пусть L = расстояние от head до начала цикла
 *      C = длина цикла
 *      x = расстояние от начала цикла до точки встречи
 * 
 * Slow прошёл: L + x
 * Fast прошёл: L + x + nC (где n >= 1)
 * 
 * Fast в 2× быстрее:
 * 2(L + x) = L + x + nC
 * L + x = nC
 * L = nC - x
 * 
 * Следовательно: расстояние от head до начала цикла (L)
 * равно расстоянию от точки встречи до начала цикла (nC - x)!
 * 
 * @param head  Голова списка
 * @return      Указатель на начало цикла, или NULL если цикла нет
 */
Node* find_cycle_start(Node *head) {
    // TODO: Реализуйте поиск начала цикла
    // 
    // 1. Сначала найдите точку встречи (как в has_cycle)
    // 2. Верните slow к head
    // 3. Двигайте оба по 1 шагу
    // 4. Где встретятся → начало цикла
    
    return NULL;  // TODO
}

// ═══════════════════════════════════════════════════════════════
// ЗАДАНИЕ 3: Длина цикла ⭐⭐⭐☆☆
// ═══════════════════════════════════════════════════════════════

/**
 * Определяет длину цикла.
 * 
 * После встречи slow и fast:
 * - Зафиксировать одного
 * - Двигать другого по кругу
 * - Считать шаги до повторной встречи
 * 
 * @param head  Голова списка
 * @return      Длина цикла, или 0 если цикла нет
 */
int cycle_length(Node *head) {
    // TODO: Реализуйте подсчёт длины цикла
    
    return 0;  // TODO
}

// ═══════════════════════════════════════════════════════════════
// ЗАДАНИЕ 4: Разорвать цикл ⭐⭐⭐⭐☆
// ═══════════════════════════════════════════════════════════════

/**
 * Разрывает цикл, делая список линейным.
 * 
 * Находит последний узел цикла и устанавливает next = NULL.
 * 
 * @param head  Голова списка
 * @return      true если цикл был разорван, false если цикла не было
 */
bool break_cycle(Node *head) {
    // TODO: Найдите начало цикла
    // TODO: Найдите последний узел перед началом цикла
    // TODO: Установите его next = NULL
    
    return false;  // TODO
}

// ═══════════════════════════════════════════════════════════════
// Вспомогательные функции (уже реализованы)
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
// Тестовые сценарии
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
    
    // Cleanup
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
    n5->next = n3;  // CYCLE: 5 -> 3
    
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
    printf("Expected: 3 (nodes: 3->4->5->3)\n");
    
    if (cycle && start != NULL && start->data == 3 && len == 3) {
        printf("✅ PASS\n");
    } else {
        printf("❌ FAIL\n");
    }
    
    // Разрываем цикл перед cleanup
    break_cycle(n1);
    
    // Cleanup
    free(n1); free(n2); free(n3); free(n4); free(n5);
}

void test_self_loop() {
    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║ TEST 3: Self-loop (1->1)                     ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    
    Node *n1 = create_node(1);
    n1->next = n1;  // CYCLE: points to itself
    
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
    
    // Разрываем цикл
    break_cycle(n1);
    
    // Cleanup
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
    n3->next = n1;  // CYCLE: back to head
    
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
    
    // Разрываем цикл
    break_cycle(n1);
    
    // Cleanup
    free(n1); free(n2); free(n3);
}

// ═══════════════════════════════════════════════════════════════
// Main
// ═══════════════════════════════════════════════════════════════

int main() {
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║  FLOYD'S CYCLE DETECTION — Linked List Cycles             ║\n");
    printf("║  Episode 16: Data Structures                              ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    test_no_cycle();
    test_cycle_at_end();
    test_self_loop();
    test_cycle_at_head();
    
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║  📊 FLOYD'S ALGORITHM SUMMARY                              ║\n");
    printf("╠═══════════════════════════════════════════════════════════╣\n");
    printf("║  Алгоритм: \"Черепаха и Заяц\" (Floyd, 1967)               ║\n");
    printf("║                                                           ║\n");
    printf("║  Время: O(n)                                              ║\n");
    printf("║  Память: O(1)  ← КРИТИЧНО! Без доп. структур!            ║\n");
    printf("║                                                           ║\n");
    printf("║  Применение:                                              ║\n");
    printf("║  1. Детекция циклов в linked lists                        ║\n");
    printf("║  2. Поиск дубликатов в массивах (Leetcode 287)            ║\n");
    printf("║  3. Алгоритмы криптографии (rho-метод Полларда)           ║\n");
    printf("║  4. Генераторы псевдослучайных чисел                      ║\n");
    printf("║                                                           ║\n");
    printf("║  Season 2 Episode 06 connection:                          ║\n");
    printf("║  Тогда вы вручную проходили цепочку указателей.          ║\n");
    printf("║  Теперь вы знаете ОПТИМАЛЬНЫЙ алгоритм O(1) памяти!      ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    
    return 0;
}

