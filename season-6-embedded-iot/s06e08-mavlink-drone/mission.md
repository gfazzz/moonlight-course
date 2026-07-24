# Mission — s06e08 «MAVLink / дроны» (финал Season 6)

## Что сдать
`artifacts/mavlink.c`: декодер MAVLink v1 — проверить контрольную сумму X.25, извлечь телеметрию дрона (координаты, высота, заряд), отбраковать повреждённый кадр; финальная интеграция `iot_controller`.

## Дано
- Кадр v1: `0xFE | LEN | SEQ | SYSID | COMPID | MSGID | payload | CK_A CK_B`.
- Payload (13 байт, little-endian): `lat` (int32, degE7), `lon` (int32, degE7), `alt` (int32, mm), `batt` (uint8, %).
- Checksum X.25/MCRF4XX: init `0xFFFF`, по `LEN..payload` + `CRC_EXTRA` (magic не входит).

## Критерии приёмки
- [ ] Компилируется с `-Wall -Wextra -std=c11` без предупреждений.
- [ ] `crc_acc` реализует шаг X.25 точно; `frame_crc` добавляет `CRC_EXTRA`.
- [ ] Проверяется magic-байт и контрольная сумма; несовпадение → кадр отвергнут.
- [ ] Поля payload читаются как little-endian int32; координаты делятся на 1e7, высота на 1000.
- [ ] Повреждённый кадр (искажён байт) даёт checksum FAIL.
- [ ] Вывод совпадает с `tests/expected.txt` (`lat=54.8486 lon=83.0974 alt=120.0 m battery=47%`); `make test` → `PASS`.

## Как проверить
```bash
cp starter.c artifacts/mavlink.c
make test
```

## Оценка времени
~50–60 минут.

## Результат в сюжете
Ты читаешь телеметрию дронов периметра: где они, на какой высоте, сколько заряда. Последняя слепая зона закрыта. Всё железо сезона собрано в одну систему — `iot_controller`.
