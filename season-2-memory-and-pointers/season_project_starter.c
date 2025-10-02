/**
 * Season 2: Season Project — memory_manager
 * Operation MOONLIGHT
 * 
 * ЗАДАНИЕ: Собрать универсальный memory manager из компонентов Episodes 05-08
 * 
 * КОМПОНЕНТЫ:
 * - Episode 05 → array_tools.c/.h     (работа с массивами, парсинг)
 * - Episode 06 → pointer_utils.c/.h   (указатели, циклы, linked lists)
 * - Episode 07 → dynamic_alloc.c/.h   (malloc/free wrapper, tracking)
 * - Episode 08 → leak_detector.c/.h   (поиск утечек, статистика)
 * 
 * ИНСТРУКЦИИ:
 * 1. Создай 4 модуля (.h и .c файлы)
 * 2. Раскомментируй #include ниже
 * 3. Реализуй функции main()
 * 4. Обнови Makefile (раскомментируй строки)
 * 5. Собери: make season_project
 * 6. Запусти: ./memory_manager
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// TODO: Раскомментируй после создания .h файлов
// #include "array_tools.h"
// #include "pointer_utils.h"
// #include "dynamic_alloc.h"
// #include "leak_detector.h"

/* ========================================================================
 * MAIN ПРОГРАММА
 * ======================================================================== */

