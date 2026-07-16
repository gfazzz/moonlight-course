# Mission — s01e04 «Первый инструмент»

## Что сдать
Модуль `crypto` (`crypto.h` + `crypto.c`) и главную программу `decoder.c`, собранные в один исполняемый файл. Декодер использует функции модуля, а не копипаст XOR.

## Дано
- Сообщение V. (те же 75 байт) — в `decoder.c`.
- Каркас модуля — в `starter/` (crypto.h, crypto.c, decoder.c).

## Критерии приёмки
- [ ] `crypto.h` защищён header guard (`#ifndef/#define/#endif`).
- [ ] `crypto.c` реализует `xor_crypt()` и `checksum()`; прототипы — в `crypto.h`.
- [ ] `decoder.c` включает `crypto.h` и вызывает функции модуля (без своей копии XOR).
- [ ] Многофайловая сборка `gcc crypto.c decoder.c` без предупреждений.
- [ ] Вывод совпадает с `tests/expected.txt` (сообщение + `Checksum: 0xA4`).
- [ ] `make test` → `PASS`.

## Как проверить
```bash
cp -r starter/* artifacts/     # crypto.h, crypto.c, decoder.c
# ... правишь artifacts/*.c ...
make test
```

## Оценка времени
~40–55 минут.

## Результат в сюжете
Три разрозненные программы становятся одним инструментом с переиспользуемым модулем. Это первый камень в фундамент Season Project `moonlight_decoder`.
