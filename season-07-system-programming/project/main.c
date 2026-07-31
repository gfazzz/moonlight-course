/*
 * OPERATION MOONLIGHT — SEASON PROJECT: stealth_agent
 *
 * Собирается из модулей ТРЁХ сезонов:
 *   season-01/lib/ml_crypto.c — XOR-шифр для скрытия полезной нагрузки (s01e06)
 *   season-06/lib/ml_crc.c    — целостность передаваемых кадров        (s06e05)
 *   season-07/lib/ml_proc.c   — запуск программ через fork+exec        (s07e02)
 *
 * Агент: запускает внешние команды, шифрует результат и упаковывает его
 * в кадр с контрольной суммой.
 */
#include <stdio.h>
#include <string.h>

#include "ml_crypto.h"   /* Season 1 */
#include "ml_crc.h"      /* Season 6 */
#include "ml_proc.h"     /* Season 7 */

int main(void) {
    printf("=== stealth_agent: сборка из модулей S1 + S6 + S7 ===\n\n");

    printf("--- модуль Season 7 (ml_proc): запуск команд ---\n");
    struct { const char *desc; const char *cmd; int expect; } tasks[] = {
        {"успешная команда",       "exit 0",   0},
        {"команда с ошибкой",      "exit 3",   3},
        {"несуществующая утилита", "/nonexistent/x 2>/dev/null", 127},
    };
    int proc_ok = 1;
    for (size_t i = 0; i < sizeof tasks / sizeof tasks[0]; i++) {
        int rc = ml_run_shell(tasks[i].cmd);
        if (rc != tasks[i].expect) proc_ok = 0;
        printf("  %-24s код %3d (ожидался %d) %s\n",
               tasks[i].desc, rc, tasks[i].expect, rc == tasks[i].expect ? "" : " <-- расхождение");
    }
    printf("  коды возврата корректны: %s\n\n", proc_ok ? "да" : "НЕТ");

    printf("--- модуль Season 1 (ml_crypto): скрытие полезной нагрузки ---\n");
    const char *report = "AGENT ALIVE / NO DETECTION";
    unsigned char payload[64];
    size_t len = strlen(report);
    memcpy(payload, report, len);

    unsigned char plain_sum = ml_checksum(payload, len);
    ml_xor_crypt(payload, len, 0x3C);
    printf("  открытый текст:  \"%s\" (сумма %u)\n", report, plain_sum);
    printf("  после шифрования первые байты: %02X %02X %02X %02X\n",
           payload[0], payload[1], payload[2], payload[3]);

    printf("\n--- модуль Season 6 (ml_crc): упаковка в кадр ---\n");
    unsigned char frame[80];
    frame[0] = 0x07;                          /* адрес агента */
    frame[1] = (unsigned char)len;
    memcpy(frame + 2, payload, len);
    unsigned short crc = ml_crc16_modbus(frame, len + 2);
    frame[len + 2] = (unsigned char)(crc & 0xFF);
    frame[len + 3] = (unsigned char)(crc >> 8);
    size_t flen = len + 4;

    printf("  кадр собран: %zu байт, CRC ok: %s\n", flen, ml_crc16_frame_ok(frame, flen) ? "да" : "НЕТ");

    /* Приёмная сторона: проверяет CRC, расшифровывает, сверяет сумму. */
    int crc_ok = ml_crc16_frame_ok(frame, flen);
    unsigned char recv[64];
    memcpy(recv, frame + 2, len);
    ml_xor_crypt(recv, len, 0x3C);
    int restored = (memcmp(recv, report, len) == 0) && (ml_checksum(recv, len) == plain_sum);
    printf("  приём: CRC %s, текст восстановлен: %s\n",
           crc_ok ? "сошёлся" : "НЕ сошёлся", restored ? "да" : "НЕТ");

    /* Повреждение кадра обязано быть замечено. */
    frame[5] ^= 0xFF;
    printf("  после повреждения байта: CRC %s\n\n",
           ml_crc16_frame_ok(frame, flen) ? "сошёлся (ПЛОХО)" : "не сошёлся (верно)");

    printf("--- состав сборки ---\n");
    printf("  [S1] ml_crypto.c — XOR-шифр\n");
    printf("  [S6] ml_crc.c    — CRC-16 кадра\n");
    printf("  [S7] ml_proc.c   — fork + exec\n");
    printf("  [S7] main.c      — агент\n");
    printf("\n4 единицы трансляции из 3 сезонов слинкованы в stealth_agent.\n");

    int ok = proc_ok && crc_ok && restored && !ml_crc16_frame_ok(frame, flen);
    printf("сборка работоспособна: %s\n", ok ? "да" : "НЕТ");
    return ok ? 0 : 1;
}
