/*
 * OPERATION MOONLIGHT — SEASON PROJECT: iot_controller
 *
 * Собирается из модулей ДВУХ сезонов:
 *   season-4/lib/ml_hash.c     — реестр устройств по имени   (s04e03, s04e04)
 *   season-6/lib/ml_crc.c      — CRC-16/MODBUS               (s06e05)
 *   season-6/lib/ml_wiegand.c  — карты доступа Wiegand-26    (s06e04)
 *
 * Контроллер периметра: опрашивает устройства по Modbus и проверяет пропуска.
 */
#include <stdio.h>
#include <string.h>

#include "ml_hash.h"      /* Season 4 */
#include "ml_crc.h"       /* Season 6 */
#include "ml_wiegand.h"   /* Season 6 */

#define NDEV 5
#define AUTH_FACILITY 42

int main(void) {
    printf("=== iot_controller: сборка из модулей S4 + S6 ===\n\n");

    static const char *devs[NDEV] = {"gate.north", "gate.south", "hvac.main", "pump.a", "cam.roof"};

    printf("--- модуль Season 4 (ml_hash): адресация устройств ---\n");
    for (int i = 0; i < NDEV; i++)
        printf("  %-12s -> slave id %zu\n", devs[i], 1 + ml_hash_bucket(devs[i], 16));

    printf("\n--- модуль Season 6 (ml_crc): опрос по Modbus ---\n");
    int ok_frames = 0, bad_frames = 0;
    for (int i = 0; i < NDEV; i++) {
        unsigned char frame[8];
        frame[0] = (unsigned char)(1 + ml_hash_bucket(devs[i], 16));   /* адрес из S4 */
        frame[1] = 0x03;
        frame[2] = 0x00; frame[3] = (unsigned char)(0x6B + i);
        frame[4] = 0x00; frame[5] = 0x03;
        unsigned short crc = ml_crc16_modbus(frame, 6);
        frame[6] = (unsigned char)(crc & 0xFF);
        frame[7] = (unsigned char)(crc >> 8);

        if (i == 3) frame[4] ^= 0xFF;               /* один кадр портим */

        int good = ml_crc16_frame_ok(frame, 8);
        good ? ok_frames++ : bad_frames++;
        printf("  %-12s кадр: %s\n", devs[i], good ? "CRC ok" : "CRC FAIL — отброшен");
    }
    printf("  принято %d, отброшено %d, баланс: %s\n\n",
           ok_frames, bad_frames, ok_frames + bad_frames == NDEV ? "сходится" : "НЕ СХОДИТСЯ");

    printf("--- модуль Season 6 (ml_wiegand): контроль доступа ---\n");
    struct { const char *who; uint8_t fac; uint16_t card; int corrupt; } badges[] = {
        {"сотрудник НИИ", 42, 1337, 0},
        {"подрядчик",     42, 9001, 0},
        {"клон карты",    99,    5, 0},
        {"битая карта",   42, 1337, 1},
    };
    int granted = 0, denied = 0;
    for (size_t i = 0; i < sizeof badges / sizeof badges[0]; i++) {
        uint32_t code = ml_wiegand_encode(badges[i].fac, badges[i].card);
        if (badges[i].corrupt) code ^= (1u << 5);

        uint8_t fac = 0; uint16_t card = 0;
        int valid = ml_wiegand_decode(code, &fac, &card);
        const char *verdict;
        if (!valid)                       verdict = "DENY (кадр повреждён)";
        else if (fac != AUTH_FACILITY)    verdict = "DENY (чужой объект)";
        else                            { verdict = "GRANT"; granted++; }
        if (strcmp(verdict, "GRANT") != 0) denied++;

        if (valid) printf("  %-15s facility %3u card %5u -> %s\n", badges[i].who, fac, card, verdict);
        else       printf("  %-15s (поля не извлечены)      -> %s\n", badges[i].who, verdict);
    }
    printf("  пропущено %d, отказано %d\n\n", granted, denied);

    printf("--- состав сборки ---\n");
    printf("  [S4] ml_hash.c    — адресация устройств\n");
    printf("  [S6] ml_crc.c     — целостность кадров\n");
    printf("  [S6] ml_wiegand.c — карты доступа\n");
    printf("  [S6] main.c       — контроллер\n");
    printf("\n4 единицы трансляции из 2 сезонов слинкованы в iot_controller.\n");

    int ok = (ok_frames + bad_frames == NDEV) && (granted == 2) && (denied == 2);
    printf("сборка работоспособна: %s\n", ok ? "да" : "НЕТ");
    return ok ? 0 : 1;
}
