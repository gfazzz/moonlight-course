/*
 * OPERATION MOONLIGHT — s10e05 "Защита входных данных"
 * Каркас. Скопируй в artifacts/harden.c и реализуй.
 *
 *   cp starter.c artifacts/harden.c
 *   make test
 *
 * МАКСИМАЛЬНАЯ АВТОНОМИЯ. Требования — в mission.md, вывод — в tests/expected.txt.
 *
 * Формат пакета: [magic:2][channel:1][len:1][payload:len][crc:1]
 *   magic = 'M','L';  crc = XOR всех байт до самого crc.
 *
 * Всё, что пришло снаружи, считается враждебным. Порядок проверок важен:
 * нельзя обращаться к полю, не убедившись, что оно вообще есть в пакете.
 *
 * Отдельное требование: сравнения не должны переполняться сами.
 * Плохо:  if (len + 5 > size)     — сумма может «завернуться»
 * Хорошо: if (size - 5 < len)     — вычитание безопасно, если size >= 5 уже проверено
 *
 * Тест собирает твой код с -fsanitize=address,undefined: любое нарушение
 * памяти или UB завалит проверку.
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MAGIC_HI   0x4D
#define MAGIC_LO   0x4C
#define MAXPAYLOAD 64
#define NCHANNELS  8

typedef struct {
    const unsigned char *raw;
    size_t size;
} Packet;

int main(void) {
    printf("=== защита разбора недоверенного пакета ===\n");
    printf("формат: [magic:2][channel:1][len:1][payload][crc:1], буфер %d байт\n\n", MAXPAYLOAD);
    return 0;
}
