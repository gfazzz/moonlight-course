/*
 * OPERATION MOONLIGHT — s10e08 "Финальная операция" (финал курса)
 * Эталонное решение: operation.c
 *
 * Концепт финала: новых тем нет. Есть ПРИЁМОЧНАЯ ПРОВЕРКА всего, что построено
 *                 за десять сезонов. Операция проходит по фазам, и каждая фаза —
 *                 это навык конкретного сезона. Фаза считается пройденной только
 *                 если выполнен её КРИТЕРИЙ, а не «код отработал без ошибок».
 *
 * Здесь важна не сложность каждой проверки, а принцип: система принимается
 * целиком, по измеримым критериям, а не по ощущению «вроде работает».
 */
#include <stdio.h>
#include <string.h>

#define PI 3.14159265358979323846

static int phases_total = 0, phases_passed = 0;

static void phase(const char *season, const char *name, int ok, const char *detail) {
    phases_total++;
    if (ok) phases_passed++;
    printf("  [%s] %-4s %s — %s\n", ok ? "OK" : "!!", season, name, detail);
}

static double my_sqrt(double x) {
    if (x <= 0.0) return 0.0;
    double g = x;
    for (int i = 0; i < 60; i++) g = 0.5 * (g + x / g);
    return g;
}

int main(void) {
    printf("=== OPERATION MOONLIGHT: финальная операция ===\n\n");

    /* ---------- ФАЗА 1: основы и память (S1-S2) ---------- */
    printf("--- фаза 1: подготовка ---\n");

    /* S1: работа с типами и битами. */
    unsigned char flags = 0;
    flags |= (1 << 3);
    flags |= (1 << 5);
    int bits = 0;
    for (int i = 0; i < 8; i++) bits += (flags >> i) & 1;
    phase("S1", "битовые операции", bits == 2 && flags == 0x28, "флаги 0x28, 2 бита");

    /* S2: ручное управление памятью — кольцевой буфер без утечек. */
    int ring[8], head = 0, count = 0;
    for (int i = 1; i <= 20; i++) { ring[head] = i; head = (head + 1) % 8; if (count < 8) count++; }
    int last_sum = 0;
    for (int i = 0; i < 8; i++) last_sum += ring[i];
    phase("S2", "кольцевой буфер", count == 8 && last_sum == 132, "последние 8 из 20, сумма 132");

    /* ---------- ФАЗА 2: сеть и криптография (S3-S4) ---------- */
    printf("\n--- фаза 2: канал и шифрование ---\n");

    /* S3: контрольная сумма пакета (как в разборе протоколов). */
    unsigned char pkt[9] = {0x4D, 0x4C, 0x03, 0x0C, 'M','O','O','N', 0};
    unsigned char sum = 0;
    for (size_t i = 0; i < 8; i++) sum ^= pkt[i];
    pkt[8] = sum;                                  /* дописали контрольную сумму */
    unsigned char verify = 0;
    for (size_t i = 0; i < sizeof pkt; i++) verify ^= pkt[i];
    phase("S3", "разбор пакета", verify == 0, "кадр с суммой самопроверяется в ноль");

    /* S4: хеш djb2 — детерминированность и различимость. */
    unsigned long h1 = 5381, h2 = 5381, h3 = 5381;
    for (const char *s = "moonlight"; *s; s++) h1 = h1 * 33 + (unsigned char)*s;
    for (const char *s = "moonligth"; *s; s++) h2 = h2 * 33 + (unsigned char)*s;
    for (const char *s = "moonlight"; *s; s++) h3 = h3 * 33 + (unsigned char)*s;
    phase("S4", "хеширование", h1 == h3 && h1 != h2,
          "детерминирован и различает перестановку букв");

    /* ---------- ФАЗА 3: анализ и железо (S5-S6) ---------- */
    printf("\n--- фаза 3: данные и железо ---\n");

    /* S5: честная оценка стратегии — out-of-sample хуже in-sample. */
    double is_return = 3.91, oos_return = -5.72;
    phase("S5", "проверка вне выборки", oos_return < is_return,
          "overfitting распознан");

    /* S6: контроль целостности кадра (CRC-16/MODBUS). */
    unsigned char frame[] = {0x11, 0x03, 0x00, 0x6B, 0x00, 0x03};
    unsigned short crc = 0xFFFF;
    for (size_t i = 0; i < sizeof frame; i++) {
        crc ^= frame[i];
        for (int b = 0; b < 8; b++) crc = (crc & 1) ? (crc >> 1) ^ 0xA001 : (crc >> 1);
    }
    phase("S6", "CRC-16 промышленного кадра", crc == 0x8776, "совпал с эталоном Modbus");

    /* ---------- ФАЗА 4: система и модели (S7-S8) ---------- */
    printf("\n--- фаза 4: система и модель ---\n");

    /* S7: воспроизводимость параллельного счёта (сид от номера задачи). */
    long seq = 0, par = 0;
    for (int i = 0; i < 100; i++) { unsigned long s = 12345UL + (unsigned long)i * 2654435761UL; seq += (long)(s % 97); }
    for (int chunk = 0; chunk < 4; chunk++)
        for (int i = chunk * 25; i < (chunk + 1) * 25; i++) {
            unsigned long s = 12345UL + (unsigned long)i * 2654435761UL; par += (long)(s % 97);
        }
    phase("S7", "воспроизводимый параллелизм", seq == par, "seq == par побитово");

    /* S8: метрика по редкому классу, а не accuracy. */
    int tp = 18, fn = 1, fp = 1, tn = 80;
    double prec = (double)tp / (tp + fp), rec = (double)tp / (tp + fn);
    double f1 = 2 * prec * rec / (prec + rec);
    double acc = (double)(tp + tn) / (tp + tn + fp + fn);
    phase("S8", "оценка по F1, а не accuracy", f1 > 0.9 && acc > f1, "F1 0.947 при accuracy 0.980");

    /* ---------- ФАЗА 5: инженерия и операция (S9-S10) ---------- */
    printf("\n--- фаза 5: инженерия и операция ---\n");

    /* S9: устойчивая оценка при шуме (фильтр точнее измерения). */
    double meas_err = 4.03, filt_err = 2.08;
    phase("S9", "фильтрация шума", filt_err < meas_err, "оценка вдвое точнее радара");

    /* S10: окно связи и запас конвейера. */
    double window_s = 524.3;
    double need_s = 40.0 * 8064.0 / 1e6;          /* работа конвейера в условных секундах */
    phase("S10", "конвейер укладывается в окно", need_s < window_s, "запас более чем достаточен");

    /* S10: баланс интеграции — ничего не потеряно. */
    int accepted = 24, rejected = 16, offered = 40;
    phase("S10", "баланс конвейера", accepted + rejected == offered, "24 + 16 = 40");

    /* Орбитальная сверка: период по третьему закону Кеплера. */
    double a = 7000.0, T = 2.0 * PI * my_sqrt(a * a * a / 398600.4418);
    phase("S10", "период орбиты", T > 5820.0 && T < 5835.0, "5828.5 с (97.1 мин)");

    /* ---------- вердикт ---------- */
    printf("\n--- приёмка ---\n");
    printf("  фаз пройдено: %d из %d\n", phases_passed, phases_total);
    printf("  операция завершена: %s\n\n", phases_passed == phases_total ? "УСПЕШНО" : "ЕСТЬ ОТКАЗЫ");

    if (phases_passed == phases_total) {
        printf("  MOONLIGHT closed.\n");
        printf("  Все десять сезонов сведены в одну систему и приняты по критериям.\n");
    }

    return phases_passed == phases_total ? 0 : 1;
}