int main(void) {
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║          MEMORY MANAGER — SEASON 2 PROJECT               ║\n");
    printf("║              OPERATION MOONLIGHT                         ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n\n");

    // ====================================================================
    // ШАГ 1: ИНИЦИАЛИЗАЦИЯ LEAK DETECTOR
    // ====================================================================
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("ШАГ 1: Инициализация системы отслеживания памяти\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    // TODO: Вызови leak_detector_init()
    // leak_detector_init();
    printf("⚠️  TODO: Раскомментируй вызов leak_detector_init()\n\n");

    // ====================================================================
    // ШАГ 2: ТЕСТ ARRAY_TOOLS
    // ====================================================================
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("ШАГ 2: Анализ массивов (Episode 05)\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    // TODO: Создай тестовый массив и анализируй его
    // uint8_t test_data[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    // size_t size = sizeof(test_data) / sizeof(test_data[0]);
    // 
    // printf("Тестовый массив: ");
    // for (size_t i = 0; i < size; i++) {
    //     printf("%u ", test_data[i]);
    // }
    // printf("\n\n");
    //
    // ArrayStats stats = analyze_byte_array(test_data, size);
    // print_array_stats(&stats);

    printf("⚠️  TODO: Раскомментируй тест array_tools\n\n");

    // ====================================================================
    // ШАГ 3: ТЕСТ POINTER_UTILS (LINKED LIST + FLOYD'S)
    // ====================================================================
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("ШАГ 3: Работа с указателями и linked lists (Episode 06)\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    // TODO: Создай linked list и проверь на циклы
    //
    // printf("3.1. Создание linked list без циклов\n");
    // Node *head = MALLOC(sizeof(Node));
    // head->data = 1;
    // head->next = MALLOC(sizeof(Node));
    // head->next->data = 2;
    // head->next->next = MALLOC(sizeof(Node));
    // head->next->next->data = 3;
    // head->next->next->next = NULL;
    //
    // printf("Список: ");
    // print_list(head);
    //
    // bool cycle = has_cycle(head);
    // printf("Есть цикл? %s\n\n", cycle ? "ДА 🔴" : "НЕТ 🟢");
    //
    // printf("3.2. Создание linked list С циклом\n");
    // head->next->next->next = head->next;  // Создаём цикл: 3 → 2
    // cycle = has_cycle(head);
    // printf("Есть цикл? %s\n\n", cycle ? "ДА 🔴" : "НЕТ 🟢");
    //
    // // ВАЖНО: Разорви цикл перед free!
    // head->next->next->next = NULL;
    //
    // // Освобождение
    // FREE(head->next->next);
    // FREE(head->next);
    // FREE(head);

    printf("⚠️  TODO: Раскомментируй тест pointer_utils\n\n");

    // ====================================================================
    // ШАГ 4: ТЕСТ DYNAMIC_ALLOC (MALLOC WRAPPER)
    // ====================================================================
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("ШАГ 4: Динамическое выделение памяти (Episode 07)\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    // TODO: Протестируй malloc/realloc/free wrapper
    //
    // printf("4.1. Тест MALLOC\n");
    // int *numbers = MALLOC(5 * sizeof(int));
    // for (int i = 0; i < 5; i++) {
    //     numbers[i] = i * 10;
    // }
    // printf("Массив создан: [%d, %d, %d, %d, %d]\n\n", 
    //        numbers[0], numbers[1], numbers[2], numbers[3], numbers[4]);
    //
    // printf("4.2. Тест REALLOC (увеличение до 10 элементов)\n");
    // numbers = REALLOC(numbers, 10 * sizeof(int));
    // for (int i = 5; i < 10; i++) {
    //     numbers[i] = i * 10;
    // }
    // printf("Массив расширен: [%d, %d, %d, %d, %d, %d, %d, %d, %d, %d]\n\n",
    //        numbers[0], numbers[1], numbers[2], numbers[3], numbers[4],
    //        numbers[5], numbers[6], numbers[7], numbers[8], numbers[9]);
    //
    // printf("4.3. Тест FREE\n");
    // FREE(numbers);
    // printf("Память освобождена\n\n");

    printf("⚠️  TODO: Раскомментируй тест dynamic_alloc\n\n");

    // ====================================================================
    // ШАГ 5: СИМУЛЯЦИЯ УТЕЧКИ (ДЛЯ ТЕСТИРОВАНИЯ LEAK DETECTOR)
    // ====================================================================
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("ШАГ 5: Симуляция утечки памяти (Episode 08)\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    // TODO: Создай утечку для тестирования
    //
    // printf("5.1. Выделяем память и ЗАБЫВАЕМ освободить (утечка!)\n");
    // int *leaked_memory = MALLOC(1024 * sizeof(int));  // 4KB утечки
    // printf("Память выделена, но не будет освобождена → утечка!\n\n");
    //
    // printf("5.2. Правильное использование (без утечки)\n");
    // char *proper_buffer = MALLOC(256);
    // // ... работа с buffer
    // FREE(proper_buffer);
    // printf("Память корректно освобождена ✅\n\n");

    printf("⚠️  TODO: Раскомментируй симуляцию утечки\n\n");

    // ====================================================================
    // ШАГ 6: СТАТИСТИКА ПАМЯТИ
    // ====================================================================
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("ШАГ 6: Статистика использования памяти\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    // TODO: Выведи статистику
    //
    // MemoryStats mem_stats = get_memory_stats();
    // print_memory_stats(&mem_stats);

    printf("⚠️  TODO: Раскомментируй вывод статистики\n\n");

    // ====================================================================
    // ШАГ 7: ФИНАЛЬНЫЙ ОТЧЁТ ОБ УТЕЧКАХ
    // ====================================================================
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("ШАГ 7: Финальный отчёт об утечках памяти\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    // TODO: Финальный отчёт
    //
    // leak_detector_report();
    //
    // printf("\n💡 ПОДСКАЗКА:\n");
    // printf("   Если отчёт показывает утечки — это нормально!\n");
    // printf("   Мы специально создали утечку в Шаге 5 для демонстрации.\n");
    // printf("   В реальном коде все утечки ДОЛЖНЫ быть исправлены!\n\n");

    printf("⚠️  TODO: Раскомментируй leak_detector_report()\n\n");

    // ====================================================================
    // ШАГ 8: ОЧИСТКА
    // ====================================================================
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("ШАГ 8: Завершение работы memory manager\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    // TODO: Очистка
    // leak_detector_cleanup();
    printf("⚠️  TODO: Раскомментируй leak_detector_cleanup()\n\n");

    // ====================================================================
    // ФИНАЛ
    // ====================================================================
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║                                                          ║\n");
    printf("║       MEMORY MANAGER — ТЕСТИРОВАНИЕ ЗАВЕРШЕНО            ║\n");
    printf("║                                                          ║\n");
    printf("║  Следующие шаги:                                         ║\n");
    printf("║  1. Создай все 4 модуля (.h и .c)                       ║\n");
    printf("║  2. Раскомментируй #include в начале файла              ║\n");
    printf("║  3. Раскомментируй TODO в main()                        ║\n");
    printf("║  4. Обнови Makefile                                     ║\n");
    printf("║  5. Собери: make season_project                         ║\n");
    printf("║  6. Протестируй: ./memory_manager                       ║\n");
    printf("║  7. Проверь утечки: leaks ./memory_manager              ║\n");
    printf("║                                                          ║\n");
    printf("║  Удачи, агент! Season 3 ждёт...                         ║\n");
    printf("║                                                          ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");

    return 0;
}

/* ========================================================================
 * ДОПОЛНИТЕЛЬНЫЕ ПОДСКАЗКИ
 * ========================================================================
 * 
 * 1. СОЗДАНИЕ МОДУЛЕЙ:
 *    Начни с простых функций. Например, в array_tools.h:
 *    
 *    typedef struct {
 *        uint8_t min;
 *        uint8_t max;
 *        double average;
 *    } ArrayStats;
 *    
 *    ArrayStats analyze_byte_array(const uint8_t *arr, size_t size);
 * 
 * 2. FLOYD'S ALGORITHM (pointer_utils.c):
 *    Используй алгоритм "черепаха и заяц" из Episode 06
 *    
 *    bool has_cycle(Node *head) {
 *        Node *slow = head;
 *        Node *fast = head;
 *        while (fast && fast->next) {
 *            slow = slow->next;
 *            fast = fast->next->next;
 *            if (slow == fast) return true;
 *        }
 *        return false;
 *    }
 * 
 * 3. MALLOC WRAPPER (dynamic_alloc.c):
 *    Логируй каждый malloc/free для отслеживания
 *    
 *    void* tracked_malloc(size_t size, const char *file, int line) {
 *        void *ptr = malloc(size);
 *        printf("[ALLOC] %zu bytes at %p (%s:%d)\n", size, ptr, file, line);
 *        // TODO: Сохрани информацию для leak_detector
 *        return ptr;
 *    }
 * 
 * 4. LEAK DETECTOR (leak_detector.c):
 *    Храни список всех аллокаций в структуре данных
 *    При вызове free() удаляй из списка
 *    В финальном отчёте выведи всё, что осталось в списке
 * 
 * 5. MAKEFILE:
 *    Добавь правило для season_project:
 *    
 *    season_project: season_project_starter.c array_tools.c pointer_utils.c \
 *                    dynamic_alloc.c leak_detector.c
 *        $(CC) $(CFLAGS) -o memory_manager $^
 * 
 * 6. ТЕСТИРОВАНИЕ:
 *    После сборки запусти:
 *    
 *    ./memory_manager
 *    
 *    Проверь утечки (macOS):
 *    leaks --atExit -- ./memory_manager
 *    
 *    Проверь утечки (Linux):
 *    valgrind --leak-check=full ./memory_manager
 * 
 * ========================================================================
 * КРИТЕРИИ УСПЕХА:
 * ========================================================================
 * 
 * ✅ Программа компилируется без ошибок и warnings
 * ✅ Все 4 модуля созданы (.h и .c)
 * ✅ array_tools работает (анализ массивов)
 * ✅ pointer_utils работает (обнаружение циклов)
 * ✅ dynamic_alloc логирует аллокации
 * ✅ leak_detector находит утечки
 * ✅ Программа выводит осмысленные результаты
 * ✅ Valgrind/leaks подтверждает утечку из Шага 5
 * 
 * ========================================================================
 * БОНУСЫ:
 * ========================================================================
 * 
 * 🌟 Визуализация heap (ASCII-арт карта памяти)
 * 🌟 Memory profiling (peak usage, график)
 * 🌟 Интеграция с valgrind (автозапуск из Makefile)
 * 🌟 Цветной вывод (красный для утечек, зелёный для OK)
 * 🌟 Документация (README для Season Project)
 * 
 * ========================================================================
 */


